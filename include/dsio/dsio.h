#pragma once

/* Status codes. */

#define DSIO_OK		0
#define DSIO_ERROR	1

#define DSIO_NELEMENTS(A) ((sizeof((A)) / sizeof((A))[0]))
