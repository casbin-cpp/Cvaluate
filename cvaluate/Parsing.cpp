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

#include <Parising.h>
#include <Exception.h>

namespace Cvaluate {
    std::vector<ExpressionToken> ParseTokens(std::string& expression, ExpressionFunctionMap& functions) {
        std::stringstream stream(expression);
        std::vector<ExpressionToken> ret;
        // Parse state machine
        TokenState state = kValidTokenStates[0];

        while (stream.good()) {
            auto token = Readtoken(stream, state, functions);

            bool found = (token.Kind != TokenKind::UNKNOWN);

            if (!found) {
                break;
            }

            // Get Possible State
            if (!GetPossibleStateForToken(state, token.Kind)) {
                return ret;
            }

            ret.push_back(token);

        }

        return ret;
    }

    ExpressionToken Readtoken(std::stringstream& stream, TokenState& state, ExpressionFunctionMap functions) {
        auto kind = TokenKind::UNKNOWN;
        std::string token_string;
        TokenAvaiableValue token_value;
        ExpressionToken ret;
        bool conditioned;

        while (stream.good()) {
            char character = stream.get();

            if (!stream.good()) {
                break;
            }

            if (std::isspace(character)) {
                continue;
            }

            kind = TokenKind::UNKNOWN;

            // numeric constant
            if (IsNumeric(character)) {
                if (character == '0') {
                    character = stream.get();

                    if (stream.good() && character == 'x') {
                        auto token_string = ReadUntilFalse(stream, false, true, true, IsHeaxDigit, conditioned);
                        auto token_value_int = std::stoi(token_string, nullptr, 16);
                        
                        kind = TokenKind::NUMERIC;
                        token_value = (token_value_int);
                        break;
                    } else {
                        stream.unget();
                        stream.clear();
                    }
                }

                // Use float in library.
                token_string = ReadTokenUntilFalse(stream, IsNumeric);
                token_value = std::stof(token_string);
                // if (token_string.find('.') != std::string::npos) {
                //     token_value = std::stof(token_string);
                // } else {
                //     token_value = std::stoi(token_string);
                // }
                
                kind = TokenKind::NUMERIC;
                break;
            }

            // comma, separator
            if (character == ',') {
                token_value = std::string(",");
                kind = TokenKind::SEPARATOR;
                break;
            }

            // escaped variable
            if (character == '[') {
                auto token_string = ReadUntilFalse(stream, true, false, true, IsNotClosingBracket, conditioned);
                kind = TokenKind::VARIABLE;
                token_value = token_string;

                if (!conditioned) {
                    throw CvaluateException("Broken operator of []");
                    return ret;
                }

                stream.get();
                break;
            }

            if (std::isalpha(character)) {
                auto token_string = ReadTokenUntilFalse(stream, IsVariableName);

                token_value = token_string;
                kind = TokenKind::VARIABLE;

                // boolen
                if (token_string == "true") {
                    kind = TokenKind::BOOLEAN;
                    token_value = true;
                } else if (token_string == "false") {
                    kind = TokenKind::BOOLEAN;
                    token_value = false;
                }

                // operator
                if (token_string == "in" || token_string == "IN") {
                    token_value = std::string("in");
                    kind = TokenKind::COMPARATOR;
                }

                // is function
                if (functions.find(token_string) != functions.end()) {
                    kind = TokenKind::FUNCTION;
                    token_value = functions[token_string];
                }

                // accessor
                size_t accessor_index = token_string.find('.');
                if (accessor_index != std::string::npos) {
                    if (token_string.back() == '.') {
                        throw CvaluateException("Hanging accessor on token" + token_string);
                    }

                    kind = TokenKind::ACCESSOR;
                    auto splits = Split(token_string, ".", -1);
                    token_value = splits;
                }
                break;
            }

            // ' / "
            if (!IsNotQuote(character)) {
                auto token_string = ReadUntilFalse(stream, true, false, true, IsNotQuote, conditioned);

                if (!conditioned) {
                    throw CvaluateException("Unclosed string literal \' \'");
                }

                stream.get();
                // Assert stream is still good

                kind = TokenKind::STRING;
                token_value = token_string;
                break;
            }

            if (character == '(') {
                token_value = std::string("(");
                kind = TokenKind::CLAUSE;
                break;
            }

            if (character == ')') {
                token_value = std::string(")");
                kind = TokenKind::CLAUSE_CLOSE;
                break;
            }

            // must be a known symbol
            token_string = ReadTokenUntilFalse(stream, IsNotAlphanumeric);
            token_value = token_string;

            // quick hack for the case where "-" can mean "prefixed negation" or "minus", which are used
		    // very differently.
            if (CanTransitionTo(state, TokenKind::PREFIX)) {
                if (kPrefixSymbols.find(token_string) != kPrefixSymbols.end()) {
                    kind = TokenKind::PREFIX;
                    break;
                }
            }

            if (kModifierSymbols.find(token_string) != kModifierSymbols.end()) {
                kind = TokenKind::MODIFIER;
                break;
            }

            if (kLogicalSymbols.find(token_string) != kLogicalSymbols.end()) {
                kind = TokenKind::LOGICALOP;
                break;
            }

            if (kComparatorSymbols.find(token_string) != kComparatorSymbols.end()) {
                kind = TokenKind::COMPARATOR;
                break;
            }

            if (kTernarySymbols.find(token_string) != kTernarySymbols.end()) {
                kind = TokenKind::TERNARY;
                break;
            }

            throw CvaluateException("Unsupport token:" + token_string);

            return ret;
        }

        ret.Kind = kind;
        ret.Value = token_value;

        return ret;
    }
    // Read string don't check condition status
    std::string ReadTokenUntilFalse(std::stringstream& stream, std::function<bool(char)> condition) {
        stream.unget();
        stream.clear();
        bool conditioned;
        return ReadUntilFalse(stream, false, true, true, condition, conditioned);
    }

    // Read string until condition occur.
    std::string ReadUntilFalse(std::stringstream& stream, bool include_white_space, bool break_white_space, 
            bool allow_escaping, std::function<bool(char)> condition, bool& conditioned) {
        std::string token;
        char character;
        conditioned = false;

        while (stream.good()) {
            character = stream.get();

            if (!stream.good()) {
                break;
            }

            if (allow_escaping && character == '\\') {
                character = stream.get();
                token.push_back(character);
                continue;
            }

            if (std::isspace(character)) {
                if (break_white_space && token.size() > 0) {
                    conditioned = true;
                    break;
                }

                if (!include_white_space) {
                    continue;
                }
            }

            if (condition(character)) {
                token.push_back(character);
            } else {
                conditioned = true;
                stream.unget();
                stream.clear();
                break;
            }
        }

        return token;
    }

    #define LARGE 2147483647

    std::vector<std::string> Split(std::string str, const std::string& del, int limit) {
        std::vector<std::string> tokens;

        if(limit <= 0)
            limit = LARGE;
        tokens.reserve((limit == LARGE) ? 0 : limit);
        for (int i = 1; i < limit ; i++) {
            size_t pos = str.find(del);
            if (pos != std::string::npos) {
                tokens.emplace_back(str.substr(0, pos));
                str = str.substr(pos + del.length());
            }
            else
                break;
        }

        tokens.emplace_back(str);

        return tokens;
    }


    bool IsNumeric(char character) {
        return std::isdigit(character) || character == '.';
    }

    bool IsHeaxDigit(char character) {
        character = tolower(character);

        return isdigit(character) || 
            character == 'a' ||
            character == 'b' ||
            character == 'c' ||
            character == 'd' ||
            character == 'e' ||
            character == 'f';
    }

    bool IsNotClosingBracket(char character ) {

	    return character != ']';
    }

    bool IsVariableName(char character) {

        return std::isalpha(character) ||
            std::isdigit(character) ||
            character == '_' ||
            character == '.';
    }

    bool IsNotQuote(char character) {
	    return character != '\'' && character != '"';
    }

    bool IsNotAlphanumeric(char character) {
        return !(std::isdigit(character) ||
            std::isalpha(character) ||
            character == '(' ||
            character == ')' ||
            character == '[' ||
            character == ']' ||
            !IsNotQuote(character));
    }
} // Cvaluate