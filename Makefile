# Component settings
COMPONENT := css
# Default to a static library
COMPONENT_TYPE ?= lib-static

# Setup the tooling
include build/makefiles/Makefile.tools

TESTRUNNER := $(PERL) build/testtools/testrunner.pl

# Toolchain flags
WARNFLAGS := -Wall -Wextra -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes \
	-Wmissing-declarations -Wnested-externs -Werror -pedantic
CFLAGS := $(CFLAGS) -std=c99 -D_BSD_SOURCE -I$(CURDIR)/include/ \
	-I$(CURDIR)/src $(WARNFLAGS) 

# Parserutils & wapcaplet
ifneq ($(PKGCONFIG),)
  CFLAGS := $(CFLAGS) $(shell $(PKGCONFIG) libparserutils libwapcaplet --cflags)
  LDFLAGS := $(LDFLAGS) $(shell $(PKGCONFIG) libparserutils libwapcaplet --libs)
else
  LDFLAGS := $(LDFLAGS) -lparserutils -lwapcaplet
endif

include build/makefiles/Makefile.top

# Extra installation rules
INSTALL_ITEMS := $(INSTALL_ITEMS) /include/libcss:include/libcss/computed.h
INSTALL_ITEMS := $(INSTALL_ITEMS) /include/libcss:include/libcss/errors.h
INSTALL_ITEMS := $(INSTALL_ITEMS) /include/libcss:include/libcss/fpmath.h
INSTALL_ITEMS := $(INSTALL_ITEMS) /include/libcss:include/libcss/functypes.h
INSTALL_ITEMS := $(INSTALL_ITEMS) /include/libcss:include/libcss/hint.h
INSTALL_ITEMS := $(INSTALL_ITEMS) /include/libcss:include/libcss/libcss.h
INSTALL_ITEMS := $(INSTALL_ITEMS) /include/libcss:include/libcss/properties.h
INSTALL_ITEMS := $(INSTALL_ITEMS) /include/libcss:include/libcss/select.h
INSTALL_ITEMS := $(INSTALL_ITEMS) /include/libcss:include/libcss/stylesheet.h
INSTALL_ITEMS := $(INSTALL_ITEMS) /include/libcss:include/libcss/types.h
INSTALL_ITEMS := $(INSTALL_ITEMS) /lib/pkgconfig:lib$(COMPONENT).pc.in
INSTALL_ITEMS := $(INSTALL_ITEMS) /lib:$(BUILDDIR)/lib$(COMPONENT)$(LIBEXT)
