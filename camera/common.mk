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

# Build the common library

$(BUILD_COMMON)/%.o: $(RCKAM_COMMON_DIR)/%.cpp $(BUILD_COMMON)/%.d $(BUILD_COMMON)/.sentinel
	$(CXX) $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
	$(POSTCOMPILE)

$(warning $(RCKAM_COMMON_LIB): $(all_common_objects))

$(RCKAM_COMMON_LIB): $(all_common_objects)
	$(AR) -crs $@ $<


include $(foreach lib, $(all_common_objects), $(lib:%.o=%.d))

