#  Copyright 2021 The casbin Authors. All Rights Reserved.
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

set(CVALUATE_SOURCE_FILES
    EvaluableExpression.cpp
    Parsing.cpp
    Token.cpp
    pch.cpp
)

# Setting to C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

add_library(cvaluate STATIC ${CVALUATE_SOURCE_FILES})

target_precompile_headers(cvaluate PRIVATE pch.h)
target_include_directories(cvaluate PRIVATE ${CVALUATE_SOURCE_DIR})
target_link_libraries(
    cvaluate PRIVATE 
    # nlohmann_json::nlohmann_json
)

set_target_properties(cvaluate PROPERTIES 
    PREFIX ""
    VERSION ${CMAKE_PROJECT_VERSION}
)

if(WIN32 OR MSVC)
    set_target_properties(cvaluate PROPERTIES SUFFIX ".lib")
elseif(UNIX)
    set_target_properties(cvaluate PROPERTIES 
        SUFFIX ".a"
        POSITION_INDEPENDENT_CODE ON
    )
endif()
