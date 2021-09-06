include(${rckam_CXX_LIBRARY_CMAKE})
include_directories(${LIBUSB_1_INCLUDE_DIR})
target_link_libraries (rckam_devices Qt5::Widgets Qt5::Quick)
