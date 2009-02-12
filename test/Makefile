# Child makefile fragment
#
# Toolchain is provided by top-level makefile
#
# Variables provided by top-level makefile
#
# COMPONENT		The name of the component
# EXPORT		The location of the export directory
# TOP			The location of the source tree root
# RELEASEDIR		The place to put release objects
# DEBUGDIR		The place to put debug objects
#
# do_include		Canned command sequence to include a child makefile
#
# Variables provided by parent makefile:
#
# DIR			The name of the directory we're in, relative to $(TOP)
#
# Variables we can manipulate:
#
# ITEMS_CLEAN		The list of items to remove for "make clean"
# ITEMS_DISTCLEAN	The list of items to remove for "make distclean"
# TARGET_TESTS		The list of target names to run for "make test"
#
# SOURCES		The list of sources to build for $(COMPONENT)
#
# Plus anything from the toolchain

# Push parent directory onto the directory stack
sp             := $(sp).x
dirstack_$(sp) := $(d)
d              := $(DIR)

# Extend toolchain settings
CFLAGS := $(CFLAGS) -I$(TOP)/src/ -I$(d)

# Tests
TESTS_$(d) := csdetect css21 lex lex-auto libcss number parse parse-auto \
		parse2-auto select-auto
TESTS_$(d) := $(TESTS_$(d))

# Items for top-level makefile to use
ITEMS_CLEAN := $(ITEMS_CLEAN) \
	$(addprefix $(d), $(addsuffix $(EXEEXT), $(TESTS_$(d)))) \
	$(addprefix $(d), $(addsuffix .gcda, $(TESTS_$(d)))) \
	$(addprefix $(d), $(addsuffix .gcno, $(TESTS_$(d)))) \
	$(addprefix $(d), $(addsuffix .d, $(TESTS_$(d))))
ITEMS_DISTCLEAN := $(ITEMS_DISTCLEAN) $(d)log

# Targets for top-level makefile to run
TARGET_TESTS := $(TARGET_TESTS) test_$(d)

# Now we get to hack around so that we know what directory we're in.
# $(d) no longer exists when running the commands for a target, so we can't
# simply use it verbatim. Assigning to a variable doesn't really help, as
# there's no guarantee that someone else hasn't overridden that variable.
# So, what we do is make the target depend on $(d), then pick it out of the
# dependency list when running commands. This isn't pretty, but is effective.
test_$(d): $(d) $(addprefix $(d), $(TESTS_$(d)))
	@$(PERL) $(TOP)/$<testrunner.pl $(TOP)/$< $(EXEEXT)

DEP_$(d) :=

define dep_test
DEP_$(d) += $(2)
$(2): $(1)
	@$$(RM) $$(RMFLAGS) $(2)
	@$$(CC) $$(DEBUGCFLAGS) -MM -MT '$(2) $(3)' -MF $(2) $(1)

endef

# Build rules for each test binary -- they all depend on the debug library
define compile_test
$(2): $$(TOP)/$$(COMPONENT)-debug.a $(1)
	@$$(ECHO) $$(ECHOFLAGS) "==> $(1)"
	@$$(CC) -c -g $$(DEBUGCFLAGS) -o $$@.o $(1)
	@$$(LD) -g -o $$@ $$@.o -lcss-debug $$(LDFLAGS)
	@$$(RM) $$(RMFLAGS) $$@.o

endef

$(eval $(foreach TEST,$(addprefix $(d), $(TESTS_$(d))), \
	$(call dep_test,$(addsuffix .c, $(TEST)),$(addsuffix .d, $(TEST)),$(TEST))))

ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
-include $(sort $(DEP_$(d)))
endif

$(eval $(foreach TEST,$(addprefix $(d), $(TESTS_$(d))), \
	$(call compile_test,$(addsuffix .c, $(TEST)),$(TEST))))

# Now include any children we may have
MAKE_INCLUDES := $(wildcard $(d)*/Makefile)
$(eval $(foreach INC, $(MAKE_INCLUDES), $(call do_include,$(INC))))

# Finally, pop off the directory stack
d  := $(dirstack_$(sp))
sp := $(basename $(sp))
