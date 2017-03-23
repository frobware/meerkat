#pragma once

extern int dsio_libwebsockets_factory(const char *uri,
				      const struct dsio_allocator *a,
				      struct dsio_websocket **wsp);

extern int dsio_libwebsockets_service(struct dsio_websocket *ws);
