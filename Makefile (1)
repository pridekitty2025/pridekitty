OBJECTS = Pridekitty.o

CC	?=
CFLAGS	 ?= -g -Wall -Wextra -std=c99 -pedantic -Wwrite-strings
CPPFLAGS ?=
LDFLAGS  ?=

all: Pridekitty

Pridekitty: $(OBJECTS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS) Pridekitty

check: all
	# Unit tests go here. None currently.
	@echo "*** ALL TESTS PASSED ***"

.PHONY: all clean check
