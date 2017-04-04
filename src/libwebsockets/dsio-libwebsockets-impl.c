/* Copyright (C) 2017 Andrew McDermott
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <libwebsockets.h>

#include <dsio/dsio.h>
#include "../src/core/mprintf.h"

#include <assert.h>

static char *M;
static size_t MLEN;

struct session_ctx {
	struct lws_context *context;
	struct dsio_websocket *ws;
	struct lws *wsi;	/* NULL if no connection */
};

static int callback_dsio(struct lws *wsi,
			 enum lws_callback_reasons reason,
			 void *userdata, void *buf, size_t len)
{
	unsigned char wrbuf[LWS_PRE + 4096];
	struct session_ctx *session = userdata;
	
	//printf("reason: %d, callback_dsio: %p\n", reason, session);

	switch (reason) {
	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		lwsl_notice("LWS_CALLBACK_CLIENT_ESTABLISHED\n");
		return session->ws->on_open(session->ws);
	case LWS_CALLBACK_CLOSED:
		lwsl_notice("LWS_CALLBACK_CLOSED\n");
		session->wsi = NULL;
		return session->ws->on_close(session->ws);
	case LWS_CALLBACK_CLIENT_RECEIVE:
		lwsl_notice("LWS_CALLBACK_CLIENT_RECEIVE\n");
		((char *)buf)[len] = '\0';
		fprintf(stderr, "lws_remaining_packet_payload() = %zd\n",
			lws_remaining_packet_payload(wsi));
		if (lws_remaining_packet_payload(wsi) != 0) {
			return 0;
		}
		return session->ws->on_message(session->ws, buf, len);
	case LWS_CALLBACK_CLIENT_WRITEABLE:
		lwsl_notice("LWS_CALLBACK_CLIENT_WRITEABLE\n");
		if (M == NULL)
			return 0;
		memset(wrbuf, 0, sizeof(wrbuf));
		lwsl_notice("SEND: %s\n", M);
		size_t l = sprintf((char *)&wrbuf[LWS_PRE], "%s", M);
		size_t n = lws_write(wsi, &wrbuf[LWS_PRE], l, LWS_WRITE_TEXT);
		if (n < l) {
			lwsl_err("Partial write LWS_CALLBACK_CLIENT_WRITEABLE\n");
			return -1;
		}
		M = NULL;
		lws_callback_on_writable(wsi);
		break;
	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		session->wsi = NULL;
		lwsl_notice("LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n");
		return session->ws->on_error(session->ws, "connection error");
	default:
		break;
	}
	return 0;
}

static struct lws_protocols protocols[2] = {
	{
		"",		/* name */
		callback_dsio,	/* callback */
		NULL,		/* per_session_data_size */
		0,		/* rx_buffer_size */
		0,
		NULL
	},
	{
		NULL,		/* name */
		NULL,		/* callback */
		0,		/* per_session_data_size */
		0,		/* rx_buffer_size */
		0,		/* id */
		NULL		/* userdata */
	}
};

static int is_ssl_protocol(const char *proto)
{
	return strcmp(proto, "https://") == 0 || strcmp(proto, "wss://") == 0;
}

static size_t dsio_libwebsockets_send(struct dsio_websocket *ws, void *p, size_t len)
{
	struct session_ctx *session = ws->userdata;
	M = p;
	MLEN = len;
	return lws_callback_on_writable_all_protocol(session->context, protocols);
}

static int ratelimit_connects(unsigned int *last, unsigned int secs)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	if (tv.tv_sec - (*last) < secs)
		return 0;

	*last = tv.tv_sec;

	return 1;
}

static int connect_server(struct dsio_websocket *ws)
{
	struct lws_client_connect_info client_info;
	const char *prot, *p;
	char *uri_cp;
	char path[1024];	/* FIXME */
	struct session_ctx *session = ws->userdata;
		
	uri_cp = dsio_mprintf(ws->cfg->allocator, "%s", ws->cfg->uri);

	if (uri_cp == NULL)
		return DSIO_NOMEM;

	memset(&client_info, 0, sizeof client_info);
	
	if (lws_parse_uri(uri_cp, &prot,
			  &client_info.address,
			  &client_info.port,
			  &p)) {
		fprintf(stderr, "cannot parse URI %s\n", ws->cfg->uri);
		DSIO_FREE(ws->cfg->allocator, uri_cp);
		return DSIO_NOMEM;
	}

	/* add back the leading / to path */
	memset(path, 0, sizeof(path));
	path[0] = '/';
	path[1] = '\0';
	strcat(path, p);

	client_info.path = path;
	client_info.context = session->context;
	client_info.ssl_connection = is_ssl_protocol(prot);
	client_info.host = client_info.address;
	client_info.origin = client_info.address;
	client_info.ietf_version_or_minus_one = -1;
	client_info.client_exts = NULL;
	client_info.protocol = protocols[0].name;
	client_info.userdata = session;

	printf("connecting to %s:%d\n", client_info.host, client_info.port);
	
	session->wsi = lws_client_connect_via_info(&client_info);

	printf("SESSION->WS: %p\n", session->ws);

	if (session->wsi == NULL) {
		fprintf(stderr, "wsi create error.\n");
		DSIO_FREE(ws->cfg->allocator, uri_cp);
		return DSIO_NOMEM;
	}

	DSIO_FREE(ws->cfg->allocator, uri_cp);
	printf("CONNECTED %s:%d -- context = %p\n", __FILE__, __LINE__, session);
	return DSIO_OK;
}

int dsio_libwebsockets_connect(struct dsio_client_cfg *cfg, struct dsio_websocket *ws)
{
	struct lws_context *context;
	struct lws_context_creation_info info;
	struct session_ctx *session;
	
	ws->userdata = NULL;
	ws->send = dsio_libwebsockets_send;
	ws->cfg = cfg;

	/*
	 * Create the websockets context. This tracks open connections
	 * and knows how to route any traffic and which protocol
	 * version to use, and if each connection is client or server
	 * side.
	 */
	memset(&info, 0, sizeof info);
	info.port = CONTEXT_PORT_NO_LISTEN;
	info.protocols = protocols;
	info.gid = -1;
	info.uid = -1;
	info.options = 0;

	if ((context = lws_create_context(&info)) == NULL) {
		fprintf(stderr, "Creating libwebsocket context failed\n");
		return 1;
	}

	if ((session = DSIO_MALLOC(ws->cfg->allocator, sizeof *session)) == NULL) {
		lws_context_destroy(context);
		return 1;
	}

	session->context = context;
	session->wsi = NULL;
	session->ws = ws;
	ws->userdata = session;
	connect_server(ws);

	return 0;
}

void dsio_libwebsockets_disconnect(struct dsio_websocket *ws)
{
}

int dsio_libwebsockets_service(struct dsio_websocket *ws)
{
	unsigned int rl_dumb = 0;

	while (1) {
		enum dsio_connection_state state;
		struct session_ctx *session = ws->userdata;

		state = dsio_client_state(ws->client);

		if (!session->wsi && ratelimit_connects(&rl_dumb, 5u)) {
			printf("client state: %s\n",
			       dsio_connection_state_names[state]);

			fprintf(stderr, "**** RECONNECTING ****\n");
			connect_server(ws);
		}

		lws_service(session->context, 1000);
	}
}
