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
#include <cvaluate/StagePlanner.h>
#include <cvaluate/Exception.h>

namespace Cvaluate {
    std::unordered_map<OperatorSymbol, EvaluationOperator> kStageSymbolMap = {
        {OperatorSymbol::EQ, EqualStage},
        {OperatorSymbol::NEQ, NotEqualStage},
        {OperatorSymbol::GT, GtStage},
        {OperatorSymbol::LT, LtStage},
        {OperatorSymbol::GTE, GteStage},
        {OperatorSymbol::LTE, LteStage},
        {OperatorSymbol::REQ, RegexStage},
        {OperatorSymbol::NREQ, NotRegexStage},
        {OperatorSymbol::AND, AndStage},
        {OperatorSymbol::OR, OrStage},
        {OperatorSymbol::IN, InStage},
        {OperatorSymbol::BITWISE_OR, BitwiseOrStage},
        {OperatorSymbol::BITWISE_AND, BitwiseAndStage},
        {OperatorSymbol::BITWISE_XOR, BitwiseXORStage},
        {OperatorSymbol::BITWISE_LSHIFT, LeftShiftStage},
        {OperatorSymbol::BITWISE_RSHIFT, RightShiftStage},
        {OperatorSymbol::PLUS, AddStage},
        {OperatorSymbol::MINUS, SubtractStage},
        {OperatorSymbol::MULTIPLY, MultiplyStage},
        {OperatorSymbol::DIVIDE, DivideStage},
        {OperatorSymbol::MODULUS, ModulusStage},
        {OperatorSymbol::EXPONENT, ExponentStage},
        {OperatorSymbol::NEGATE, NegateStage},
        {OperatorSymbol::INVERT, InvertStage},
        {OperatorSymbol::BITWISE_NOT, BitwiseNotStage},
        {OperatorSymbol::TERNARY_TRUE, TernaryIfStage},
        {OperatorSymbol::TERNARY_FALSE, TernaryElseStage},
        {OperatorSymbol::COALESCE, TernaryElseStage},
        {OperatorSymbol::SEPARATE, SeparatorStage},
    };

    Precedent planFunctions = PlanFunctions;
    PrecedencePlanner planPrefix(kPrefixSymbols, {TokenKind::PREFIX}, nullptr, planFunctions);
    PrecedencePlanner planExponential(kExponentialSymbolsS, {TokenKind::MODIFIER}, planFunctions, nullptr);
    PrecedencePlanner planMultiplicative(kMultiplicativeSymbols, {TokenKind::MODIFIER}, planExponential, nullptr);
    PrecedencePlanner planAdditive(kAdditiveSymbols, {TokenKind::MODIFIER}, planMultiplicative, nullptr);
    PrecedencePlanner planShift(kBitwiseShiftSymbols, {TokenKind::MODIFIER}, planAdditive, nullptr);
    PrecedencePlanner planBitwise(kBitwiseSymbols, {TokenKind::MODIFIER}, planShift, nullptr);
    PrecedencePlanner planComparator(kComparatorSymbols, {TokenKind::COMPARATOR}, planBitwise, nullptr);
    PrecedencePlanner planLogicalAnd({{"&&", OperatorSymbol::AND}}, {TokenKind::LOGICALOP}, planComparator, nullptr);
    PrecedencePlanner planLogicalOr({{"||", OperatorSymbol::OR}}, {TokenKind::LOGICALOP}, planLogicalAnd, nullptr);
    PrecedencePlanner planTernary(kTernarySymbols, {TokenKind::TERNARY}, planLogicalOr, nullptr);
    PrecedencePlanner planSeparator(kSeparatorSymbols, {TokenKind::SEPARATOR}, planTernary, nullptr);

    /*
        Creates a `evaluationStageList` object which represents an execution plan (or tree)
        which is used to completely evaluate a set of tokens at evaluation-time.
        The three stages of evaluation can be thought of as parsing strings to tokens, then tokens to a stage list, then evaluation with parameters.
    */
    std::shared_ptr<EvaluationStage> PlanStages(std::vector<ExpressionToken>& tokens) {
        auto stream = TokenStream(tokens);

        auto stage = PlanTokens(stream);

        // while we're now fully-planned, we now need to re-order same-precedence operators.
        // this could probably be avoided with a different planning method
        RecorderStages(stage);

        return stage;
    }

    std::shared_ptr<EvaluationStage> PlanTokens(TokenStream& stream) {
        if (!stream.HasNext()) {
            return nullptr;
        }

        return planSeparator(stream);
    }

    /*
        During stage planning, stages of equal precedence are parsed such that they'll be evaluated in reverse order.
        For commutative operators like "+" or "-", it's no big deal. But for order-specific operators, it ruins the expected result.
    */
    void RecorderStages(std::shared_ptr<EvaluationStage> root_stage) {
        std::shared_ptr<EvaluationStage> next_stage, current_stage;
        OperatorPrecedence precedence, current_precedence;
        std::vector<std::shared_ptr<EvaluationStage>> identical_precedences;

        next_stage = root_stage;
        precedence = FindOperatorPrecedenceForSymbol(root_stage->symbol_);

        while (next_stage != nullptr) {
            current_stage = next_stage;
            next_stage = current_stage->right_stage_;

            if (current_stage->left_stage_) {
                RecorderStages(current_stage->left_stage_);
            }

            current_precedence = FindOperatorPrecedenceForSymbol(current_stage->symbol_);

            // current precedence is same to right precedence, put to identical_predence.
            if (current_precedence == precedence) {
                identical_precedences.push_back(current_stage);
                continue;
            }

            if (identical_precedences.size() > 1) {
                MirrorStageSubtree(identical_precedences);
            }

            identical_precedences = {current_stage};
            precedence = current_precedence;
        }

        if (identical_precedences.size() > 1) {
            MirrorStageSubtree(identical_precedences);
        }
    }

    /*
        The most usual method of parsing an evaluation stage for a given precedence.
        Most stages use the same logic
    */
    std::shared_ptr<EvaluationStage> PrecedencePlanner::PlanPrecedenceLevel(TokenStream& stream,
            StringOperatorSymbolMap valid_symbols, std::vector<TokenKind> valid_kinds,
            Precedent right_precedent, Precedent left_precedent) {
        ExpressionToken token;
        OperatorSymbol symbol = OperatorSymbol::VALUE;
        std::shared_ptr<EvaluationStage> right_stage = nullptr;
        std::shared_ptr<EvaluationStage> left_stage = nullptr;
        TypeChecks checks;
        bool key_found = false;

        if (left_precedent != nullptr) {
            left_stage = left_precedent(stream);
        }

        while (stream.HasNext()) {
            token = *stream.Next();

            if (valid_kinds.size() > 0) {
                key_found = false;

                auto find_key = std::find(valid_kinds.begin(), valid_kinds.end(), token.Kind);
                key_found = (find_key != valid_kinds.end());

                if (!key_found) {
                    break;
                }
            }

            if (!valid_symbols.empty()) {
                auto data = GetTokenValueData(token.Value);
                if (!IsString(data)) {
                    break;
                }

                auto token_string = data.get<std::string>();

                if (valid_symbols.find(token_string) == valid_symbols.end()) {
                    break;
                } else {
                    symbol = valid_symbols[token_string];
                }
            }

            if (right_precedent != nullptr) {
                right_stage = right_precedent(stream);
            }

            checks = FindTypeChecks(symbol);

            auto ret = std::make_shared<EvaluationStage>(symbol, left_stage, right_stage, kStageSymbolMap[symbol],
                checks.left, checks.right, checks.combined);

            return ret;
        }

        stream.Rewind();

        return left_stage;
    }

    /*
        A truly special precedence function, this handles all the "lowest-case" errata of the process, including literals, parmeters,
        clauses, and prefixes.
    */
    std::shared_ptr<EvaluationStage> PlanValue(TokenStream& stream) {

        if (!stream.HasNext()) {
            return nullptr;
        }
        EvaluationOperator plan_operator = nullptr;
        OperatorSymbol symbol;

        auto token = stream.Next();

        switch (token->Kind)
        {
            case TokenKind::CLAUSE: {
                auto right_stage = PlanTokens(stream);
		        // advance past the CLAUSE_CLOSE token. 
                // We know that it's a CLAUSE_CLOSE, because at parse-time we check for unbalanced parens.
                stream.Next();

                auto ret = std::make_shared<EvaluationStage>(OperatorSymbol::NOOP, nullptr, right_stage, 
                        NoopStageRight, nullptr, nullptr, nullptr);

                return ret;
            }
            
            case TokenKind::CLAUSE_CLOSE: {
                stream.Rewind();

                return nullptr;
            }

            case TokenKind::VARIABLE: {
                plan_operator = MakeParameterStage(GetTokenValueString(token->Value));
                break;
            }

            case TokenKind::NUMERIC:
            case TokenKind::STRING:
            case TokenKind::PATTERN:
            case TokenKind::BOOLEAN: {
                symbol = OperatorSymbol::LITERAL;
                plan_operator = MakeLiteralStage(GetTokenValueData(token->Value));
                break;
            }

            case TokenKind::PREFIX: {
                stream.Rewind();
                return planPrefix(stream);
            }
            default:
                break;
        }

        if (plan_operator == nullptr) {
            throw CvaluateException("Unable to plan token");
        }

        auto ret = std::make_shared<EvaluationStage>(
            symbol,
            nullptr,
            nullptr,
            plan_operator,
            nullptr,
            nullptr,
            nullptr
        );

        return ret;
    }

    std::shared_ptr<EvaluationStage> PlanFunctions(TokenStream& stream) {
        auto token = stream.Next();

        if (token->Kind != TokenKind::FUNCTION) {
            stream.Rewind();
            return PlanAccessor(stream);
        }

        auto right_stage = PlanAccessor(stream);

        auto ret = std::make_shared<EvaluationStage>(
            OperatorSymbol::FUNCTIONAL,
            nullptr,
            right_stage,
            MakeFunctionStage(GetTokenValueFunction(token->Value)),
            nullptr,
            nullptr,
            nullptr
        );

        return ret;
    }

    std::shared_ptr<EvaluationStage> PlanAccessor(TokenStream& stream) {
        auto token = stream.Next();

        if (token->Kind != TokenKind::ACCESSOR) {
            stream.Rewind();
            return PlanValue(stream);
        }

        // throw CvaluateException("PlanAccessor Not Implement");
        
        if (stream.HasNext()) {
            auto otherToken = stream.Next();
            if (otherToken->Kind == TokenKind::CLAUSE) {
                throw CvaluateException("Don't support member function.");
            } else {
                stream.Rewind();
            }
        }

        auto ret = std::make_shared<EvaluationStage>(
            OperatorSymbol::ACCESS,
            nullptr,
            nullptr,
            MakeAccessorStage(GetTokenValueData(token->Value)),
            nullptr,
            nullptr,
            nullptr
        );

        return ret;
    }

    /*
        Performs a "mirror" on a subtree of stages.
        This mirror functionally inverts the order of execution for all members of the [stages] list.
        That list is assumed to be a root-to-leaf (ordered) list of evaluation stages, where each is a right-hand stage of the last.
    */
    void MirrorStageSubtree(std::vector<std::shared_ptr<EvaluationStage>>& stages) {
        std::shared_ptr<EvaluationStage> root_stage, inverse_stage, carry_stage, front_stage;

        int stage_length = stages.size();

        // reverse left and right
        for (auto front_stage: stages) {
            carry_stage = front_stage->right_stage_;
            front_stage->right_stage_ = front_stage->left_stage_;
            front_stage->left_stage_ = carry_stage;
        }

        // end left swaps with root right
        root_stage = stages[0];
        front_stage = stages.back();

        carry_stage = front_stage->left_stage_;
        front_stage->left_stage_ = root_stage->right_stage_;
        root_stage->right_stage_ = carry_stage;

        // for all non-root non-end stages, right is swapped with inverse stage right in list
        for (int i = 0; i < (stage_length - 2) / 2 + 1; i++) {
            front_stage = stages[i + 1];
            inverse_stage = stages[stage_length - i - 1];

            carry_stage = front_stage->right_stage_;
            front_stage->right_stage_ = inverse_stage->right_stage_;
            inverse_stage->right_stage_ = carry_stage;
        }

        // swap all other information with inverse stage
        for (int i = 0; i < stage_length / 2; i++) {
            front_stage = stages[i];
            inverse_stage = stages[stage_length - i - 1];
            front_stage->SwapWith(inverse_stage);
        }
    }
    /*
        Maps a given [symbol] to a set of typechecks to be used during runtime.
    */
    TypeChecks FindTypeChecks(OperatorSymbol symbol) {
        switch (symbol) {
            case OperatorSymbol::GT:
            case OperatorSymbol::LT:
            case OperatorSymbol::GTE:
            case OperatorSymbol::LTE:
                return TypeChecks{
                    nullptr,
                    nullptr,
                    ComparatorTypeCheck,
                };
            case OperatorSymbol::REQ:
            case OperatorSymbol::NREQ:
                return TypeChecks{
                    IsString,
                    IsRegexOrString,
                    nullptr,
                };
            case OperatorSymbol::AND:
            case OperatorSymbol::OR:
                return TypeChecks{
                    IsBool,
                    IsBool,
                    nullptr,
                };
            case OperatorSymbol::IN:
                return TypeChecks{
                    nullptr,
                    IsArray,
                    nullptr,
                };
            case OperatorSymbol::BITWISE_LSHIFT:
            case OperatorSymbol::BITWISE_RSHIFT:
            case OperatorSymbol::BITWISE_OR:
            case OperatorSymbol::BITWISE_AND:
            case OperatorSymbol::BITWISE_XOR:
                return TypeChecks{
                    IsNumeric,
                    IsNumeric,
                    nullptr
                };
            case OperatorSymbol::PLUS:
                return TypeChecks{
                    nullptr,
                    nullptr,
                    AdditionTypeCheck,
                };
            case OperatorSymbol::MINUS:
            case OperatorSymbol::MULTIPLY:
            case OperatorSymbol::DIVIDE:
            case OperatorSymbol::MODULUS:
            case OperatorSymbol::EXPONENT:
                return TypeChecks{
                    IsNumeric,
                    IsNumeric,
                    nullptr,
                };
            case OperatorSymbol::NEGATE:
                return TypeChecks{
                    nullptr,
                    IsNumeric,
                    nullptr,
                };
            case OperatorSymbol::INVERT:
                return TypeChecks{
                    nullptr,
                    IsBool,
                    nullptr,
                };
            case OperatorSymbol::BITWISE_NOT:
                return TypeChecks{
                    nullptr,
                    IsNumeric,
                    nullptr,
                };
            case OperatorSymbol::TERNARY_TRUE:
                return TypeChecks{
                    IsBool,
                    nullptr,
                    nullptr
                };

            // unchecked cases
            case OperatorSymbol::EQ:
            case OperatorSymbol::NEQ:
                return TypeChecks{};
            case OperatorSymbol::TERNARY_FALSE:
            case OperatorSymbol::COALESCE:
            default:
                return TypeChecks{};
            }
    }
} // Cvaluate