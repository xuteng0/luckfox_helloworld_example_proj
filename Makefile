ifeq ($(APP_PARAM), )
    APP_PARAM := ../Makefile.param
    include $(APP_PARAM)
endif

export LC_ALL := C
SHELL := /bin/bash

CURRENT_DIR := $(shell pwd)

PKG_NAME := helloworld
PKG_BIN  ?= out

HELLOWORLD_CFLAGS  := $(RK_APP_OPTS) $(RK_APP_CROSS_CFLAGS)
HELLOWORLD_LDFLAGS := $(RK_APP_OPTS) $(RK_APP_LDFLAGS)

PKG_TARGET := helloworld-build

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done"

helloworld-build:
	@rm -rf $(PKG_BIN); \
	mkdir -p $(PKG_BIN)/bin
	$(RK_APP_CROSS)-gcc $(HELLOWORLD_CFLAGS) helloworld.c -o $(PKG_BIN)/bin/helloworld $(HELLOWORLD_LDFLAGS)
	$(call MAROC_COPY_PKG_TO_APP_OUTPUT, $(RK_APP_OUTPUT), $(PKG_BIN))

clean:
	@rm -rf $(PKG_BIN)

distclean: clean

info:
	@echo "This is $(PKG_NAME) for $(RK_APP_CHIP) with $(RK_APP_CROSS)"
