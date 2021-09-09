#!/bin/bash
################################################################################
##
## rckam: a Qt remote control for digital cameras
## Copyright (C) <year>  <name of author>
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU Affero General Public License as
## published by the Free Software Foundation, either version 3 of the
## License, or (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Affero General Public License for more details.
##
## You should have received a copy of the GNU Affero General Public License
## along with this program.  If not, see <https://www.gnu.org/licenses/>.
##
################################################################################
##
## file configure.sh
##
## Top level configuration file
##
## author Come Raczy
##
################################################################################

#set -x
set -o pipefail
shopt -s compat31 2>/dev/null

# Use gcc/g++ by default.
export CC=${CC:="gcc"}
export CXX=${CXX:="g++"}

# Display rckam configure usage
rckam_usage()
{
    cat <<EOF
Usage: $0 [options]
Options: [defaults in brackets after descriptions]
Configuration:
  --build-type                specify the build type for CMake (affects compiler
                              options). Allowed values are "", "Debug", "Release",
                              "RelWithDebInfo", and "MinSizeRel" [RelWithDebInfo]
  --help                      print this message
  --package-type=type         enables generation of deployment package target (make package)
                              Valid types are: rpm, deb, tgz and android
  --static                    forces library static linking
  --terse                     display less information (disables CMAKE_VERBOSE_MAKEFILE)
  --verbose                   display more information (enables CMAKE_VERBOSE_MAKEFILE)
  --version                   only print version information
  --with-cmake=CMAKE          specify the cmake executable [cmake]
  --with-unit-tests           allow unit testing during the build
  --without-unit-tests        prevent unit testing during the build (default)

Directory and file names:
  --prefix=PREFIX         install files in tree rooted at PREFIX
  --exec-prefix=EPREFIX   install binary files in tree rooted at EPREFIX
                          [PREFIX]
  --bindir=DIR            install executable in DIR [EPREFIX/bin]
  --libdir=DIR            install library files in DIR [EPREFIX/lib/rckam-${rckam_version_full}]
  --libexecdir=DIR        install library programs in DIR [EPREFIX/libexec/rckam-${rckam_version_full}]
  --includedir=DIR        install include files in DIR [PREFIX/include/rckam-${rckam_version_full}]
  --datadir=DATADIR       install data files in DIR [PREFIX/share/rckam-${rckam_version_full}]
  --docdir=DIR            install documentation in DIR [DATADIR/doc]
  --mandir=DIR            install man pages files in DIR/manN [PREFIX/man]
  --builddir=DIR          build rckam in DIR [./]

Some influential environment variables:
  BOOST_ROOT       location of the boost library
  BOOST_INCLUDEDIR location of the include directory of boost
  BOOST_LIBRARYDIR location of the lib directory of boost
  CC               C compiler command
  CFLAGS           C compiler flags
  LDFLAGS          linker flags, e.g. -L<lib dir> if you have libraries in a
                   nonstandard directory <lib dir>
  CXX              C++ compiler command
  CXXFLAGS         C++ compiler flags
  CMAKE_OPTIONS    CMake command line options (for CMAKE_BUILD_TYPE, use --build-type)
  
  LIBRARY_PATH        library search path for library detection
  C_INCLUDE_PATH      include search path for library detection
  CPATH               include search path for library detection
  CPLUS_INCLUDE_PATH  include search path for library detection

Use these variables to override the choices made by 'configure' or to help
it to find libraries and programs with nonstandard names/locations.

EOF
    exit 10
}

# Version number extraction function.
rckam_version_component()
{
    cat "${rckam_source_dir}/CMakeLists.txt" | sed -n "
/^set(rckam_VERSION_${1}/ {s/set(rckam_VERSION_${1} *\(\(SAAC\)\?[0-9]*\))/\1/;p;}
"
}

# Helper function to fix windows paths.
rckam_fix_directory_slashes ()
{
    case $1 in
        */) echo "$1" | sed 's/\\/\//g';;
        *) echo "$1/" | sed 's/\\/\//g';;
    esac
}

rckam_create_path ()
{
	mkdir -p "$1"
	(cd "$1" && pwd) || exit 2 
}

# Detect system and directory information.
rckam_system="`uname`"
# uname -p returns unknown on cygwin. uname -m returns x86_64 on both CentOS and Cygwin
rckam_processor="`uname -m`"
rckam_source_dir="`echo $0 | sed -n '/\//{s/\/[^\/]*$//;p;}'`"
rckam_source_dir="`(cd "${rckam_source_dir}";pwd)`"
rckam_redist_dir="${rckam_source_dir}/../redist"
rckam_bootstrap_dir="${rckam_source_dir}/cmake/bootstrap"
rckam_build_dir="`pwd`"
rckam_version_major="`rckam_version_component MAJOR`"
rckam_version_minor="`rckam_version_component MINOR`"
rckam_version_patch="`rckam_version_component PATCH`"
rckam_version="${rckam_version_major}.${rckam_version_minor}"
rckam_version_full="${rckam_version_major}.${rckam_version_minor}.${rckam_version_patch}"

# Determine whether this is OS X
if echo "${rckam_system}" | grep Darwin >/dev/null 2>&1; then
    rckam_system_darwin=true
else
    rckam_system_darwin=false
fi

# Parse arguments
rckam_build_type=RelWithDebInfo
rckam_cmake=cmake
rckam_cmake_generator="Unix Makefiles"
rckam_verbose=
rckam_parallel=1
for a in "$@"; do
    if echo $a | grep "^--prefix=" > /dev/null 2> /dev/null; then
        rckam_prefix_dir=`echo $a | sed "s/^--prefix=//"`
        rckam_prefix_dir=`rckam_fix_directory_slashes "${rckam_prefix_dir}"` || exit $?
    elif echo $a | grep "^--exec-prefix=" > /dev/null 2> /dev/null; then
        rckam_exec_prefix_dir=`echo $a | sed "s/^--exec-prefix=//"`
        rckam_exec_prefix_dir=`rckam_fix_directory_slashes "${rckam_exec_prefix_dir}"` || exit $?
    elif echo $a | grep "^--bindir=" > /dev/null 2> /dev/null; then
        rckam_bin_dir=`echo $a | sed "s/^--bindir=//"`
        rckam_bin_dir=`rckam_fix_directory_slashes "${rckam_bin_dir}"` || exit $?
    elif echo $a | grep "^--libdir=" > /dev/null 2> /dev/null; then
        rckam_lib_dir=`echo $a | sed "s/^--libdir=//"`
        rckam_lib_dir=`rckam_fix_directory_slashes "${rckam_lib_dir}"` || exit $?
    elif echo $a | grep "^--libexecdir=" > /dev/null 2> /dev/null; then
        rckam_libexec_dir=`echo $a | sed "s/^--libexecdir=//"`
        rckam_libexec_dir=`rckam_fix_directory_slashes "${rckam_libexec_dir}"` || exit $?
    elif echo $a | grep "^--includedir=" > /dev/null 2> /dev/null; then
        rckam_include_dir=`echo $a | sed "s/^--includedir=//"`
        rckam_include_dir=`rckam_fix_directory_slashes "${rckam_include_dir}"` || exit $?
    elif echo $a | grep "^--datadir=" > /dev/null 2> /dev/null; then
        rckam_data_dir=`echo $a | sed "s/^--datadir=//"`
        rckam_data_dir=`rckam_fix_directory_slashes "${rckam_data_dir}"` || exit $?
    elif echo $a | grep "^--docdir=" > /dev/null 2> /dev/null; then
        rckam_doc_dir=`echo $a | sed "s/^--docdir=//"`
        rckam_doc_dir=`rckam_fix_directory_slashes "${rckam_doc_dir}"` || exit $?
    elif echo $a | grep "^--mandir=" > /dev/null 2> /dev/null; then
        rckam_man_dir=`echo $a | sed "s/^--mandir=//"`
        rckam_man_dir=`rckam_fix_directory_slashes "${rckam_man_dir}"` || exit $?
    elif echo $a | grep "^--builddir=" > /dev/null 2> /dev/null; then
        rckam_build_dir=`echo $a | sed "s/^--builddir=//"`
        rckam_build_dir=`rckam_fix_directory_slashes "${rckam_build_dir}"` || exit $?
        rckam_build_dir=`rckam_create_path "${rckam_build_dir}"` || exit 2
    elif echo $a | grep "^--help" > /dev/null 2> /dev/null; then
        rckam_usage
    elif echo $a | grep "^--version" > /dev/null 2> /dev/null; then
        echo "rckam ${rckam_version_full}, Copyright (c) 2019 come Raczy <come.raczy@gmail.com>"
        exit 2
    elif echo $a | grep "^--with-cmake" > /dev/null 2> /dev/null; then
        rckam_cmake=`echo $a | sed "s/^--with-cmake=//"`
    elif echo $a | grep "^--build-type" > /dev/null 2> /dev/null; then
        rckam_build_type=`echo $a | sed "s/^--build-type=//"`
    elif echo $a | grep "^--with-unit-tests" > /dev/null 2> /dev/null; then
        CMAKE_OPTIONS="$CMAKE_OPTIONS -Drckam_UNIT_TESTS=TRUE"
    elif echo $a | grep "^--without-unit-tests" > /dev/null 2> /dev/null; then
        CMAKE_OPTIONS="$CMAKE_OPTIONS -Drckam_UNIT_TESTS=FALSE"
    elif echo $a | grep "^--verbose" > /dev/null 2> /dev/null; then
        CMAKE_OPTIONS="$CMAKE_OPTIONS -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DBoost_DEBUG:BOOL=ON"
        rckam_verbose=TRUE
    elif echo $a | grep "^--terse" > /dev/null 2> /dev/null; then
        CMAKE_OPTIONS="$CMAKE_OPTIONS -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF -DBoost_DEBUG:BOOL=OFF"
        rckam_verbose=
    elif echo $a | grep "^--static" > /dev/null 2> /dev/null; then
        rckam_static=TRUE
    elif echo $a | grep "^--package-type=rpm" > /dev/null 2> /dev/null; then
        rckam_package=RPM
    elif echo $a | grep "^--package-type=deb" > /dev/null 2> /dev/null; then
        rckam_package=DEB
    elif echo $a | grep "^--package-type=tgz" > /dev/null 2> /dev/null; then
        rckam_package=TGZ
    elif echo $a | grep "^--with-gprof" > /dev/null 2> /dev/null; then
        export CXXFLAGS="-pg -fprofile-arcs ${CXXFLAGS}"
    else
        echo "Unknown command line option: $a" >&2
        exit 2
    fi
    
done

# Set the build and install paths

# display information if required
if [ -n "${rckam_verbose}" ]; then
    echo "Source  directory:  ${rckam_source_dir}"
    echo "Prefix  directory:  ${rckam_prefix_dir}"
    echo "Exec    directory:  ${rckam_exec_prefix_dir}"
    echo "Binary  directory:  ${rckam_bin_dir}"
    echo "Lib     directory:  ${rckam_lib_dir}"
    echo "Libexec directory:  ${rckam_libexec_dir}"
    echo "Include directory:  ${rckam_include_dir}"
    echo "Data    directory:  ${rckam_data_dir}"
    echo "Doc     directory:  ${rckam_doc_dir}"
    echo "Man     directory:  ${rckam_man_dir}"
    echo "Build   directory:  ${rckam_build_dir}"
    echo "Cmake   executable: ${rckam_cmake}"
    echo "LIBRARY_PATH     :  ${LIBRARY_PATH}"
    echo "C_INCLUDE_PATH   :  ${C_INCLUDE_PATH}"
    echo "CPATH:              ${CPATH}"
    echo "CPLUS_INCLUDE_PATH: ${CPLUS_INCLUDE_PATH}"
    echo ""
fi  

# create the build directory if necessary
if [[ ! -d "${rckam_build_dir}" ]]; then 
    mkdir "${rckam_build_dir}"
    if [ "$?" != 0 ]; then
        echo "Couldn't create the build directory: ${rckam_build_dir}"
        exit 4
    fi
fi

# invoke cmake

CMAKE_OPTIONS="$CMAKE_OPTIONS -Drckam_PREFIX:STRING=${rckam_prefix_dir}"
CMAKE_OPTIONS="$CMAKE_OPTIONS -Drckam_EXEC_PREFIX:STRING=${rckam_exec_prefix_dir}"
CMAKE_OPTIONS="$CMAKE_OPTIONS -Drckam_BINDIR:PATH=${rckam_bin_dir}"
CMAKE_OPTIONS="$CMAKE_OPTIONS -Drckam_LIBDIR:PATH=${rckam_lib_dir}"
CMAKE_OPTIONS="$CMAKE_OPTIONS -Drckam_LIBEXECDIR:PATH=${rckam_libexec_dir}"
CMAKE_OPTIONS="$CMAKE_OPTIONS -Drckam_INCLUDEDIR:PATH=${rckam_include_dir}"
CMAKE_OPTIONS="$CMAKE_OPTIONS -Drckam_DATADIR:PATH=${rckam_data_dir}"
CMAKE_OPTIONS="$CMAKE_OPTIONS -Drckam_DOCDIR:PATH=${rckam_doc_dir}"
CMAKE_OPTIONS="$CMAKE_OPTIONS -Drckam_MANDIR:PATH=${rckam_man_dir}"
CMAKE_OPTIONS="$CMAKE_OPTIONS -DCMAKE_BUILD_TYPE:STRING=${rckam_build_type}"

if [ -n "${rckam_static}" ]; then
    CMAKE_OPTIONS="$CMAKE_OPTIONS -Drckam_FORCE_STATIC_LINK:BOOL=ON"
    CMAKE_OPTIONS="$CMAKE_OPTIONS -DLINK_SEARCH_END_STATIC:BOOL=ON"
fi

if [ "DEB" == "${rckam_package}" ]; then
    CMAKE_OPTIONS="${CMAKE_OPTIONS} \
        -DCPACK_GENERATOR:STRING=DEB \
        -DCPACK_SYSTEM_NAME:STRING=${rckam_system}-${rckam_processor} \
        -DCPACK_PACKAGE_CONTACT:STRING='come.raczy@@gmail.com' \
        -DCPACK_DEBIAN_PACKAGE_ARCHITECTURE:STRING='`dpkg --print-architecture`'"
	
    CMAKE_OPTIONS="$CMAKE_OPTIONS -DCPACK_DEBIAN_PACKAGE_DEPENDS:STRING='gnuplot(>=4.0),libxslt1.1(>=1.1),xsltproc(>=1.1)'"
    
elif [ "RPM" == "${rckam_package}" ]; then
    CMAKE_OPTIONS="${CMAKE_OPTIONS} \
        -DCPACK_GENERATOR:STRING=RPM \
        -DCPACK_SYSTEM_NAME:STRING=${rckam_system}-${rckam_processor} \
        -DCPACK_PACKAGE_CONTACT:STRING='come.raczy@gmail.com'"

    CMAKE_OPTIONS="$CMAKE_OPTIONS -DCPACK_RPM_PACKAGE_REQUIRES:STRING='gnuplot >= 4.0,libxslt'"
elif [ "TGZ" == "${rckam_package}" ]; then
    CMAKE_OPTIONS="${CMAKE_OPTIONS} \
        -DCPACK_SYSTEM_NAME:STRING=${rckam_system}-${rckam_processor} \
        -DCPACK_GENERATOR:STRING=TGZ"
fi

if [ -n "${rckam_verbose}" ]; then
    echo "Running on: `uname -a`"
    echo "Configuring the build directory with:"
    echo "    "${rckam_cmake} -H\"${rckam_source_dir}\" -B\"${rckam_build_dir}\" -G\"${rckam_cmake_generator}\" ${CMAKE_OPTIONS}
    echo ""
fi

eval "${rckam_cmake} -H'${rckam_source_dir}' -B'${rckam_build_dir}' -G'${rckam_cmake_generator}' ${CMAKE_OPTIONS}"

if [ "$?" != 0 ]; then
    echo "Couldn't configure the project:"
    echo ""
    echo "${rckam_cmake} -H\"${rckam_source_dir}\" -B\"${rckam_build_dir}\" -G\"${rckam_cmake_generator}\" ${CMAKE_OPTIONS}"
    echo ""
    echo "Moving CMakeCache.txt to CMakeCache.txt.removed"
    echo ""
    rm -f ${rckam_build_dir}/CMakeCache.txt.removed && mv ${rckam_build_dir}/CMakeCache.txt ${rckam_build_dir}/CMakeCache.txt.removed
    echo ""
    exit 5
fi

if [ "${rckam_build_dir}" == "${rckam_source_dir}" ]; then
    echo ""
    echo "Warning: it is recommended to build rckam outside of the source directory:"
    echo "    mkdir ../rckam-build"
    echo "    cd ../rckam-build"
    echo "    ../$(basename $(pwd))/configure --prefix=/path/to/install/dir"
    echo "    make"
    echo "    make install"
    echo ""
fi

echo "The build directory ${rckam_build_dir} was configured successfully"
echo ""
echo Type "make" at the top level of the root directory to build rckam
echo ""

