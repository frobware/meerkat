#include <stdio.h>
#include <libwebsockets.h>
#include <dsio/dsio.h>

#include "dsio-libwebsockets-impl.h"

int main(int argc, char *argv[])
{
	int rc;
	struct dsio_websocket *ws;

	if (argc != 2) {
		fprintf(stderr, "usage: <URI>\n");
		return EXIT_FAILURE;
	}

	rc = dsio_libwebsockets_connect(argv[1], NULL, &ws);
	printf("rc=%d\n", rc);

	dsio_libwebsockets_service(ws);

	return 0;
}
