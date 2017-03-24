#include <stdio.h>
#include <dsio/dsio.h>
#include "../src/libwebsockets/dsio-libwebsockets-impl.h"

#include <libwebsockets.h>

int main(int argc, char *argv[])
{
	int rc;
	struct dsio_client client;
	struct dsio_client_cfg client_cfg;

	if (argc != 2) {
		fprintf(stderr, "usage: <URI>\n");
		return EXIT_FAILURE;
	}

	client_cfg.allocator = dsio_stdlib_allocator;
	client_cfg.uri = argv[1];
	client_cfg.username = NULL;
	client_cfg.password = NULL;
	client_cfg.websocket_broker = dsio_libwebsockets_broker;
	
	rc = dsio_login(&client, &client_cfg);
	printf("login rc = %d\n", rc);
	dsio_libwebsockets_msgpump(&client);

	return 0;
}
