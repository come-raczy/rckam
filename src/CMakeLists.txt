set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#Create the global config.h
set(rckam_CXX_CONFIG_H_DIR ${CMAKE_CURRENT_BINARY_DIR}/include/common)
configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/common/config.h.in" "${rckam_CXX_CONFIG_H_DIR}/config.h")
include_directories (${rckam_CXX_CONFIG_H_DIR})
include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-Wall -pedantic -Wextra)

set (rckam_ALL_LIBRARIES
    common
    options
    devices
    models
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

find_package(Boost 1.67 REQUIRED COMPONENTS ${rckam_BOOST_COMPONENTS})
rckam_find_library(ZLIB zlib.h z)
rckam_find_library(PTHREAD pthread.h pthread)
rckam_find_library(GPHOTO2 gphoto2/gphoto2.h gphoto2)
rckam_find_library(UDEV libudev.h udev)

# libusb1 needs a bit more help
#rckam_find_library(LIBUSB_1 libusb.h usb-1.0)
find_path(LIBUSB_1_INCLUDE_DIR NAMES libusb.h PATH_SUFFIXES libusb-1.0)
find_library(LIBUSB_1_LIBRARY NAMES usb-1.0 usb)
message(STATUS "LIBUSB_1_INCLUDE_DIR: ${LIBUSB_1_INCLUDE_DIR}")
message(STATUS "LIBUSB_1_LIBRARY: ${LIBUSB_1_LIBRARY}")


find_package(Qt5 REQUIRED Core Widgets Qml Quick Gui Xml Svg)
find_package(Qt5QuickCompiler)

qtquick_compiler_add_resources(qml_QRC qml/qml.qrc)

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

include_directories(${LIBUSB_1_INCLUDE_DIR})

file(GLOB rckam_EXECUTABLE_SOURCES *.cpp)

foreach (SOURCE_FILE ${rckam_EXECUTABLE_SOURCES})
    get_filename_component (EXECUTABLE_NAME ${SOURCE_FILE} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${SOURCE} ${SOURCE_FILE} ${qml_QRC})
    target_link_libraries (${EXECUTABLE_NAME} ${rckam_AVAILABLE_LIBRARIES} Qt5::Qml Qt5::Quick ${Boost_LIBRARIES} ${PTHREAD_LIBRARY} ${GPHOTO2_LIBRARY} ${LIBUSB_1_LIBRARY} ${UDEV_LIBRARY})
endforeach (SOURCE_FILE)
