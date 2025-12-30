CC = gcc
CFLAGS = -I include -fsanitize=address -Werror -Wall -Wextra
SRCDIR = src
BUILDDIR = build
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/cltools.c $(SRCDIR)/findpaths.c $(SRCDIR)/readnote.c $(SRCDIR)/readwriteutils.c $(SRCDIR)/writenote.c
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
TARGET = $(BUILDDIR)/main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) -fsanitize=address

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) -c $< $(CFLAGS) -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean
