# Toolchain definitions for building on the destination platform
CC := gcc
AR := ar
LD := gcc

CP := cp
RM := rm
MKDIR := mkdir
MV := mv
ECHO := echo
MAKE := make
PERL := perl
PKGCONFIG := pkg-config
INSTALL := install
SED := sed
LCOV := lcov
GENHTML := genhtml

# Toolchain flags
WARNFLAGS := -Wall -Wextra -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes \
	-Wmissing-declarations -Wnested-externs -Werror -pedantic
CFLAGS += -std=c99 -D_BSD_SOURCE -I$(TOP)/include/ $(WARNFLAGS) \
	`$(PKGCONFIG) --cflags libparserutils`
RELEASECFLAGS = $(CFLAGS) -DNDEBUG -O2
DEBUGCFLAGS = $(CFLAGS) -O0 -g
ARFLAGS := -cru
LDFLAGS += `$(PKGCONFIG) --libs libparserutils` -L$(TOP)/

CPFLAGS :=
RMFLAGS := -f
MKDIRFLAGS := -p
MVFLAGS :=
ECHOFLAGS := 
MAKEFLAGS :=
PKGCONFIGFLAGS :=

EXEEXT :=

# Default installation prefix
PREFIX ?= /usr/local


include build/Makefile.common
