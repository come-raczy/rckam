set(rckam_CXX_CONFIG_H_DIR ${CMAKE_CURRENT_BINARY_DIR}/include/common)
configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/common/config.h.in" "${rckam_CXX_CONFIG_H_DIR}/config.h")
include_directories (${rckam_CXX_CONFIG_H_DIR})
include_directories (${CMAKE_CURRENT_SOURCE_DIR})

set (rckam_ALL_LIBRARIES
    common
    options
    )

if    (rckam_FORCE_STATIC_LINK)
    message(STATUS "All libraries will be statically linked")
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "-static")
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-static")
    # ensure that even if cmake decides to allow for dynamic libs resolution,
    # this gets overriden into static...
    set(CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS ${CMAKE_EXE_LINK_STATIC_CXX_FLAGS})
    set(rckam_LIBRARY_PREFIX ${CMAKE_STATIC_LIBRARY_PREFIX})
    set(rckam_LIBRARY_SUFFIX ${CMAKE_STATIC_LIBRARY_SUFFIX})
else  (rckam_FORCE_STATIC_LINK)
    set(rckam_LIBRARY_PREFIX "")
    set(rckam_LIBRARY_SUFFIX "")
endif (rckam_FORCE_STATIC_LINK)

#rckam_find_boost(${rckam_BOOST_VERSION_MIN} "${rckam_BOOST_COMPONENTS}")
find_package(Boost 1.67 REQUIRED COMPONENTS ${rckam_BOOST_COMPONENTS})
rckam_find_library(ZLIB zlib.h z)
rckam_find_library(PTHREAD pthread.h pthread)
rckam_find_library(GPHOTO2 gphoto2/gphoto2.h gphoto2)
#find_package( GPHOTO2 REQUIRED )


set (rckam_AVAILABLE_LIBRARIES "")
set (rckam_ALL_LIBRARY_DIRS "")
add_custom_target(build_libs)
set (PREVIOUS_TARGET build_libs)
foreach (rckam_LIB_DIR ${rckam_ALL_LIBRARIES})
    add_subdirectory(${rckam_LIB_DIR})
    add_dependencies(rckam_${rckam_LIB_DIR} ${PREVIOUS_TARGET})
    set(rckam_AVAILABLE_LIBRARIES rckam_${rckam_LIB_DIR} ${rckam_AVAILABLE_LIBRARIES} )
    set (PREVIOUS_TARGET rckam_${rckam_LIB_DIR})
endforeach (rckam_LIB_DIR)

#set (rckam_AVAILABLE_LIBRARIES ${rckam_AVAILABLE_LIBRARIES} PARENT_SCOPE)

file(GLOB rckam_EXECUTABLE_SOURCES *.cpp)

foreach (SOURCE_FILE ${rckam_EXECUTABLE_SOURCES})
    get_filename_component (EXECUTABLE_NAME ${SOURCE_FILE} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILE})
    target_link_libraries (${EXECUTABLE_NAME} ${rckam_AVAILABLE_LIBRARIES} ${Boost_LIBRARIES} ${PTHREAD_LIBRARY})
endforeach (SOURCE_FILE)

