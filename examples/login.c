#include <stdio.h>
#include <dsio/dsio.h>
#include "../src/libwebsockets/dsio-libwebsockets-impl.h"

int main(int argc, char *argv[])
{
	int rc;
	struct dsio_client_cfg client_cfg;
	struct dsio_client *client;

	if (argc != 2) {
		fprintf(stderr, "usage: <URI>\n");
		return EXIT_FAILURE;
	}

	client_cfg.allocator = dsio_stdlib_allocator;
	client_cfg.uri = argv[1];
	client_cfg.username = NULL;
	client_cfg.password = NULL;
	client_cfg.wsk_factory = dsio_libwebsockets_factory;
	
	client = dsio_login(&client_cfg);

	if (client == NULL) {
		fprintf(stderr, "out of memory!");
		exit(EXIT_FAILURE);
	}

#if 0	
	struct dsio_websocket *ws;

	rc = dsio_libwebsockets_factory(argv[1], NULL, &ws);
	printf("rc=%d\n", rc);

	dsio_libwebsockets_service(ws);
#endif
	return 0;
}
