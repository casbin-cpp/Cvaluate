/*
* Copyright 2020 The casbin Authors. All Rights Reserved.
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
*/
#include <EvaluationStage.h>
namespace Cvaluate {
    TokenAvaiableValue NoopStageRight(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        return right;
    }
    
    bool IsString(TokenAvaiableValue value) {
        if (auto v = std::get_if<std::string>(&(value))) {
            return true;
        }
        return false;
    }

    bool IsBool(TokenAvaiableValue value) {
        if (auto v = std::get_if<bool>(&(value))) {
            return true;
        }
        return false;
    }

    bool IsNumeric(TokenAvaiableValue value) {
        if (auto v = std::get_if<int>(&(value))) {
            return true;
        }
        if (auto v = std::get_if<float>(&(value))) {
            return true;
        }
        return false;
    }

    bool IsArray(TokenAvaiableValue value) {
        if (auto v = std::get_if<std::vector<std::string>>(&(value))) {
            return true;
        }
        return false;
    }

    bool AdditionTypeCheck(TokenAvaiableValue left, TokenAvaiableValue right) {
        if (IsNumeric(left) && IsNumeric(right)) {
            return true;
        }

        if (!IsString(left) && !IsString(right)) {
            return false;
        }
        
        return true;
    }

    bool ComparatorTypeCheck(TokenAvaiableValue left, TokenAvaiableValue right) {
        if (IsNumeric(left) && IsNumeric(right)) {
            return true;
        }

        if (IsString(left) && IsString(right)) {
            return true;
        }

        return false;
    }


    bool IsRegexOrString(TokenAvaiableValue value) {
        return IsString(value);
    }
} // Cvaluate