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

#ifndef CVALUATE_EVALUABLE_EXPRESSION
#define CVALUATE_EVALUABLE_EXPRESSION

#include "./pch.h"
#include "./Token.h"
#include "./Parising.h"
#include "./StagePlanner.h"

namespace Cvaluate {

class EvaluableExpression {
    private:
        std::string e_input;
        std::vector<ExpressionToken> e_tokens;
        std::shared_ptr<EvaluationStage> e_evaluation_stage;

        TokenAvaiableData EvaluateStage(std::shared_ptr<EvaluationStage>, Parameters);
    public:
        /**
         * Default constructor.
         * 
         * @param expression Eavl expression.
         */
        EvaluableExpression(std::string expression, 
            ExpressionFunctionMap functions = {});

        /**
         * Return Tokens copy
         */
        std::vector<ExpressionToken> Tokens();

        TokenAvaiableData Evaluate(Parameters);
};

}

#endif