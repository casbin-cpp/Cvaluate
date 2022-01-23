/*
* Copyright 2021 The casbin Authors. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* This is a test file for benchmarking the performance of casbin::Model
*/

#include <cvaluate/cvaluate.h>
#include "../test_config.h"

#ifdef SINGLEPARSE
static void SingleParse(int round) {
    for(int i = 0; i < round; i++)
        Cvaluate::EvaluableExpression("1");
}
#endif

int main() {
#ifdef SINGLEPARSE
    SingleParse(1000);
#endif
}
