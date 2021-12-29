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

#ifndef CVALUATE_OPERATOR_SYMBOL
#define CVALUATE_OPERATOR_SYMBOL
#include <pch.h>

namespace Cvaluate {
    enum class OperatorSymbol {
        OperatorSymbol,
        LITERAL,
        NOOP,
        EQ,
        NEQ,
        GT,
        LT,
        GTE,
        LTE,
        REQ,
        NREQ,
        IN,

        AND,
        OR,

        PLUS,
        MINUS,
        BITWISE_AND,
        BITWISE_OR,
        BITWISE_XOR,
        BITWISE_LSHIFT,
        BITWISE_RSHIFT,
        MULTIPLY,
        DIVIDE,
        MODULUS,
        EXPONENT,

        NEGATE,
        INVERT,
        BITWISE_NOT,

        TERNARY_TRUE,
        TERNARY_FALSE,
        COALESCE,

        FUNCTIONAL,
        ACCESS,
        SEPARATE,
    };

    using StringOperatorSymbolMap = std::unordered_map<std::string, OperatorSymbol>;

    const StringOperatorSymbolMap kComparatorSymbols = {
        {"==", OperatorSymbol::EQ},
        {"!=", OperatorSymbol::NEQ},
        {">", OperatorSymbol::GT},
        {">=", OperatorSymbol::GTE},
        {"<",  OperatorSymbol::LT},
        {"<=", OperatorSymbol::LTE},
        {"=~", OperatorSymbol::REQ},
        {"!~", OperatorSymbol::NREQ},
        {"in", OperatorSymbol::IN},
    };

    const StringOperatorSymbolMap kLogicalSymbols = {
        {"&&", OperatorSymbol::AND},
        {"||", OperatorSymbol::OR},
    };

    const StringOperatorSymbolMap kBitwiseSymbols= {
        {"^", OperatorSymbol::BITWISE_XOR},
        {"&", OperatorSymbol::BITWISE_AND},
        {"|", OperatorSymbol::BITWISE_OR},
    };

    const StringOperatorSymbolMap kBitwiseShiftSymbols = {
        {">>", OperatorSymbol::BITWISE_RSHIFT},
        {"<<", OperatorSymbol::BITWISE_LSHIFT},
    };

    const StringOperatorSymbolMap kAdditiveSymbols = {
        {"+", OperatorSymbol::PLUS},
        {"-", OperatorSymbol::MINUS},
    };

    const StringOperatorSymbolMap kMultiplicativeSymbols = {
        {"*", OperatorSymbol::MULTIPLY},
        {"/", OperatorSymbol::DIVIDE},
        {"%", OperatorSymbol::MODULUS},
    };

    const StringOperatorSymbolMap kExponentialSymbolsS = {
        {"**", OperatorSymbol::EXPONENT},
    };

    const StringOperatorSymbolMap kPrefixSymbols = {
        {"-", OperatorSymbol::NEGATE},
        {"!", OperatorSymbol::INVERT},
        {"~", OperatorSymbol::BITWISE_NOT},
    };

    const StringOperatorSymbolMap kTernarySymbols = {
        {"?", OperatorSymbol::TERNARY_TRUE},
        {":", OperatorSymbol::TERNARY_FALSE},
        {"??", OperatorSymbol::COALESCE},
    };

    const StringOperatorSymbolMap kModifierSymbols = {
        {"+", OperatorSymbol::PLUS},
        {"-", OperatorSymbol::MINUS},
        {"*", OperatorSymbol::MULTIPLY},
        {"/", OperatorSymbol::DIVIDE},
        {"%", OperatorSymbol::MODULUS},
        {"**", OperatorSymbol::EXPONENT},
        {"&", OperatorSymbol::BITWISE_AND},
        {"|", OperatorSymbol::BITWISE_OR},
        {"^", OperatorSymbol::BITWISE_XOR},
        {">>", OperatorSymbol::BITWISE_RSHIFT},
        {"<<", OperatorSymbol::BITWISE_LSHIFT},
    };

    const StringOperatorSymbolMap kSeparatorSymbols = {
        {",", OperatorSymbol::SEPARATE}
    };

} // Cvaluate
#endif