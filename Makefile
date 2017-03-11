LIB_SRCS = $(wildcard src/lib/*.c)
LIB_OBJS = $(patsubst src/lib/%.c,obj/lib/%.o,$(LIB_SRCS))

TESTS = bin/test-message

LIB_DSIO = lib/libdsio.a

CFLAGS += -Wall -pedantic -ggdb -Iinclude -MMD

.PHONY: clean verify

all: $(TESTS)

$(LIB_DSIO): $(LIB_OBJS) | lib
	ar cr $@ $^

obj/lib/%.o: src/lib/%.c | obj/lib
	$(COMPILE.c) $< -o $@

obj/lib/test/%.o: src/lib/test/%.c | obj/lib/test
	$(COMPILE.c) $< -o $@

bin/test-message: src/lib/test/test-message.c | $(LIB_DSIO) $(LIB_OBJS) bin
	$(LINK.c) src/lib/test/test-message.c -Llib -ldsio -o $@

bin/lib/%: lib/test

obj/lib/test obj/lib bin lib:
	@mkdir -p $@

clean:
	$(RM) -r $(LIB_OBJS) lib obj bin

verify:
	@echo LIB_SRCS=$(LIB_SRCS)
	@echo LIB_OBJS=$(LIB_OBJS)

$(LIB_OBJS): Makefile

-include obj/lib/*.d
-include obj/lib/test/*.d
