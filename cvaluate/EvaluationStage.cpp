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
#include <nlohmann/json.hpp>

namespace Cvaluate {
    TokenAvaiableData AddStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
        if (IsString(left) || IsString(right)) {
            return GetTokenValueString(left) + GetTokenValueString(right);
        }
        
        return GetTokenValueNumeric(left) + GetTokenValueNumeric(right);
    }

    TokenAvaiableData SubtractStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
        return GetTokenValueNumeric(left) - GetTokenValueNumeric(right); 
    }

    TokenAvaiableData MultiplyStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
        return GetTokenValueNumeric(left) * GetTokenValueNumeric(right);
    }

    TokenAvaiableData DivideStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
        return GetTokenValueNumeric(left) / GetTokenValueNumeric(right);
    }

    TokenAvaiableData ExponentStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
        return (float)pow(GetTokenValueNumeric(left), GetTokenValueNumeric(right));
    }

    TokenAvaiableData ModulusStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
        return (float)((int)GetTokenValueNumeric(left) % (int)GetTokenValueNumeric(right));
    }

    TokenAvaiableData GteStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
        if (IsString(left) && IsString(right)) {
            return GetTokenValueString(left) >= GetTokenValueString(right);
        }
        return GetTokenValueNumeric(left) >= GetTokenValueNumeric(right);
    }

    TokenAvaiableData GtStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
        if (IsString(left) && IsString(right)) {
            return GetTokenValueString(left) > GetTokenValueString(right);
        }
        return GetTokenValueNumeric(left) > GetTokenValueNumeric(right);
    }

    TokenAvaiableData LteStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
        if (IsString(left) && IsString(right)) {
            return GetTokenValueString(left) <= GetTokenValueString(right);
        }
        return GetTokenValueNumeric(left) <= GetTokenValueNumeric(right);
    }

    TokenAvaiableData LtStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
        if (IsString(left) && IsString(right)) {
            return GetTokenValueString(left) < GetTokenValueString(right);
        }
        return GetTokenValueNumeric(left) < GetTokenValueNumeric(right);
    }

    TokenAvaiableData EqualStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
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

    TokenAvaiableData NotEqualStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
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

    TokenAvaiableData AndStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
        return GetTokenValueBool(left) && GetTokenValueBool(right);
    }

    TokenAvaiableData OrStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
        return GetTokenValueBool(left) || GetTokenValueBool(right);
    }

    TokenAvaiableData NegateStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
        return -GetTokenValueNumeric(right);
    }

    TokenAvaiableData InvertStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
        return !GetTokenValueBool(right);
    }

    TokenAvaiableData BitwiseNotStage(TokenAvaiableData, TokenAvaiableData, Parameters) {
        // throw CvaluateException("BitwiseNotStage Not Implement");
    }

    TokenAvaiableData TernaryIfStage(TokenAvaiableData, TokenAvaiableData, Parameters) {
        // throw CvaluateException("TernaryIfStage Not Implement");
    }

    TokenAvaiableData TernaryElseStage(TokenAvaiableData, TokenAvaiableData, Parameters)  {
        // throw CvaluateException("TernaryElseStage Not Implement");
    }

    TokenAvaiableData RegexStage(TokenAvaiableData, TokenAvaiableData, Parameters) {
        // throw CvaluateException("RegexStage Not Implement");
    }

    TokenAvaiableData NotRegexStage(TokenAvaiableData, TokenAvaiableData, Parameters) {
        // throw CvaluateException("NotRegexStage Not Implement");
    }

    TokenAvaiableData BitwiseOrStage(TokenAvaiableData, TokenAvaiableData, Parameters) {
        // throw CvaluateException("BitwiseOrStage Not Implement");
    }

    TokenAvaiableData BitwiseAndStage(TokenAvaiableData, TokenAvaiableData, Parameters) {
        // throw CvaluateException("BitwiseAndStage Not Implement");
    }

    TokenAvaiableData BitwiseXORStage(TokenAvaiableData, TokenAvaiableData, Parameters) {
        // throw CvaluateException("BitwiseXORStage Not Implement");
    }

    TokenAvaiableData LeftShiftStage(TokenAvaiableData, TokenAvaiableData, Parameters) {
        // throw CvaluateException("LeftShiftStage Not Implement");
    }
    TokenAvaiableData RightShiftStage(TokenAvaiableData, TokenAvaiableData, Parameters) {
        // throw CvaluateException("RightShiftStage Not Implement");
    }

    TokenAvaiableData NoopStageRight(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
        return right;
    }
    
    TokenAvaiableData InStage(TokenAvaiableData, TokenAvaiableData, Parameters) {
        // throw CvaluateException("InStage Not Implement");
    }

    TokenAvaiableData SeparatorStage(TokenAvaiableData left, TokenAvaiableData right, Parameters) {
        TokenAvaiableData ans;
        if (left.is_object()) {
            ans = left;
            ans.push_back(right);
        } else {
            ans.push_back(left);
            ans.push_back(right);
        }

        return ans;
    }

    EvaluationOperator MakeParameterStage(std::string parameter_name) {
        // throw CvaluateException("MakeParameterStage Not Implement");
    }

    EvaluationOperator MakeLiteralStage(TokenAvaiableData value) {
        auto func = [] (TokenAvaiableData value, TokenAvaiableData, Parameters) -> TokenAvaiableData {
            return value;
        };

        auto ret = std::bind(func, value, std::placeholders::_2, std::placeholders::_3);

        return ret;
    }

    EvaluationOperator MakeFunctionStage(ExpressionFunction function) {
        auto func = [] (TokenAvaiableData, TokenAvaiableData right, Parameters, ExpressionFunction function) -> TokenAvaiableData {
            return function(right);
        };

        auto ret = std::bind(func, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, function);

        return ret;
    }

    bool IsString(TokenAvaiableData value) {
        return value.is_string();
    }

    bool IsBool(TokenAvaiableData value) {
        return value.is_boolean();
    }

    bool IsNumeric(TokenAvaiableData value) {
        return IsFloat(value) || IsInt(value);
    }

    bool IsFloat(TokenAvaiableData value) {
        return value.is_number_float();
    }

    bool IsInt(TokenAvaiableData value) {
        return value.is_number_integer();
    }

    bool IsArray(TokenAvaiableData value) {
        return value.is_object();
    }

    bool AdditionTypeCheck(TokenAvaiableData left, TokenAvaiableData right) {
        if (IsNumeric(left) && IsNumeric(right)) {
            return true;
        }

        if (!IsString(left) && !IsString(right)) {
            return false;
        }
        
        return true;
    }

    bool ComparatorTypeCheck(TokenAvaiableData left, TokenAvaiableData right) {
        if (IsNumeric(left) && IsNumeric(right)) {
            return true;
        }

        if (IsString(left) && IsString(right)) {
            return true;
        }

        return false;
    }


    bool IsRegexOrString(TokenAvaiableData value) {
        return IsString(value);
    }
} // Cvaluate