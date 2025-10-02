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
	$(CC) $(SRC) -o $(OUTFILE) $(LDFLAGS)

$(OUTDIR):
	mkdir -p $(OUTDIR)

clean:
	rm -r bin