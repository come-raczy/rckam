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

$(BUILD)/$(program).o: $(RCKAM_SRC_DIR)/$(program).cpp $(BUILD)/$(program).d $(BUILD)/.sentinel
	$(CXX) $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
	$(POSTCOMPILE)

$(BUILD)/$(program): $(BUILD)/$(program).o $(libraries)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(libraries) $(LDFLAGS)

include $(wildcard $(BUILD)/$(program).d)

