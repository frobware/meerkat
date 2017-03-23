#include <string.h>
#include <dsio/client.h>

int dsio_login(const struct dsio_client_cfg *cfg,
	       struct dsio_client *client,
	       DSIO_WEBSOCKET_CONNECT *wsf)
{
	memset(client, 0, sizeof *client);
	/* client->ws = (*wsf)(cfg->uri, cfg->allocator, 0); */

	if (client->ws == NULL) {
		return DSIO_CLIENT_CONNECT_ERROR;
	}

	return 0;
}
