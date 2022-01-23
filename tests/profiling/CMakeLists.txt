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

# add_definitions(-DCASBIN_PROJECT_DIR=${CMAKE_SOURCE_DIR})

set(CVALUATE_PROFILE_SOURCE
    main.cpp
)

add_definitions(-DSINGLEPARSE)

add_executable(cvaluate_profile ${CVALUATE_PROFILE_SOURCE})

target_include_directories(cvaluate_profile PUBLIC ${CVALUATE_INCLUDE_DIR})

if(UNIX)
    set_target_properties(cvaluate_profile PROPERTIES
      POSITION_INDEPENDENT_CODE ON
    )
endif()

target_link_libraries(
    cvaluate_profile
        PRIVATE
    benchmark
    cvaluate
    nlohmann_json::nlohmann_json
    pthread
)
