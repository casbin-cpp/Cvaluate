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

#ifndef CVALUATE_TOKEN
#define CVALUATE_TOKEN
#include <pch.h>
#include <nlohmann/json.hpp>

namespace Cvaluate {

    using ExpressionFunction = std::function<bool(void*)>;
    using ExpressionFunctionMap = std::unordered_map<std::string, ExpressionFunction>;

    using TokenAvaiableValue = std::variant<
            int,
            bool,
            float,
            std::string, 
            std::vector<std::string>,
            nlohmann::json,
            ExpressionFunction>;

    enum class TokenKind {
        UNKNOWN = 0,

        PREFIX,
        NUMERIC,
        BOOLEAN,
        STRING,
        PATTERN,
        TIME,
        VARIABLE,
        FUNCTION,
        SEPARATOR,
        ACCESSOR,

        COMPARATOR,
        LOGICALOP,
        MODIFIER,

        CLAUSE,
        CLAUSE_CLOSE,

        TERNARY,
    };

    struct ExpressionToken {
        TokenKind Kind;
        TokenAvaiableValue Value;
        ExpressionToken() {};
        ExpressionToken(TokenKind kind_, TokenAvaiableValue value_) :
            Kind(kind_), Value(value_) {};
    };

    // State machine for paring string.
    struct TokenState {
        TokenKind Kind;
        bool IsEOF;
        bool IsNullable;
        std::vector<TokenKind> ValidNextKinds;
    };

    bool CanTransitionTo(const TokenState& cur_state, const TokenKind& next_kind);

    bool GetPossibleStateForToken(TokenState& state, TokenKind& kind);

    const TokenState kValidTokenStates[] = {
        {
            TokenKind::UNKNOWN,
            false,
            true,
            {

			    TokenKind::PREFIX,
                TokenKind::NUMERIC,
                TokenKind::BOOLEAN,
                TokenKind::VARIABLE,
                TokenKind::PATTERN,
                TokenKind::FUNCTION,
                TokenKind::ACCESSOR,
                TokenKind::STRING,
                TokenKind::TIME,
                TokenKind::CLAUSE,
            }
        },

	    {
		    TokenKind::CLAUSE,
		    false,
		    true,
		    {

                TokenKind::PREFIX,
                TokenKind::NUMERIC,
                TokenKind::BOOLEAN,
                TokenKind::VARIABLE,
                TokenKind::PATTERN,
                TokenKind::FUNCTION,
                TokenKind::ACCESSOR,
                TokenKind::STRING,
                TokenKind::TIME,
                TokenKind::CLAUSE,
                TokenKind::CLAUSE_CLOSE,
		    },
	    },

        {
            TokenKind::CLAUSE_CLOSE,
            true,
            true,
            {

                TokenKind::COMPARATOR,
                TokenKind::MODIFIER,
                TokenKind::NUMERIC,
                TokenKind::BOOLEAN,
                TokenKind::VARIABLE,
                TokenKind::STRING,
                TokenKind::PATTERN,
                TokenKind::TIME,
                TokenKind::CLAUSE,
                TokenKind::CLAUSE_CLOSE,
                TokenKind::LOGICALOP,
                TokenKind::TERNARY,
                TokenKind::SEPARATOR,
            },
        },

        {
            TokenKind::NUMERIC,
            true,
            false,
            {

                TokenKind::MODIFIER,
                TokenKind::COMPARATOR,
                TokenKind::LOGICALOP,
                TokenKind::CLAUSE_CLOSE,
                TokenKind::TERNARY,
                TokenKind::SEPARATOR,
            },
        },

        {
            TokenKind::BOOLEAN,
            true,
            false,
            {

                TokenKind::MODIFIER,
                TokenKind::COMPARATOR,
                TokenKind::LOGICALOP,
                TokenKind::CLAUSE_CLOSE,
                TokenKind::TERNARY,
                TokenKind::SEPARATOR,
            },
        },

        {
            TokenKind::STRING,
            true,
            false,
            {

                TokenKind::MODIFIER,
                TokenKind::COMPARATOR,
                TokenKind::LOGICALOP,
                TokenKind::CLAUSE_CLOSE,
                TokenKind::TERNARY,
                TokenKind::SEPARATOR,
            },
        },

        {
            TokenKind::TIME,
            true,
            false,
            {

                TokenKind::MODIFIER,
                TokenKind::COMPARATOR,
                TokenKind::LOGICALOP,
                TokenKind::CLAUSE_CLOSE,
                TokenKind::SEPARATOR,
            },
        },

        {
            TokenKind::PATTERN,
            true,
            false,
            {

                TokenKind::MODIFIER,
                TokenKind::COMPARATOR,
                TokenKind::LOGICALOP,
                TokenKind::CLAUSE_CLOSE,
                TokenKind::SEPARATOR,
            },
        },

        {
            TokenKind::VARIABLE,
            true,
            false,
            {

                TokenKind::MODIFIER,
                TokenKind::COMPARATOR,
                TokenKind::LOGICALOP,
                TokenKind::CLAUSE_CLOSE,
                TokenKind::TERNARY,
                TokenKind::SEPARATOR,
            },
        },

        {
            TokenKind::MODIFIER,
            false,
            false,
            {

                TokenKind::PREFIX,
                TokenKind::NUMERIC,
                TokenKind::VARIABLE,
                TokenKind::FUNCTION,
                TokenKind::ACCESSOR,
                TokenKind::STRING,
                TokenKind::BOOLEAN,
                TokenKind::CLAUSE,
                TokenKind::CLAUSE_CLOSE,
            },
        },

        {
            TokenKind::COMPARATOR,
            false,
            false,
            {

                TokenKind::PREFIX,
                TokenKind::NUMERIC,
                TokenKind::BOOLEAN,
                TokenKind::VARIABLE,
                TokenKind::FUNCTION,
                TokenKind::ACCESSOR,
                TokenKind::STRING,
                TokenKind::TIME,
                TokenKind::CLAUSE,
                TokenKind::CLAUSE_CLOSE,
                TokenKind::PATTERN,
            },
        },

        {
            TokenKind::LOGICALOP,
            false,
            false,
            {

                TokenKind::PREFIX,
                TokenKind::NUMERIC,
                TokenKind::BOOLEAN,
                TokenKind::VARIABLE,
                TokenKind::FUNCTION,
                TokenKind::ACCESSOR,
                TokenKind::STRING,
                TokenKind::TIME,
                TokenKind::CLAUSE,
                TokenKind::CLAUSE_CLOSE,
            },
        },

        {
            TokenKind::PREFIX,
            false,
            false,
            {

                TokenKind::NUMERIC,
                TokenKind::BOOLEAN,
                TokenKind::VARIABLE,
                TokenKind::FUNCTION,
                TokenKind::ACCESSOR,
                TokenKind::CLAUSE,
                TokenKind::CLAUSE_CLOSE,
            },
        },

        {
            TokenKind::TERNARY,
            false,
            false,
            {

                TokenKind::PREFIX,
                TokenKind::NUMERIC,
                TokenKind::BOOLEAN,
                TokenKind::STRING,
                TokenKind::TIME,
                TokenKind::VARIABLE,
                TokenKind::FUNCTION,
                TokenKind::ACCESSOR,
                TokenKind::CLAUSE,
                TokenKind::SEPARATOR,
            },
        },

        {
            TokenKind::FUNCTION,
            false,
            false,
            {

                TokenKind::CLAUSE,
            },
        },

        {
            TokenKind::ACCESSOR,
            true,
            false,
            {

                TokenKind::CLAUSE,
                TokenKind::MODIFIER,
                TokenKind::COMPARATOR,
                TokenKind::LOGICALOP,
                TokenKind::CLAUSE_CLOSE,
                TokenKind::TERNARY,
                TokenKind::SEPARATOR,
            },
	    },

	    {
            TokenKind::SEPARATOR,
            false,
            true,
            {

                    TokenKind::PREFIX,
                    TokenKind::NUMERIC,
                    TokenKind::BOOLEAN,
                    TokenKind::STRING,
                    TokenKind::TIME,
                    TokenKind::VARIABLE,
                    TokenKind::FUNCTION,
                    TokenKind::ACCESSOR,
                    TokenKind::CLAUSE,
            },
        }
    };

    std::string GetTokenValueString(TokenAvaiableValue);
    int GetTokenValueInt(TokenAvaiableValue);
    float GetTokenValueFloat(TokenAvaiableValue);
    float GetTokenValueNumeric(TokenAvaiableValue);
    bool GetTokenValueBool(TokenAvaiableValue);
} // Cvaluate

#endif