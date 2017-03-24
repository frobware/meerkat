#include <stdio.h>
#include <string.h>
#include <dsio/client.h>
#include "mprintf.h"

static void on_open(struct dsio_client *client)
{
	printf("%s:%d\n", __FILE__, __LINE__);
}

static void on_close(struct dsio_client *client)
{
	printf("%s:%d\n", __FILE__, __LINE__);
}

static void on_error(struct dsio_client *client)
{
	printf("%s:%d\n", __FILE__, __LINE__);
}

static void on_message(struct dsio_client *client)
{
	printf("%s:%d\n", __FILE__, __LINE__);
}

int dsio_login(struct dsio_client *client, const struct dsio_client_cfg *cfg)
{
	int rc;
	memset(client, 0, sizeof *client);
 	client->cfg = cfg;
	client->buf = dsio_mprintf(cfg->allocator, "client: %p", client);
	client->on_open = on_open;
	client->on_close = on_close;
	client->on_error = on_error;
	client->on_message = on_message;
	rc = (*client->cfg->websocket_broker)(client);
	printf("websocket_broker rc=%d\n", rc);
	return rc;
}

void dsio_logout(struct dsio_client *c)
{
}

