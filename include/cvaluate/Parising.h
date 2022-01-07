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

#ifndef CVALUATE_PARSING
#define CVALUATE_PARSING

#include "./pch.h"
#include "./Token.h"
#include "./OperatorSymbol.h"

namespace Cvaluate {
    std::vector<ExpressionToken> ParseTokens(std::string& expression, ExpressionFunctionMap& functions);
    
    ExpressionToken Readtoken(std::stringstream& stream, TokenState& state, ExpressionFunctionMap functions);
    std::string ReadTokenUntilFalse(std::stringstream& stream, std::function<bool(char character)> condition);
    std::pair<std::string, bool> ReadUntilFalse(std::stringstream& stream, bool include_white_space, bool break_white_space, 
            bool allow_escaping, std::function<bool(char character)> condition);

    std::vector<std::string> Split(std::string str, const std::string& del, int limit);

    bool IsHeaxDigit(char character);
    bool IsNumeric(char character);
    bool IsHeaxDigit(char character);
    bool IsNotClosingBracket(char character);
    bool IsVariableName(char character);
    bool IsNotQuote(char character);
    bool IsNotAlphanumeric(char character);
}

#endif