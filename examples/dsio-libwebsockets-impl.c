#include <libwebsockets.h>
#include <dsio/dsio.h>
#include <dsio/allocator.h>
#include <dsio/websocket.h>
#include "dsio-libwebsockets-impl.h"

static int
callback_dumb_increment(struct lws *wsi, enum lws_callback_reasons reason,
                        void *user, void *in, size_t len)
{
        switch (reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
                lwsl_info("dumb: LWS_CALLBACK_CLIENT_ESTABLISHED\n");
                break;

        case LWS_CALLBACK_CLOSED:
                lwsl_notice("dumb: LWS_CALLBACK_CLOSED\n");
                break;

        case LWS_CALLBACK_CLIENT_RECEIVE:
                ((char *)in)[len] = '\0';
                lwsl_info("rx %d '%s'\n", (int)len, (char *)in);
                break;

        /* because we are protocols[0] ... */

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		lwsl_err("dumb: LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n");
                break;

        case LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED:
                if ((strcmp(in, "x-webkit-deflate-frame") == 0))
                        return 1;
                if ((strcmp(in, "deflate-frame") == 0))
                        return 1;
                break;

        default:
                lwsl_info("dumb: reason = %d\n", reason);
                break;
        }

        return 0;
}

/*
 * dumb_increment protocol
 *
 * since this also happens to be protocols[0], some callbacks that are not
 * bound to a specific protocol also turn up here.
 */
static struct lws_protocols protocols[] = {
	{
		"dumb-increment-protocol",
		callback_dumb_increment,
		1000,
		2000,
	},
	{ NULL, NULL, 0, 0 }	/* end */
};

static int is_ssl_protocol(const char *proto)
{
	return strcmp(proto, "https://") == 0 || strcmp(proto, "wss://") == 0;
}

int dsio_libwebsockets_connect(char *uri,
			       const struct dsio_allocator *allocator,
			       struct dsio_websocket **ws)
{
	struct lws *wsi;
	struct lws_context *context;
	struct lws_context_creation_info ctx_info;
	struct lws_client_connect_info client_info;
	const char *prot, *p;
	char path[1024];	/* FIXME */

	if (allocator == NULL) {
		allocator = dsio_stdlib_allocator;
	}

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

	if (lws_parse_uri(strdup(uri), &prot,
			  &client_info.address,
			  &client_info.port,
			  &p)) {
		fprintf(stderr, "cannot parse URI %s\n", uri);
		return 1;
	}

	/* add back the leading / on path */
	memset(path, 0, sizeof(path));
	path[0] = '/';
	path[1] = '\0';
	strcat(path, p);
	client_info.path = path;

	printf("protocol: %s, path=%s\n", prot, client_info.path);

	client_info.context = context;
	client_info.ssl_connection = is_ssl_protocol(prot);
	client_info.host = client_info.address;
	client_info.origin = client_info.address;
	client_info.ietf_version_or_minus_one = -1;
	client_info.client_exts = NULL;
	client_info.protocol = protocols[0].name;

	if ((wsi = lws_client_connect_via_info(&client_info)) == NULL) {
		fprintf(stderr, "[Main] wsi create error.\n");
		return -1;
	}

	*ws = DSIO_MALLOC(allocator, sizeof **ws);

	if (ws == NULL) {
		return DSIO_NOMEM;
	}

	memset(*ws, 0, sizeof **ws);
	(*ws)->userdata = context;
	int n = 0;

        while (n >= 0) {
                n = lws_service(context, 0);
        }

	return 0;
}

int dsio_libwebsockets_service(struct dsio_websocket *ws)
{
	int rc;
	do {
		rc = lws_service(ws->userdata, 100);
	} while (rc == 0);

	return 0;
}
