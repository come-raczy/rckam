################################################################################
##
## file cxxLibrary.cmake
##
## CMake configuration file for all the c++ libraries
##
## author Come Raczy
##
################################################################################
include_directories (${rckam_CXX_CONFIG_H_DIR})

get_filename_component(rckam_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message (STATUS "Adding the c++    library subdirectory: ${rckam_LIB_DIR}")

##
## Some generators (VS) require all targets to be unique across the project.
## Therefore, a unique prefix is needed to create the target names which are
## shared across libraries
##

string(REGEX REPLACE ${CMAKE_SOURCE_DIR}/c[+][+]/ "" TMP1 ${CMAKE_CURRENT_SOURCE_DIR}/)
string(REGEX REPLACE "/" "_" rckam_UNIQUE_PREFIX ${TMP1})

message(STATUS "   ====  rckam_UNIQUE_PREFIX: ${rckam_UNIQUE_PREFIX}")
##
## build the library. C++ library files must start with a Capital letter
##
file(GLOB_RECURSE rckam_LIBRARY_SOURCES_WITH_CPPUNIT [A-Z]*.cpp *.c)
message(STATUS "   ====  rckam_LIBRARY_SOURCES_WITH_CPPUNIT: ${rckam_LIBRARY_SOURCES_WITH_CPPUNIT}")

foreach (SOURCE_FILE ${rckam_LIBRARY_SOURCES_WITH_CPPUNIT})
    string(REGEX MATCH "cppunit" CPPUNIT_MATCH ${SOURCE_FILE} )
    if (NOT CPPUNIT_MATCH)
        set(rckam_LIBRARY_SOURCES ${rckam_LIBRARY_SOURCES} ${SOURCE_FILE})
    endif (NOT CPPUNIT_MATCH)
endforeach (SOURCE_FILE)

foreach (SOURCE_FILE ${rckam_LIBRARY_SOURCES})
    get_filename_component(SOURCE_NAME ${SOURCE_FILE} NAME_WE)
    if (${SOURCE_NAME}_COMPILE_FLAGS)
        set_source_files_properties(${SOURCE_FILE} PROPERTIES COMPILE_FLAGS ${${SOURCE_NAME}_COMPILE_FLAGS})
    endif (${SOURCE_NAME}_COMPILE_FLAGS)
endforeach (SOURCE_FILE)

#include_directories (${rckam_COMMON_INCLUDE} )
add_library         (rckam_${rckam_LIB_DIR} STATIC ${rckam_LIBRARY_SOURCES})

##
## build the unit tests if any (this should be mandatory really)
##

if (HAVE_CPPUNIT AND rckam_UNIT_TESTS)
    find_path(${CMAKE_CURRENT_SOURCE_DIR}_CPPUNIT_DIR cppunit PATHS ${CMAKE_CURRENT_SOURCE_DIR} NO_DEFAULT_PATH)
    if (${CMAKE_CURRENT_SOURCE_DIR}_CPPUNIT_DIR)
        message (STATUS "Adding the cppunit subdirectory for ${rckam_LIB_DIR}")
        add_subdirectory (cppunit)
    endif (${CMAKE_CURRENT_SOURCE_DIR}_CPPUNIT_DIR)
endif(HAVE_CPPUNIT AND rckam_UNIT_TESTS)

