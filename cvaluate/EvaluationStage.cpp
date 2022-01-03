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
#include <Exception.h>

namespace Cvaluate {
    TokenAvaiableValue AddStage(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        if (IsString(left) || IsString(right)) {
            return GetTokenValueString(left) + GetTokenValueString(right);
        }
        
        return GetTokenValueNumeric(left) + GetTokenValueNumeric(right);
    }

    TokenAvaiableValue SubtractStage(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        return GetTokenValueNumeric(left) - GetTokenValueNumeric(right); 
    }

    TokenAvaiableValue MultiplyStage(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        return GetTokenValueNumeric(left) * GetTokenValueNumeric(right);
    }

    TokenAvaiableValue DivideStage(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        return GetTokenValueNumeric(left) / GetTokenValueNumeric(right);
    }

    TokenAvaiableValue ExponentStage(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        return (float)pow(GetTokenValueNumeric(left), GetTokenValueNumeric(right));
    }

    TokenAvaiableValue ModulusStage(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        return (float)((int)GetTokenValueNumeric(left) % (int)GetTokenValueNumeric(right));
    }

    TokenAvaiableValue GteStage(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        if (IsString(left) && IsString(right)) {
            return GetTokenValueString(left) >= GetTokenValueString(right);
        }
        return GetTokenValueNumeric(left) >= GetTokenValueNumeric(right);
    }

    TokenAvaiableValue GtStage(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        if (IsString(left) && IsString(right)) {
            return GetTokenValueString(left) > GetTokenValueString(right);
        }
        return GetTokenValueNumeric(left) > GetTokenValueNumeric(right);
    }

    TokenAvaiableValue LteStage(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        if (IsString(left) && IsString(right)) {
            return GetTokenValueString(left) <= GetTokenValueString(right);
        }
        return GetTokenValueNumeric(left) <= GetTokenValueNumeric(right);
    }

    TokenAvaiableValue LtStage(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        if (IsString(left) && IsString(right)) {
            return GetTokenValueString(left) < GetTokenValueString(right);
        }
        return GetTokenValueNumeric(left) < GetTokenValueNumeric(right);
    }

    TokenAvaiableValue EqualStage(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        if (IsString(left) && IsString(right)) {
            return GetTokenValueString(left) == GetTokenValueString(right);
        }

        if (IsNumeric(left) && IsNumeric(right)) {
            return GetTokenValueNumeric(left) == GetTokenValueNumeric(right);
        }

        if (IsBool(left) && IsBool(right)) {
            return GetTokenValueBool(left) == GetTokenValueBool(right);
        }

        return false;
    }

    TokenAvaiableValue NotEqualStage(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        if (IsString(left) && IsString(right)) {
            return GetTokenValueString(left) != GetTokenValueString(right);
        }

        if (IsNumeric(left) && IsNumeric(right)) {
            return GetTokenValueNumeric(left) != GetTokenValueNumeric(right);
        }

        if (IsBool(left) && IsBool(right)) {
            return GetTokenValueBool(left) != GetTokenValueBool(right);
        }

        return false;
    }

    TokenAvaiableValue AndStage(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        return GetTokenValueBool(left) && GetTokenValueBool(right);
    }

    TokenAvaiableValue OrStage(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        return GetTokenValueBool(left) || GetTokenValueBool(right);
    }

    TokenAvaiableValue NegateStage(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        return -GetTokenValueNumeric(right);
    }

    TokenAvaiableValue InvertStage(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        return !GetTokenValueBool(right);
    }

    TokenAvaiableValue BitwiseNotStage(TokenAvaiableValue, TokenAvaiableValue, Parameters) {
        // throw CvaluateException("BitwiseNotStage Not Implement");
    }

    TokenAvaiableValue TernaryIfStage(TokenAvaiableValue, TokenAvaiableValue, Parameters) {
        // throw CvaluateException("TernaryIfStage Not Implement");
    }

    TokenAvaiableValue TernaryElseStage(TokenAvaiableValue, TokenAvaiableValue, Parameters)  {
        // throw CvaluateException("TernaryElseStage Not Implement");
    }

    TokenAvaiableValue RegexStage(TokenAvaiableValue, TokenAvaiableValue, Parameters) {
        // throw CvaluateException("RegexStage Not Implement");
    }

    TokenAvaiableValue NotRegexStage(TokenAvaiableValue, TokenAvaiableValue, Parameters) {
        // throw CvaluateException("NotRegexStage Not Implement");
    }

    TokenAvaiableValue BitwiseOrStage(TokenAvaiableValue, TokenAvaiableValue, Parameters) {
        // throw CvaluateException("BitwiseOrStage Not Implement");
    }

    TokenAvaiableValue BitwiseAndStage(TokenAvaiableValue, TokenAvaiableValue, Parameters) {
        // throw CvaluateException("BitwiseAndStage Not Implement");
    }

    TokenAvaiableValue BitwiseXORStage(TokenAvaiableValue, TokenAvaiableValue, Parameters) {
        // throw CvaluateException("BitwiseXORStage Not Implement");
    }

    TokenAvaiableValue LeftShiftStage(TokenAvaiableValue, TokenAvaiableValue, Parameters) {
        // throw CvaluateException("LeftShiftStage Not Implement");
    }
    TokenAvaiableValue RightShiftStage(TokenAvaiableValue, TokenAvaiableValue, Parameters) {
        // throw CvaluateException("RightShiftStage Not Implement");
    }

    TokenAvaiableValue NoopStageRight(TokenAvaiableValue left, TokenAvaiableValue right, Parameters) {
        return right;
    }
    
    TokenAvaiableValue InStage(TokenAvaiableValue, TokenAvaiableValue, Parameters) {
        // throw CvaluateException("InStage Not Implement");
    }

    TokenAvaiableValue SeparatorStage(TokenAvaiableValue, TokenAvaiableValue, Parameters) {
        // throw CvaluateException("SeparatorStage Not Implement");
    }

    EvaluationOperator MakeParameterStage(std::string parameter_name) {
        // throw CvaluateException("MakeParameterStage Not Implement");
    }

    EvaluationOperator MakeLiteralStage(TokenAvaiableValue value) {
        auto func = [] (TokenAvaiableValue value, TokenAvaiableValue, Parameters) -> TokenAvaiableValue {
            return value;
        };

        auto ret = std::bind(func, value, std::placeholders::_2, std::placeholders::_3);

        return ret;
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

    bool IsFloat(TokenAvaiableValue value) {
        if (auto v = std::get_if<float>(&(value))) {
            return true;
        }
        return false;
    }

    bool IsInt(TokenAvaiableValue value) {
        if (auto v = std::get_if<int>(&(value))) {
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