CC = gcc
CFLAGS = -Wall -Werror -std=c99 -pedantic
DEBUGFLAGS = -g
PRODUCTIONFLAGS = -O3
BINARYNAME = exceptional_examples
SOURCEDIR := src

ALL_SOURCES := $(shell find $(SOURCEDIR) -name '*.c')

debug:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(ALL_SOURCES) -o $(BINARYNAME)

production:
	$(CC) $(CFLAGS) $(PRODUCTIONFLAGS) $(ALL_SOURCES) -o $(BINARYNAME)

