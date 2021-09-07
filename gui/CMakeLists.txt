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

include ("${CMAKE_SOURCE_DIR}/cmake/macros.cmake")
set (rckam_CXX_LIBRARY_CMAKE "${CMAKE_SOURCE_DIR}/cmake/cxxLibrary.cmake")
set (rckam_CXX_EXECUTABLE_CMAKE "${CMAKE_SOURCE_DIR}/cmake/cxxExecutable.cmake")

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

