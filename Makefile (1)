OBJECTS = pridekitty.o

CC	?=
CFLAGS	 ?= -g -Wall -Wextra -std=c99 -pedantic -Wwrite-strings
CPPFLAGS ?=
LDFLAGS  ?=

all: pridekitty

pridekitty: $(OBJECTS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS) pridekitty

check: all
	# Unit tests go here. None currently.
	@echo "*** ALL TESTS PASSED ***"

.PHONY: all clean check
