LIB_SRCS = $(wildcard src/lib/*.c)
LIB_OBJS = $(patsubst src/lib/%.c,obj/lib/%.o,$(LIB_SRCS))

CFLAGS ?= -Wall -Wextra -ggdb -Iinclude -I.

lib/libds.so: $(LIB_OBJS) | lib
	$(LINK.c) -shared -o $@ $^

obj/lib/%.o: src/lib/%.c | obj/lib
	$(COMPILE.c) -shared -fPIC $< -o $@

clean:
	$(RM) -r $(LIB_OBJS) lib obj

obj/lib:
	mkdir -p $@

lib:
	mkdir -p $@

verify:
	@echo LIB_SRCS=$(LIB_SRCS)
	@echo LIB_OBJS=$(LIB_OBJS)

$(LIB_OBJS): Makefile
