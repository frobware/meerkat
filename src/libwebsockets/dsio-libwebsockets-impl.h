#pragma once

extern int dsio_libwebsockets_connect(struct dsio_client *client);
extern void dsio_libwebsockets_disconnect(struct dsio_client *client);
extern int dsio_libwebsockets_msgpump(struct dsio_client *client);
