#  Copyright 2020 The casbin Authors. All Rights Reserved.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

cmake_minimum_required(VERSION 3.19)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake/modules
)

set(CMAKE_WARN_DEPRECATED ON)

###############################################################################
# Project definition.

project(
    cvaluate
    VERSION 0.0.0
    DESCRIPTION "Expression evaluate in C/C++"
    LANGUAGES CXX C
)


###############################################################################
# Forbid in-source build.

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR 
            "In-source build not allowed. Please make a new sub-directory and run CMake from there.")
endif()

###############################################################################
# Global CMake options.

option(CVALUATE_BUILD_TEST "State whether to build test" ON)

# Intrinsic directory paths
set(CVALUATE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cvaluate)
set(CVALUATE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CVALUATE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/cvaluate)

# Do not output install messages.
if(NOT DEFINED CMAKE_INSTALL_MESSAGE)
    set(CMAKE_INSTALL_MESSAGE "LAZY")
endif()

# Change the path max size to avoid problem on Windows.
if(NOT DEFINED CMAKE_OBJECT_PATH_MAX)
    set(CMAKE_OBJECT_PATH_MAX 300)
endif()

# Setting to C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

###############################################################################
# Install external dependencies
# Some required targets may be created by third-party CMake configs, which 
# don't generally produce global targets. To guarantee all imported targets are 
# global, this module is included at the project root level.

include(FindExtPackages)

add_subdirectory(cvaluate)

if(CVALUATE_BUILD_TEST)
    enable_testing()
    add_subdirectory(tests)
endif()
