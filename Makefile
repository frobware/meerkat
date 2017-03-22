# Copyright 2017 Andrew McDermott
#
# Licensed under the Apache License, Version 2.0 (the "License"); you
# may not use this file except in compliance with the License. You
# may obtain a copy of the License at:
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied. See the License for the specific language governing
# permissions and limitations under the License.

LIB_SRCS = $(wildcard src/lib/*.c)
LIB_OBJS = $(patsubst src/lib/%.c,obj/lib/%.o,$(LIB_SRCS))

TEST_SRCS := $(wildcard src/lib/test/test-*.c)
TEST_BINS := $(patsubst src/lib/test/test-%.c,bin/test-%,$(TEST_SRCS))

LOGIN_SRCS := examples/login.c examples/dsio-libwebsockets-impl.c
LOGIN_OBJS := $(patsubst examples/%.c,examples/%.o,$(LOGIN_SRCS)) 

EXAMPLES := bin/login

LIB_DSIO = lib/libdsio.a

CFLAGS += -Wall -pedantic -ggdb -fno-inline -Iinclude -MMD -coverage -Werror

.PHONY: clean verify

all: $(TEST_BINS) $(EXAMPLES)

$(LIB_DSIO): $(LIB_OBJS) | lib
	ar cr $@ $^

obj/lib/%.o: src/lib/%.c | obj/lib
	$(COMPILE.c) $< -o $@

obj/lib/test/%.o: src/lib/test/%.c | obj/lib/test
	$(COMPILE.c) $< -o $@

bin/test-%: src/lib/test/test-%.c | $(LIB_DSIO) $(LIB_OBJS) bin
	$(LINK.c) $< -Llib -ldsio -o $@

bin/login: $(LOGIN_OBJS) | $(LIB_DSIO)
	$(LINK.c) $^ -Llib -ldsio $(shell pkg-config --libs libwebsockets) -o $@

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
	@echo LOGIN_SRCS=$(LOGIN_SRCS)
	@echo LOGIN_OBJS=$(LOGIN_OBJS)

$(LIB_OBJS): Makefile
$(TEST_BINS): $(LIB_OBJS)

-include obj/lib/*.d
-include obj/lib/test/*.d
-include bin/*.d
