############################################################
##
## rckam
## Copyright (c) 2020-2021 Come Raczy
## All rights reserved.
##
## This software is provided under the terms and conditions of the
## GNU AFFERO GENERAL PUBLIC LICENSE
##
## You should have received a copy of the
## GNU AFFERO GENERAL PUBLIC LICENSE
## along with this program. If not, see
## <https://fsf.org/>
##
############################################################
##
## To configure the build, see config.mk
##
############################################################

include config.mk

all: $(programs:%=$(BUILD)/%)

.PHONY: clean
clean:
	$(RMDIR) $(RCKAM_BUILD_DIR_BASE)

.PHONY: help
clean:
help: $(RCKAM_ROOT_DIR)/tracker/README.md
	cat $<

############################################################
##
## Rules and includes for the actual build as needed.
## empty MAKECMDGOALS defaults to "all". Inclusion must happen if any goal is not in "clean help"
##
############################################################
ifneq ($(filter-out clean help, $(or $(MAKECMDGOALS), all)),)

# Dependencies are initially generated with ".Td" extension to avoid issues if compiling fails afterwards
# and a POSTCOMPILE operation is needed to rename the file with the final ".d" extension
.PRECIOUS: %.d
DEPFLAGS = -MT $@ -MMD -MP -MF $(@:%.o=%.Td)
POSTCOMPILE ?= mv -f $(@:%.o=%.Td) $(@:%.o=%.d)
%.d: ;

# use a .sentinel file as a proxy to directories to avoid time stamp galore
.PRECIOUS: %/.sentinel
%/.sentinel:
	@mkdir -p $* && touch $@

include $(RCKAM_MAKE_DIR)/libs.mk

programs_aux:=$(programs)
include $(foreach program, $(programs), $(RCKAM_MAKE_DIR)/program.mk)

include $(RCKAM_MAKE_DIR)/install.mk
endif

############################################################
##
## Tracing make variables
## Only add these targets if the goal is to print as it adds
## spurious targets for all non-print goals specified on the command line
##
############################################################
ifneq (,$(filter print-%, $(MAKECMDGOALS)))
print-%: ; @$(error $* is $($*) (from $(origin $*)))
$(filter-out print-%, $(MAKECMDGOALS)): $(filter print-%, $(MAKECMDGOALS))
endif

