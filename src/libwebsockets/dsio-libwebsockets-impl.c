#include <libwebsockets.h>

#include <dsio/dsio.h>
#include <dsio/allocator.h>
#include <dsio/websocket.h>
#include <dsio/message.h>
#include "../src/core/mprintf.h"

static int client_recv;

static int ws_callback(struct lws *wsi,
		       enum lws_callback_reasons reason,
		       void *userdata, void *in, size_t len)
{
	unsigned char buf[LWS_PRE + 4096];
	int l = 0;
	int n;

	switch (reason) {
	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		lwsl_notice("dumb: LWS_CALLBACK_CLIENT_ESTABLISHED\n");
		lws_callback_on_writable(wsi);
		break;
	case LWS_CALLBACK_CLOSED:
		lwsl_notice("dumb: LWS_CALLBACK_CLOSED\n");
		break;
	case LWS_CALLBACK_CLIENT_RECEIVE:
		((char *)in)[len] = '\0';
		lwsl_notice("RECV %d '%s'\n", (int)len, (char *)in);
		struct dsio_msg msg;
		printf("msg rc = %d\n", dsio_msg_parse(dsio_stdlib_allocator, (char *const)in, &msg));
		client_recv = 1;
		break;
	case LWS_CALLBACK_CLIENT_WRITEABLE:
		if (!client_recv) {
			lws_callback_on_writable(wsi);
			return 0;
		}
		memset(buf, 0, sizeof(buf));
		lwsl_notice("LWS_CALLBACK_CLIENT_WRITEABLE\n");
		l = sprintf((char *)&buf[LWS_PRE + l], "C%cCHR%c%s%c",
			    DSIO_MSG_PART_SEPARATOR,
			    DSIO_MSG_PART_SEPARATOR,
			    "ws://deepstream:6020/deepstream",
			    DSIO_MSG_RECORD_SEPARATOR);
		printf("l=%d\n", l);
		printf("s=%s\n", &buf[LWS_PRE]);
		n = lws_write(wsi, &buf[LWS_PRE], l, LWS_WRITE_TEXT);
		if (n < 0) {
			lwsl_err("write error LWS_CALLBACK_CLIENT_WRITEABLE\n");
			return -1;
		}
		if (n < l) {
			lwsl_err("Partial write LWS_CALLBACK_CLIENT_WRITEABLE\n");
			return -1;
		}
		printf("n=%d\n", n);
		/* get notified as soon as we can write again */
		/* lws_callback_on_writable(wsi); */
		break;
	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		lwsl_err("dumb: LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n");
		break;
	default:
		break;
	}

	return 0;
}

static struct lws_protocols protocols[] = {
	{
		"dsio",
		ws_callback,
		1000,		/* TODO */
		2000,		/* TODO */
		0,
		NULL
	},
	{
		NULL, 		/* name */
		NULL, 		/* callback */
		0, 		/* per_session_data_size */
		0,		/* rx_buffer_size */
		0,		/* id */
		NULL		/* userdata */
	}
};

static int is_ssl_protocol(const char *proto)
{
	return strcmp(proto, "https://") == 0 || strcmp(proto, "wss://") == 0;
}

int dsio_libwebsockets_factory(const char *uri,
			       const struct dsio_allocator *allocator,
			       struct dsio_websocket **ws)
{
	struct lws *wsi;
	struct lws_context *context;
	struct lws_context_creation_info ctx_info;
	struct lws_client_connect_info client_info;
	const char *prot, *p;
	char *uri_cp;
	char path[1024];	/* FIXME */

	if (allocator == NULL) {
		allocator = dsio_stdlib_allocator;
	}

	if ((uri_cp = dsio_mprintf(allocator, "%s", uri)) == NULL)
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

	if (lws_parse_uri(strdup(uri_cp), &prot,
			  &client_info.address,
			  &client_info.port,
			  &p)) {
		fprintf(stderr, "cannot parse URI %s\n", uri);
		DSIO_FREE(allocator, uri_cp);
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

	printf("protocol: %s, path=%s\n", prot, client_info.path);

	if ((wsi = lws_client_connect_via_info(&client_info)) == NULL) {
		fprintf(stderr, "[Main] wsi create error.\n");
		DSIO_FREE(allocator, uri_cp);
		return -1;
	}

	*ws = DSIO_MALLOC(allocator, sizeof **ws);

	if (ws == NULL) {
		DSIO_FREE(allocator, uri_cp);
		return DSIO_NOMEM;
	}

	memset(*ws, 0, sizeof **ws);
	(*ws)->userdata = context;
	DSIO_FREE(allocator, uri_cp);

	return 0;
}

int dsio_libwebsockets_service(struct dsio_websocket *ws)
{
	int rc;

	do {
		rc = lws_service(ws->userdata, 100);
	} while (rc == 0);

	return rc;
}
