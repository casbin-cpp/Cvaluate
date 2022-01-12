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

if (CVALUATE_BUILD_TEST)
    set(CMAKE_CXX_STANDARD 17)

    set(CVALUATE_TEST_SOURCE
        evaluation_test.cpp
        parsing_test.cpp
    )

    add_executable(cvaluate_test ${CVALUATE_TEST_SOURCE} ${CVALUATE_TEST_HEADER})

    if(UNIX)
        set_target_properties(cvaluate_test PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        )
    endif()

    target_include_directories(cvaluate_test PUBLIC ${CVALUATE_INCLUDE_DIR})

    target_link_libraries(
        cvaluate_test
        PRIVATE
        gtest_main
        cvaluate
        nlohmann_json::nlohmann_json
    )

    include(GoogleTest)
    gtest_discover_tests(cvaluate_test)

endif()