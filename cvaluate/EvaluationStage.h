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

#ifndef EVALUATION_STAGE_SYMBOL
#define EVALUATION_STAGE_SYMBOL

#include "pch.h"
#include <Token.h>
#include <OperatorSymbol.h>

namespace Cvaluate {
    using Parameters = std::unordered_map<std::string, TokenAvaiableData>;
    // Parameters kEmptyParameters;
    using EvaluationOperator = std::function<TokenAvaiableData(TokenAvaiableData, TokenAvaiableData, Parameters)>;
    using StageTypeCheck = std::function<bool(TokenAvaiableData)>;
    using StageCombinedTypeCheck = std::function<bool(TokenAvaiableData, TokenAvaiableData)>;

    class EvaluationStage {
        public:
            OperatorSymbol symbol_;
            std::shared_ptr<EvaluationStage> left_stage_;
            std::shared_ptr<EvaluationStage> right_stage_;

            EvaluationOperator operator_;

            StageTypeCheck left_type_check_;
            StageTypeCheck right_type_check_;

            StageCombinedTypeCheck type_check_;
        public:
            EvaluationStage() = delete;
            EvaluationStage(OperatorSymbol symbol, std::shared_ptr<EvaluationStage> left_stage,
                std::shared_ptr<EvaluationStage> right_stage, EvaluationOperator Operator,
                StageTypeCheck left_type_check, StageTypeCheck right_type_check, StageCombinedTypeCheck type_check) :
                symbol_(symbol), left_stage_(left_stage), right_stage_(right_stage),
                operator_(Operator), left_type_check_(left_type_check), right_type_check_(right_type_check),
                type_check_(type_check) {};
            void SwapWith(EvaluationStage& other);
            void SetToNonStage(EvaluationStage& other);
            bool IsShortCircuitable();
    };

    bool IsString(TokenAvaiableData value);
    bool IsBool(TokenAvaiableData value);
    bool IsNumeric(TokenAvaiableData value);
    bool IsFloat(TokenAvaiableData value);
    bool IsInt(TokenAvaiableData value);
    bool IsArray(TokenAvaiableData value);
    bool IsRegexOrString(TokenAvaiableData value);
    bool AdditionTypeCheck(TokenAvaiableData left, TokenAvaiableData right);
    bool ComparatorTypeCheck(TokenAvaiableData left, TokenAvaiableData right);

    // Operator type
    TokenAvaiableData AddStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData SubtractStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData MultiplyStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData DivideStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData ExponentStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData ModulusStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData GteStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData GtStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData LteStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData LtStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData EqualStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData NotEqualStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData AndStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData OrStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData NegateStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData InvertStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData BitwiseNotStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData TernaryIfStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData TernaryElseStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData RegexStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData NotRegexStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData BitwiseOrStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData BitwiseAndStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData BitwiseXORStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData LeftShiftStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});
    TokenAvaiableData RightShiftStage(TokenAvaiableData, TokenAvaiableData, Parameters = {});

    TokenAvaiableData NoopStageRight(TokenAvaiableData, TokenAvaiableData, Parameters);
    TokenAvaiableData InStage(TokenAvaiableData, TokenAvaiableData, Parameters);
    TokenAvaiableData SeparatorStage(TokenAvaiableData, TokenAvaiableData, Parameters);

    EvaluationOperator MakeParameterStage(std::string parameter_name);
    EvaluationOperator MakeLiteralStage(TokenAvaiableData);
    EvaluationOperator MakeFunctionStage(ExpressionFunction);
    EvaluationOperator MakeAccessorStage(TokenAvaiableData);
} // Cvaluate


#endif