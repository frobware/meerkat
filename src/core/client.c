#include <string.h>
#include <dsio/client.h>

struct dsio_client *dsio_login(const struct dsio_client_cfg *cfg)
{
	struct dsio_client *c;

	if ((c = DSIO_MALLOC(cfg->allocator, sizeof *c)) == NULL)
		return NULL;
	
	memset(c, 0, sizeof *c);
 	c->cfg = cfg;

	return c;
}

void dsio_logout(struct dsio_client *c)
{
	DSIO_FREE(c->cfg->allocator, c);
}

