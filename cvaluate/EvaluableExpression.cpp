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

#include <EvaluableExpression.h>
#include <Exception.h>

namespace Cvaluate {

EvaluableExpression::EvaluableExpression(std::string expression, 
            ExpressionFunctionMap functions) {
    this->e_input = expression;

    this->e_tokens = ParseTokens(expression, functions);

    this->e_evaluation_stage = PlanStages(this->e_tokens);
};

std::vector<ExpressionToken> EvaluableExpression::Tokens() {
    return this->e_tokens;
}

TokenAvaiableData EvaluableExpression::Evaluate(Parameters params) {

    return EvaluateStage(this->e_evaluation_stage, params);
}

TokenAvaiableData EvaluableExpression::EvaluateStage(std::shared_ptr<EvaluationStage> stage, Parameters params) {
    TokenAvaiableData left, right;
    if (stage == nullptr) {
        throw Cvaluate::CvaluateException("Found empty stage.");
    }

    if (stage != nullptr && stage->left_stage_) {
        left = this->EvaluateStage(stage->left_stage_, params);
    }

    if (stage != nullptr && stage->right_stage_) {
        right = this->EvaluateStage(stage->right_stage_, params);
    }

    auto ans = stage->operator_(left, right, params);

    return ans;
}

} // Cvaluate