include(CheckIncludeFile)

#
# Macro to find libraries with given prefix and suffix.
#
macro(rckam_find_any_library name header library prefix suffix)
if    (NOT ${name}_INCLUDE_DIR)
    CHECK_INCLUDE_FILE(${header} ${name}_INCLUDE_DIR_FOUND)
    #else (NOT ${name}_INCLUDE_DIR)
    #CHECK_INCLUDE_FILE(${header} ${name}_INCLUDE_DIR_FOUND CMAKE_REQUIRED_INCLUDES=${${name}_INCLUDE_DIR})
endif (NOT ${name}_INCLUDE_DIR)
if    (${name}_INCLUDE_DIR_FOUND AND NOT ${name}_LIBRARY)
    message (STATUS "Checking library: ${prefix}${library}${suffix}")
    find_library(${name}_LIBRARY
                 NAMES "${prefix}${library}${suffix}"
                 HINTS ENV LIBRARY_PATH)
endif (${name}_INCLUDE_DIR_FOUND AND NOT ${name}_LIBRARY)
if(${name}_INCLUDE_DIR_FOUND AND ${name}_LIBRARY)
    set (HAVE_${name} ${${name}_LIBRARY})
    message (STATUS "Found ${name}  header: ${header}")
    message (STATUS "Found ${name} library: ${${name}_LIBRARY}")
endif(${name}_INCLUDE_DIR_FOUND AND ${name}_LIBRARY)
endmacro(rckam_find_any_library)

#
# Find static or dynamic library according to globally set preferences
#
macro(rckam_find_library name header library)
    rckam_find_any_library("${name}" "${header}" "${library}" "${rckam_LIBRARY_PREFIX}" "${rckam_LIBRARY_SUFFIX}")
endmacro(rckam_find_library)

#   
# Not only finds boost but also sets the variables so that
# it is being used for include and linking
# Also makes sure pthread is available for boost
#
macro(rckam_find_boost boost_version boost_components)

    find_package(Boost ${boost_version} REQUIRED ${boost_components})
    if (NOT Boost_FOUND)
        message(FATAL_ERROR "boost libraries are required to build the software")
    endif (NOT Boost_FOUND)

    include_directories(BEFORE SYSTEM ${Boost_INCLUDE_DIRS})

    set      (HAVE_LIBBOOST_DATE_TIME       ${Boost_DATE_TIME_FOUND})
    set      (HAVE_LIBBOOST_FILESYSTEM      ${Boost_FILESYSTEM_FOUND})
    set      (HAVE_LIBBOOST_IOSTREAMS       ${Boost_IOSTREAMS_FOUND})
    set      (HAVE_LIBBOOST_PROGRAM_OPTIONS ${Boost_PROGRAM_OPTIONS_FOUND})
    set      (HAVE_LIBBOOST_PYTHON          ${Boost_PYTHON_FOUND})
    set      (HAVE_LIBBOOST_REGEX           ${Boost_REGEX_FOUND})
    set      (HAVE_LIBBOOST_SERIALIZATION   ${Boost_SERIALIZATION_FOUND})
    set      (HAVE_LIBBOOST_SYSTEM          ${Boost_SYSTEM_FOUND})
endmacro(rckam_find_boost)

