# Component settings
COMPONENT := css
COMPONENT_VERSION := 0.0.1
# Default to a static library
COMPONENT_TYPE ?= lib-static

# Setup the tooling
include build/makefiles/Makefile.tools

TESTRUNNER := $(PERL) build/testtools/testrunner.pl

# Toolchain flags
WARNFLAGS := -Wall -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes \
	-Wmissing-declarations -Wnested-externs -Werror -pedantic
ifneq ($(GCCVER),2)
  WARNFLAGS := $(WARNFLAGS) -Wextra
endif
CFLAGS := $(CFLAGS) -D_BSD_SOURCE -I$(CURDIR)/include/ \
	-I$(CURDIR)/src $(WARNFLAGS) 
ifneq ($(GCCVER),2)
  CFLAGS := $(CFLAGS) -std=c99
else
  # __inline__ is a GCCism
  CFLAGS := $(CFLAGS) -Dinline="__inline__"
endif

# Parserutils & wapcaplet
ifneq ($(PKGCONFIG),)
  CFLAGS := $(CFLAGS) $(shell $(PKGCONFIG) libparserutils-0 libwapcaplet-0 --cflags)
  LDFLAGS := $(LDFLAGS) $(shell $(PKGCONFIG) libparserutils-0 libwapcaplet-0 --libs)
else
  CFLAGS := $(CFLAGS) -I$(PREFIX)/include/parserutils0 \
	-I$(PREFIX)/include/wapcaplet0
  LDFLAGS := $(LDFLAGS) -lparserutils0 -lwapcaplet0
endif

include build/makefiles/Makefile.top

# Extra installation rules
I := /include/libcss$(major-version)/libcss

INSTALL_ITEMS := $(INSTALL_ITEMS) $(I):include/libcss/computed.h
INSTALL_ITEMS := $(INSTALL_ITEMS) $(I):include/libcss/errors.h
INSTALL_ITEMS := $(INSTALL_ITEMS) $(I):include/libcss/fpmath.h
INSTALL_ITEMS := $(INSTALL_ITEMS) $(I):include/libcss/functypes.h
INSTALL_ITEMS := $(INSTALL_ITEMS) $(I):include/libcss/hint.h
INSTALL_ITEMS := $(INSTALL_ITEMS) $(I):include/libcss/libcss.h
INSTALL_ITEMS := $(INSTALL_ITEMS) $(I):include/libcss/properties.h
INSTALL_ITEMS := $(INSTALL_ITEMS) $(I):include/libcss/select.h
INSTALL_ITEMS := $(INSTALL_ITEMS) $(I):include/libcss/stylesheet.h
INSTALL_ITEMS := $(INSTALL_ITEMS) $(I):include/libcss/types.h
INSTALL_ITEMS := $(INSTALL_ITEMS) /lib/pkgconfig:lib$(COMPONENT).pc.in
INSTALL_ITEMS := $(INSTALL_ITEMS) /lib:$(OUTPUT)
