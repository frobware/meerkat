#prgma once

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*lexer_error_handler)(const struct src_loc *loc, const char *msg);

struct lexer {
	struct token token;
	unsigned int lexer_errors;
	unsigned int lineno;
	unsigned int column;
	const char *filename;
	/* private */
	FILE *stream;
	struct byte_buffer bb;
	int c;			/* lookahead character */
	const struct dsio_allocator *allocator;
	lexer_error_handler error_handler;
};

#ifdef __GNUC__
void lexer_error(struct lexer *l, const struct src_loc *loc, const char *fmt, ...) __attribute__((format(printf, 3, 4)));
#endif

extern void lexer_error(struct lexer *l, const struct src_loc *loc, const char *fmt, ...);
extern void lexer_default_error_handler(const struct src_loc *loc, const char *msg);
extern void lexer_init(struct lexer *l, lexer_error_handler error_handler, const char *filename, FILE *stream, struct dsio_hashtbl *strtbl, const struct dsio_allocator *allocator);
extern int lexer_next_token(struct lexer *l);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

