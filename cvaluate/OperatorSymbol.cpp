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
#include <OperatorSymbol.h>

namespace Cvaluate {

OperatorPrecedence FindOperatorPrecedenceForSymbol(OperatorSymbol symbol)  {

	switch (symbol) {
        case OperatorSymbol::NOOP:
            return OperatorPrecedence::NoopPrecedence;
        case OperatorSymbol::VALUE:
            return OperatorPrecedence::ValuePrecedence;
        case OperatorSymbol::EQ:
        case OperatorSymbol::NEQ:
        case OperatorSymbol::GT:
        case OperatorSymbol::LT:
        case OperatorSymbol::GTE:
        case OperatorSymbol::LTE:
        case OperatorSymbol::REQ:
        case OperatorSymbol::NREQ:
        case OperatorSymbol::IN:
            return OperatorPrecedence::ComparatorPrecedence;
        case OperatorSymbol::AND:
            return OperatorPrecedence::LogicalAndPrecedence;
        case OperatorSymbol::OR:
            return OperatorPrecedence::LogicalOrPrecedence;
        case OperatorSymbol::BITWISE_AND:
        case OperatorSymbol::BITWISE_OR:
        case OperatorSymbol::BITWISE_XOR:
            return  OperatorPrecedence::BitwisePrecedence;
        case OperatorSymbol::BITWISE_LSHIFT:
        case OperatorSymbol::BITWISE_RSHIFT:
            return OperatorPrecedence::BitwiseShiftPrecedence;
        case OperatorSymbol::PLUS:
        case OperatorSymbol::MINUS:
            return OperatorPrecedence::AdditivePrecedence;
        case OperatorSymbol::MULTIPLY:
        case OperatorSymbol::DIVIDE:
        case OperatorSymbol::MODULUS:
            return OperatorPrecedence::MultiplicativePrecedence;
        case OperatorSymbol::EXPONENT:
            return OperatorPrecedence::ExponentialPrecedence;
        case OperatorSymbol::BITWISE_NOT:
        case OperatorSymbol::NEGATE:
        case OperatorSymbol::INVERT:
            return OperatorPrecedence::PrefixPrecedence;
        case OperatorSymbol::COALESCE:
        case OperatorSymbol::TERNARY_TRUE:
        case OperatorSymbol::TERNARY_FALSE:
            return OperatorPrecedence::TernaryPrecedence;
        case OperatorSymbol::ACCESS:
        case OperatorSymbol::FUNCTIONAL:
            return OperatorPrecedence::FunctionalPrecedence;
        case OperatorSymbol::SEPARATE:
            return OperatorPrecedence::SeparatePrecedence;
	}

	return OperatorPrecedence::ValuePrecedence;
}
} // cvaluate