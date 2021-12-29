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

#ifndef CVALUATE_STAGE_PLANNER
#define CVALUATE_STAGE_PLANNER

#include <EvaluationStage.h>
#include <TokenStream.h>

namespace Cvaluate {
    /*
        A "precedent" is a function which will recursively parse new evaluateionStages from a given stream of tokens.
        It's called a `precedent` because it is expected to handle exactly what precedence of operator,
        and defer to other `precedent`s for other operators.
    */
    class Precedent {
        public:
            using function_type = std::function<std::shared_ptr<EvaluationStage>(TokenStream)>;
            function_type generate_func = nullptr;
            const StringOperatorSymbolMap valid_symbols;
            std::vector<TokenKind> valid_kinds;
            Precedent* next;
            Precedent* next_right;

            Precedent(function_type func) {
                generate_func = func;
            }

            Precedent(const StringOperatorSymbolMap valid_symbols_, std::vector<TokenKind> valid_kinds_,
                    Precedent* next_, Precedent* next_right_) :
                    valid_symbols(valid_symbols_), valid_kinds(valid_kinds_), next(next_), next_right(next_right_) {
                generate_func = nullptr;
            }

            std::shared_ptr<EvaluationStage> PlanPrecedenceLevel(TokenStream stream,
                const StringOperatorSymbolMap vlaid_symbols, std::vector<TokenKind> valid_kinds,
                Precedent* right_precedent, Precedent* left_precedent);

            std::shared_ptr<EvaluationStage> operator()(TokenStream& stream) {
                if (!IsNullptrFunction()) {
                    return generate_func(stream);
                } else {
                    return PlanPrecedenceLevel(stream, valid_symbols, valid_kinds, next, next_right);
                }
            }

            bool IsNullptrFunction() {
                return generate_func == nullptr;
            }
    };
    /*
	Convenience function to pass a triplet of typechecks between `findTypeChecks` and `planPrecedenceLevel`.
	Each of these members may be nil, which indicates that type does not matter for that value.
    */
    struct TypeChecks{
        StageTypeCheck left;
        StageTypeCheck right;
        StageCombinedTypeCheck combined;
    };
    

    std::shared_ptr<EvaluationStage> PlanStages(std::vector<ExpressionToken> tokens);
    std::shared_ptr<EvaluationStage> PlanTokens(TokenStream& stream);
    void RecorderStages(std::shared_ptr<EvaluationStage> root_stage);

    std::shared_ptr<EvaluationStage> PlanFunctions(TokenStream stream);
    std::shared_ptr<EvaluationStage> PlanAccessor(TokenStream stream);
    std::shared_ptr<EvaluationStage> PlanValue(TokenStream stream);
    TypeChecks FindTypeChecks(OperatorSymbol);
} //Cvaluate

#endif