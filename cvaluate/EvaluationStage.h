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
    using Parameters = std::unordered_map<std::string, TokenAvaiableValue>;
    // Parameters kEmptyParameters;
    using EvaluationOperator = std::function<TokenAvaiableValue(TokenAvaiableValue, TokenAvaiableValue, Parameters)>;
    using StageTypeCheck = std::function<bool(TokenAvaiableValue)>;
    using StageCombinedTypeCheck = std::function<bool(TokenAvaiableValue, TokenAvaiableValue)>;

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

    bool IsString(TokenAvaiableValue value);
    bool IsBool(TokenAvaiableValue value);
    bool IsNumeric(TokenAvaiableValue value);
    bool IsArray(TokenAvaiableValue value);
    bool IsRegexOrString(TokenAvaiableValue value);
    bool AdditionTypeCheck(TokenAvaiableValue left, TokenAvaiableValue right);
    bool ComparatorTypeCheck(TokenAvaiableValue left, TokenAvaiableValue right);

    // Operator type
    TokenAvaiableValue AddStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue SubtractStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue MultiplyStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue DivideStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue ExponentStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue ModulusStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue GteStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue GtStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue LteStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue LtStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue EqualStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue NotEqualStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue AndStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue OrStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue NegateStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue InvertStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue BitwiseNotStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue TernaryIfStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue TernaryElseStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue RegexStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue NotRegexStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue BitwiseOrStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue BitwiseAndStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue BitwiseXORStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue LeftShiftStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});
    TokenAvaiableValue RightShiftStage(TokenAvaiableValue, TokenAvaiableValue, Parameters = {});

    TokenAvaiableValue NoopStageRight(TokenAvaiableValue, TokenAvaiableValue, Parameters);
    TokenAvaiableValue InStage(TokenAvaiableValue, TokenAvaiableValue, Parameters);
    TokenAvaiableValue SeparatorStage(TokenAvaiableValue, TokenAvaiableValue, Parameters);

    EvaluationOperator MakeParameterStage(std::string parameter_name);
    EvaluationOperator MakeLiteralStage(TokenAvaiableValue);
} // Cvaluate


#endif