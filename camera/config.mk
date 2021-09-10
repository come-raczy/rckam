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

RCKAM_VERSION:=0.2021.08.23

############################################################
##
## Operating system
##
############################################################

SHELL:=/bin/bash
UNAME_STRING:=$(shell uname -a)
OS?=$(or \
    $(findstring el7,$(UNAME_STRING)),\
#    $(findstring el8,$(UNAME_STRING)),\
#    $(findstring Ubuntu,$(UNAME_STRING)),\
    )
ifndef OS
$(error Unsupported Operating System: $(UNAME_STRING))
endif

############################################################
##
## Compiler and other tools
##
############################################################

CC=gcc
CXX=g++
AR=ar

############################################################
##
## Boost Libraries
## Should work with vanilla Boost versions for CentOs 7 (1.53),
## CentOs 8 (1.66), Ubuntu 16.04.6 LTS (1.58)
##
############################################################

ifneq (,$(BOOST_ROOT))
BOOST_INCLUDEDIR?=$(BOOST_ROOT)/include
BOOST_LIBRARYDIR?=$(BOOST_ROOT)/lib
endif # ifneq (,$(BOOST_ROOT))

BOOST_LIBRARIES := system filesystem date_time thread iostreams regex program_options

############################################################
##
## Gtest
## Should work with vanilla Gtest version for CentOs 7 (1.53),
## CentOs 8 (1.66), Ubuntu 16.04.6 LTS (1.58)
##
############################################################

HAS_GTEST?=1

ifneq (,$(GTEST_ROOT))
GTEST_INCLUDEDIR?=$(GTEST_ROOT)/include
GTEST_LIBRARYDIR?=$(GTEST_ROOT)/lib
endif # ifneq(,$(GTEST_ROOT))

############################################################
##
## Tools
##
############################################################

SILENT?=$(if $(VERBOSE),,@)
CAT?=cat
ECHO?=echo
EVAL?=eval
RMDIR?=rm -rf
MV?=mv

############################################################
##
## rckam directory structure and source files
##
############################################################

## Root defaults to the parent directory of the makefile
#RCKAM_ROOT_DIR?=$(realpath $(dir $(dir $(filter %Makefile, $(MAKEFILE_LIST)))))
RCKAM_MAKEFILE_DIR?=$(dir $(realpath $(filter %Makefile, $(MAKEFILE_LIST))))
ifeq (,$(RCKAM_MAKEFILE_DIR))
$(error Failed to infer RCKAM_MAKEFILE_DIR from MAKEFILE_LIST: $(MAKEFILE_LIST))
endif
RCKAM_ROOT_DIR?=$(realpath $(RCKAM_MAKEFILE_DIR)/..)
ifeq (,$(RCKAM_ROOT_DIR))
$(error Failed to infer RCKAM_ROOT_DIR from MAKEFILE_LIST: $(MAKEFILE_LIST))
endif

RCKAM_CAMERA_DIR?=$(RCKAM_ROOT_DIR)/camera
RCKAM_COMMON_DIR?=$(RCKAM_ROOT_DIR)/common
RCKAM_MAKE_DIR?=$(RCKAM_CAMERA_DIR)
RCKAM_TEST_DIR?=$(RCKAM_ROOT_DIR)/tests
RCKAM_BUILD_DIR_BASE?=$(RCKAM_CAMERA_DIR)/build

ifdef DEBUG
BUILD_TYPE=debug
else
BUILD_TYPE=release
endif
# TODO: add support for differentiating by toolset
RCKAM_BUILD_DIR?=$(RCKAM_BUILD_DIR_BASE)/$(BUILD_TYPE)
BUILD:=$(RCKAM_BUILD_DIR)

BUILD_COMMON:=$(BUILD)/common
RCKAM_COMMON_LIB=$(BUILD)/common.a

############################################################
##
## Compilation and linking flags
##
############################################################

# version must be tagged in the git repo
VERSION_STRING?=$(shell git describe --tags --always --abbrev=8 2> /dev/null || echo "UNKNOWN")

CPPFLAGS?=-Wall -ggdb3
CPPFLAGS += -DRCKAM_VERSION="$(RCKAM_VERSION)" 
CPPFLAGS += -DVERSION_STRING="$(VERSION_STRING)"
CPPFLAGS += -I $(RCKAM_CAMERA_DIR)
CPPFLAGS += -I $(RCKAM_ROOT_DIR)
CXXFLAGS?=-std=c++17
CFLAGS?=-std=c99 

LDFLAGS?=

# link the common library
LDFLAGS+= $(RCKAM_COMMON_LIB)


ifneq (,$(BOOST_LIBRARYDIR))
LDFLAGS += -L $(BOOST_LIBRARYDIR)
endif
LDFLAGS += $(BOOST_LIBRARIES:%=-lboost_%)

ifdef DEBUG
CPPFLAGS += -O1 -ggdb3 -femit-class-debug-always -fno-omit-frame-pointer
ifeq ($(DEBUG),glibc)
ifeq (,$(BOOST_LIBRARYDIR))
$(error BOOST_LIBRARYDIR is not set. make sure that LD_LIBRARY_PATH and BOOST_LIBRARYDIR point to boost built with _GLIBCXX_DEBUG)
endif #ifeq (,$(BOOST_LIBRARYDIR))
CPPFLAGS += -D_GLIBCXX_DEBUG
endif #ifeq ($(DEBUG),glibc)
ifdef ASAN
CPPFLAGS += -fsanitize=address
ifeq ($(ASAN),all)
CPPFLAGS += -fsanitize=leak -fsanitize=undefined # Not supported by g++ 4.8
endif # ifeq ($(ASAN,all)
endif # ASAN 
#CPPFLAGS += -pg -fstack-usage -fprofile-arcs -ftest-coverage
LDFLAGS += -lgcov -Wl,--exclude-libs=ALL
else # non DEBUG
#CPPFLAGS += -O3 -march=native # Not particularly great
#CPPFLAGS += -O3 -march=skylake-avx512 # same as above

CPPFLAGS += -Wfatal-errors -g -O2 -ftree-vectorize -finline-functions -fpredictive-commoning -fgcse-after-reload -funswitch-loops -ftree-slp-vectorize -fvect-cost-model -fipa-cp-clone -ftree-phiprop

# this seems slightly slower than above
#CXXFLAGS += -g -mavx2 -O2 -ftree-vectorize -finline-functions -fpredictive-commoning -fgcse-after-reload -funswitch-loops -ftree-slp-vectorize -fvect-cost-model -fipa-cp-clone -ftree-phiprop
endif # if DEBUG

LDFLAGS+= -lz -lstdc++ -lrt -lgomp -lpthread
LDFLAGS+= -lpigpio

ifneq (,$(GTEST_INCLUDEDIR))
GTEST_CPPFLAGS+= -I $(GTEST_INCLUDEDIR)
endif

ifneq (,$(GTEST_LIBRARYDIR))
GTEST_LDFLAGS+= -L $(GTEST_LIBRARYDIR)
endif # ifneq (,$(GTEST_LIBRARYDIR))

GTEST_LDFLAGS+= -lgtest_main -lgtest

LDFLAGS+= -lgphoto2 -lgphoto2_port

############################################################
##
## Basic verification
##
############################################################

ifneq (,$(BOOST_INCLUDEDIR))
ifeq ($(wildcard $(BOOST_INCLUDEDIR)),)
$(error BOOST_INCLUDEDIR: $(BOOST_INCLUDEDIR): directory not found: make sure that the environment variables BOOST_ROOT or BOOST_INCLUDEDIR are correctly set)
endif
endif

############################################################
##
## Structuring the source
##
############################################################

sources := $(wildcard $(RCKAM_CAMERA_DIR)/*.cpp)
#programs := $(sources:$(RCKAM_CAMERA_DIR)/%.cpp=%)
programs := rckam-camera
all_lib_sources := $(filter-out $(programs:%=$(RCKAM_CAMERA_DIR)/%.cpp), $(sources))
all_lib_objects := $(all_lib_sources:$(RCKAM_CAMERA_DIR)/%.cpp=$(BUILD)/%.o)

all_common_sources := $(wildcard $(RCKAM_COMMON_DIR)/*.cpp)
all_common_objects := $(all_common_sources:$(RCKAM_COMMON_DIR)/%.cpp=$(BUILD_COMMON)/%.o)
