cmake_minimum_required (VERSION 2.8.11)

project (rckam)

set (rckam_STRING "rckam")
set (rckam_VERSION_MAJOR 00)
set (rckam_VERSION_MINOR 19)
set (rckam_VERSION_PATCH 06)
set (rckam_VERSION_DEVEL 21)
set (rckam_VERSION_FULL "${rckam_STRING}-${rckam_VERSION_MAJOR}.${rckam_VERSION_MINOR}.${rckam_VERSION_PATCH}.${rckam_VERSION_DEVEL}")

set (rckam_BOOST_VERSION_MIN 1.67.0)
set (rckam_BOOST_COMPONENTS date_time filesystem program_options system)

cmake_policy(SET CMP0046 NEW)
cmake_policy(SET CMP0071 NEW)

get_filename_component(rckam_ROOT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)
include ("${rckam_ROOT_DIR}/cmake/macros.cmake")
set (rckam_CXX_LIBRARY_CMAKE "${rckam_ROOT_DIR}/cmake/cxxLibrary.cmake")
set (rckam_CXX_EXECUTABLE_CMAKE "${rckam_ROOT_DIR}/cmake/cxxExecutable.cmake")

set(CMAKE_CXX_STANDARD 17)

set(rckam_COMMON_SOURCE_DIR "${rckam_ROOT_DIR}/common")
set(rckam_COMMON_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/common")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

#Create the global config.h
configure_file ("${rckam_COMMON_SOURCE_DIR}/config.h.in" "${rckam_COMMON_BUILD_DIR}/config.h")
get_filename_component(rckam_COMMON_SOURCE_PARENT_DIR "${rckam_COMMON_SOURCE_DIR}" DIRECTORY)
include_directories (${rckam_COMMON_SOURCE_PARENT_DIR}) # still want #include "common/SomeFile.hpp"
include_directories (${rckam_COMMON_BUILD_DIR})
include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-Wall -pedantic -Wextra)

if (rckam_FORCE_STATIC_LINK)
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

find_package(Boost 1.71 REQUIRED COMPONENTS ${rckam_BOOST_COMPONENTS})
rckam_find_library(ZLIB zlib.h z)
rckam_find_library(PTHREAD pthread.h pthread)
#rckam_find_library(GPHOTO2 gphoto2/gphoto2.h gphoto2)
#rckam_find_library(UDEV libudev.h udev)

# libusb1 needs a bit more help
#rckam_find_library(LIBUSB_1 libusb.h usb-1.0)
#find_path(LIBUSB_1_INCLUDE_DIR NAMES libusb.h PATH_SUFFIXES libusb-1.0)
#find_library(LIBUSB_1_LIBRARY NAMES usb-1.0 usb)
#message(STATUS "LIBUSB_1_INCLUDE_DIR: ${LIBUSB_1_INCLUDE_DIR}")
#message(STATUS "LIBUSB_1_LIBRARY: ${LIBUSB_1_LIBRARY}")


#find_package(Qt5 REQUIRED Core Widgets Qml Quick Gui Xml Svg)
#find_package(Qt5Widgets REQUIRED)
#find_package(Qt5QuickCompiler)
#find_package(Qt5QuickControls2 REQUIRED)

#qtquick_compiler_add_resources(qml_QRC qml/qml.qrc)

set (rckam_AVAILABLE_LIBRARIES "")
set (rckam_ALL_LIBRARY_DIRS "")
add_custom_target(build_libs)
set (PREVIOUS_TARGET build_libs)

foreach (rckam_LIB_DIR "common" "client") #${rckam_ALL_LIBRARIES})
  if (rckam_LIB_DIR STREQUAL "common")
    add_subdirectory("${rckam_COMMON_SOURCE_DIR}" "${rckam_COMMON_BUILD_DIR}")
  elseif (rckam_LIB_DIR STREQUAL "client")
   include(${rckam_CXX_LIBRARY_CMAKE})
   #target_link_libraries (rckam_client Qt5::Widgets Qt5::Quick Qt5::Core Qt5::Gui Qt5::QuickControls2) 
  endif()
  add_dependencies(rckam_${rckam_LIB_DIR} ${PREVIOUS_TARGET})
  set(rckam_AVAILABLE_LIBRARIES rckam_${rckam_LIB_DIR} ${rckam_AVAILABLE_LIBRARIES} )
  set (PREVIOUS_TARGET rckam_${rckam_LIB_DIR})
endforeach (rckam_LIB_DIR)

#include_directories(${LIBUSB_1_INCLUDE_DIR})

# file(GLOB rckam_EXECUTABLE_SOURCES *.cpp)
#foreach (SOURCE_FILE ${rckam_EXECUTABLE_SOURCES})
#    get_filename_component (EXECUTABLE_NAME ${SOURCE_FILE} NAME_WE)
#    add_executable(${EXECUTABLE_NAME} ${SOURCE} ${SOURCE_FILE} ${qml_QRC})
#    target_link_libraries (${EXECUTABLE_NAME} ${rckam_AVAILABLE_LIBRARIES} Qt5::Qml Qt5::Quick ${Boost_LIBRARIES} ${PTHREAD_LIBRARY} ${GPHOTO2_LIBRARY} ${LIBUSB_1_LIBRARY} ${UDEV_LIBRARY})
#endforeach (SOURCE_FILE)

set(rckam_CLIENT_SOURCE "rckam-client.cpp")
get_filename_component (EXECUTABLE_NAME ${rckam_CLIENT_SOURCE} NAME_WE)
add_executable(${EXECUTABLE_NAME} ${SOURCE} ${rckam_CLIENT_SOURCE} ${qml_QRC})
#target_link_libraries (${EXECUTABLE_NAME} ${rckam_AVAILABLE_LIBRARIES} Qt5::Qml Qt5::Quick ${Boost_LIBRARIES} ${PTHREAD_LIBRARY} ${GPHOTO2_LIBRARY} ${LIBUSB_1_LIBRARY} ${UDEV_LIBRARY})
target_link_libraries (${EXECUTABLE_NAME} ${rckam_AVAILABLE_LIBRARIES} ${Boost_LIBRARIES} ${PTHREAD_LIBRARY} )


