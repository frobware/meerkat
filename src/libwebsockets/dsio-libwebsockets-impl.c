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
#include <dsio/allocator.h>
#include <dsio/websocket.h>
#include "../src/core/mprintf.h"

#include <assert.h>

static char *M;
static size_t MLEN;

static int callback(struct lws *wsi,
		    enum lws_callback_reasons reason,
		    void *userdata, void *buf, size_t len)
{
	struct dsio_websocket *ws = userdata;
	unsigned char wrbuf[LWS_PRE + 4096];

	switch (reason) {
	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		return ws->on_open(ws);
	case LWS_CALLBACK_CLOSED:
		return ws->on_close(ws);
	case LWS_CALLBACK_CLIENT_RECEIVE:
		((char *)buf)[len] = '\0';
		return ws->on_message(ws, buf, len);
	case LWS_CALLBACK_CLIENT_WRITEABLE:
		if (M == NULL)
			return 0;
		lwsl_notice("LWS_CALLBACK_CLIENT_WRITEABLE\n");
		memset(wrbuf, 0, sizeof(wrbuf));
		printf("sending: %s\n", M);
		size_t l = sprintf((char *)&wrbuf[LWS_PRE], "%s", M);
		printf("l=%zd\n", l);
		printf("s=%s\n", &wrbuf[LWS_PRE]);
		size_t n = lws_write(wsi, &wrbuf[LWS_PRE], l, LWS_WRITE_TEXT);
		if (n < l) {
			lwsl_err("Partial write LWS_CALLBACK_CLIENT_WRITEABLE\n");
			return -1;
		}
		printf("n=%zd\n", n);
		M = NULL;
		lws_callback_on_writable(wsi);
		break;
	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		return ws->on_error(ws, "connection error");
	default:
		break;
	}
	return 0;
}

static struct lws_protocols protocols[2] = {
	{
		"dsio_libwebsockets",
		callback,
		0,		/* TODO */
		1280,		/* TODO */
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
	const struct lws_context *context = ws->userdata;
	M = p;
	MLEN = len;
	return lws_callback_on_writable_all_protocol(context, protocols);
}

int dsio_libwebsockets_connect(struct dsio_client_cfg *cfg, struct dsio_websocket *ws)
{
	struct lws *wsi;
	struct lws_context *context;
	struct lws_context_creation_info ctx_info;
	struct lws_client_connect_info client_info;
	const char *prot, *p;
	char *uri_cp;
	char path[1024];	/* FIXME */

	ws->send = dsio_libwebsockets_send;
	ws->cfg = cfg;

	uri_cp = dsio_mprintf(ws->cfg->allocator, "%s", ws->cfg->uri);

	if (uri_cp == NULL)
		return DSIO_NOMEM;

	/*
	 * Create the websockets context. This tracks open connections
	 * and knows how to route any traffic and which protocol
	 * version to use, and if each connection is client or server
	 * side.
	 */
	memset(&ctx_info, 0, sizeof ctx_info);
	ctx_info.port = CONTEXT_PORT_NO_LISTEN;
	ctx_info.protocols = protocols;
	ctx_info.gid = -1;
	ctx_info.uid = -1;
	ctx_info.options = 0;

	if ((context = lws_create_context(&ctx_info)) == NULL) {
		fprintf(stderr, "Creating libwebsocket context failed\n");
		return 1;
	}

	memset(&client_info, 0, sizeof client_info);

	if (lws_parse_uri(uri_cp, &prot,
			  &client_info.address,
			  &client_info.port,
			  &p)) {
		fprintf(stderr, "cannot parse URI %s\n", ws->cfg->uri);
		DSIO_FREE(ws->cfg->allocator, uri_cp);
		return 1;
	}

	/* add back the leading / to path */
	memset(path, 0, sizeof(path));
	path[0] = '/';
	path[1] = '\0';
	strcat(path, p);

	client_info.path = path;
	client_info.context = context;
	client_info.ssl_connection = is_ssl_protocol(prot);
	client_info.host = client_info.address;
	client_info.origin = client_info.address;
	client_info.ietf_version_or_minus_one = -1;
	client_info.client_exts = NULL;
	client_info.protocol = protocols[0].name;
	client_info.userdata = ws;

	printf("protocol: %s, path=%s\n", prot, client_info.path);

	if ((wsi = lws_client_connect_via_info(&client_info)) == NULL) {
		fprintf(stderr, "[Main] wsi create error.\n");
		DSIO_FREE(ws->cfg->allocator, uri_cp);
		return -1;
	}

	DSIO_FREE(ws->cfg->allocator, uri_cp);
	ws->userdata = context;
	printf("%s:%d -- context = %p\n", __FILE__, __LINE__, ws->userdata);
	return 0;
}

void dsio_libwebsockets_disconnect(struct dsio_websocket *ws)
{
}

int dsio_libwebsockets_service(struct dsio_websocket *ws)
{
	int rc;

	printf("context = %p\n", ws->userdata);

	do {
		rc = lws_service(ws->userdata, -1);
		printf("lws_service: rc=%d\n", rc);
	} while (rc == 0);

	return rc;
}
