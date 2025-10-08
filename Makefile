# needs to be updated for every release
VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_PATCH = 3


CC = gcc
SRC = Flash-cli.c
OUTDIR = bin
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    LDFLAGS = -lcurl
    OUTFILE = $(OUTDIR)/flash
endif

ifeq ($(UNAME_S),Darwin)  # macOS
    LDFLAGS = -lcurl
    OUTFILE = $(OUTDIR)/flash
endif

ifeq ($(OS),Windows_NT)
    LDFLAGS = -lcurl -lws2_32 -lcrypt32
    OUTFILE = $(OUTDIR)/flash.exe
endif

all: $(OUTFILE)

$(OUTFILE): $(SRC) | $(OUTDIR)
	@echo "Generating version.h with version $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)..."
	@echo "#define RELEASE_VERSION \"$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)\"" > version.h
	@echo "Compiling $(SRC) → $(OUTFILE)..."
	$(CC) $(SRC) -o $(OUTFILE) $(LDFLAGS)

$(OUTDIR):
	mkdir -p $(OUTDIR)

clean:
	rm -r bin