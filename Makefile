LIB_SRCS = $(wildcard src/lib/*.c)
LIB_OBJS = $(patsubst src/lib/%.c, obj/lib/%.o, $(LIB_SRCS))

TEST_SRCS := $(wildcard src/lib/test/test-*.c)
TEST_BINS := $(patsubst src/lib/test/test-%.c, bin/test-%, $(TEST_SRCS))

LIB_DSIO = lib/libdsio.a

CFLAGS += -Wall -pedantic -ggdb -Iinclude -MMD -coverage

.PHONY: clean verify

all: $(TEST_BINS)

$(LIB_DSIO): $(LIB_OBJS) | lib
	ar cr $@ $^

obj/lib/%.o: src/lib/%.c | obj/lib
	$(COMPILE.c) $< -o $@

obj/lib/test/%.o: src/lib/test/%.c | obj/lib/test
	$(COMPILE.c) $< -o $@

bin/test-%: src/lib/test/test-%.c | $(LIB_DSIO) $(LIB_OBJS) bin
	$(LINK.c) $< -Llib -ldsio -o $@

bin/lib/%: lib/test

obj/lib/test obj/lib bin lib:
	@mkdir -p $@

clean:
	$(RM) -r $(LIB_OBJS) lib obj bin

verify:
	@echo LIB_SRCS=$(LIB_SRCS)
	@echo LIB_OBJS=$(LIB_OBJS)
	@echo TEST_SRCS=$(TEST_SRCS)
	@echo TEST_BINS=$(TEST_BINS)

$(LIB_OBJS): Makefile
$(TEST_BINS): $(LIB_OBJS)

-include obj/lib/*.d
-include obj/lib/test/*.d
-include bin/*.d
