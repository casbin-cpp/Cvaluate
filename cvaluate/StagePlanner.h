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
    using Precedent = std::function<std::shared_ptr<EvaluationStage>(TokenStream&)>;

    class PrecedencePlanner {
        public:
            StringOperatorSymbolMap valid_symbols;
            std::vector<TokenKind> valid_kinds;
            Precedent next;
            Precedent next_right;

            PrecedencePlanner(StringOperatorSymbolMap valid_symbols, std::vector<TokenKind> valid_kinds,
                            Precedent next, Precedent next_right) {
                this->valid_symbols = valid_symbols;
                this->valid_kinds = valid_kinds;
                this->next = next;
                this->next_right = next_right;
            };

            std::shared_ptr<EvaluationStage> PlanPrecedenceLevel(TokenStream& stream,
                StringOperatorSymbolMap vlaid_symbols, std::vector<TokenKind> valid_kinds,
                Precedent right_precedent, Precedent left_precedent);

            std::shared_ptr<EvaluationStage> operator()(TokenStream& stream) {
                Precedent generated = nullptr;
                Precedent nextRight = nullptr;

                generated = [&] (TokenStream& stream) -> std::shared_ptr<EvaluationStage> {
                    return PlanPrecedenceLevel(
                        stream,
                        this->valid_symbols,
                        this->valid_kinds,
                        nextRight,
                        this->next
                    );
                };

                // left precence > current, right precence >= current
                if (this->next_right != nullptr) {
                    nextRight = this->next_right;
                } else {
                    nextRight = generated;
                }
                
                return generated(stream);
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
    
    std::shared_ptr<EvaluationStage> PlanStages(std::vector<ExpressionToken>& tokens);
    std::shared_ptr<EvaluationStage> PlanTokens(TokenStream& stream);
    void RecorderStages(std::shared_ptr<EvaluationStage> root_stage);

    std::shared_ptr<EvaluationStage> PlanFunctions(TokenStream& stream);
    std::shared_ptr<EvaluationStage> PlanAccessor(TokenStream& stream);
    std::shared_ptr<EvaluationStage> PlanValue(TokenStream& stream);
    TypeChecks FindTypeChecks(OperatorSymbol);
} //Cvaluate

#endif