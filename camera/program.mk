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

ifeq (,$(programs_aux))
$(error No programs specified)
endif

program := $(word 1, $(programs_aux))

ifeq (1, $(words $(programs_aux)))
programs_aux:=
else
programs_aux:=$(wordlist 2, $(words $(programs_aux)), $(programs_aux))
endif

# Objects are all created with the same rule in libs.mk
# Only linking is required
$(BUILD)/$(program): $(BUILD)/$(program).o $(all_lib_objects)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(all_lib_objects) $(LDFLAGS)

include $(wildcard $(BUILD)/$(program).d)

