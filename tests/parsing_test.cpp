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
#include <gtest/gtest.h>
#include <EvaluableExpression.h>

namespace {

/*
	Represents a test of parsing all tokens correctly from a string
*/
struct TokenParsingTest {
	std::string Name;     
	std::string Input;
    std::vector<Cvaluate::ExpressionToken> Expected;
    Cvaluate::ExpressionFunctionMap Functions;
    TokenParsingTest(std::string name, std::string input, 
        std::vector<Cvaluate::ExpressionToken> expected, Cvaluate::ExpressionFunctionMap functions = {}) :
        Name(name), Input(input), Expected(expected), Functions(functions) {};
};

/*
	Because std::function don't support operator==
    So, we can't directly compare to std::function variable in std::variant
*/
template<typename T>
void Assert_Token(Cvaluate::ExpressionToken& expected_token, Cvaluate::ExpressionToken& actual_token, TokenParsingTest& test_case) {

    if (auto expected_value = std::get_if<T>(&(expected_token.Value))) {
        if (auto actual_value = std::get_if<T>(&(actual_token.Value))) {
            ASSERT_EQ(*expected_value, *actual_value) << "Vaule don't match " << test_case.Name;
        } else {
            FAIL() << test_case.Name << "\n Value type don't match, expected: ";
        }
    }

}

void RunTokenParsingTest(std::vector<TokenParsingTest>& token_parsing_tests) {
    for (auto& test_case: token_parsing_tests) {
        auto expression = Cvaluate::EvaluableExpression(test_case.Input, test_case.Functions);
        
        auto actual_tokens = expression.Tokens();
        auto expected_tokens = test_case.Expected;

        ASSERT_EQ(actual_tokens.size(), expected_tokens.size());

        for (size_t i = 0; i < actual_tokens.size(); i++) {
            auto actual_token = actual_tokens[i];
            auto expected_token = expected_tokens[i];

            // Token kind
            ASSERT_EQ(actual_token.Kind, expected_token.Kind)
                << test_case.Name << "\nType don't match!";
            
            // Token value
            // TokenAvaiableData
            Assert_Token<Cvaluate::TokenAvaiableData>(actual_token, expected_token, test_case);
        }
    }
}

TEST(TestParse, TestConstantParsing) {
    std::vector<TokenParsingTest> token_parsing_tests = {
        {
			"Single numeric",
			"1",
			{
                {
                    Cvaluate::TokenKind::NUMERIC, 
                    1,
                },
            }
		},

        {
            "Single two-digit numeric",
            "50",
            {
                {
                    Cvaluate::TokenKind::NUMERIC,
                    50,
                },
            }
        },

        {
            "Zero",
            "0",
            {
                {
                    Cvaluate::TokenKind::NUMERIC,
                    0,
                },
            }
        },

        {
            "One digit hex",
            "0x1",
            {
                {
                    Cvaluate::TokenKind::NUMERIC,
                    1,
                },
            }
        },

        {
            "Two digit hex",
            "0x10",
            {
                {
                    Cvaluate::TokenKind::NUMERIC,
                    16,
                },
            }
        },

        {
            "Hex with lowercase",
            "0xabcdef",
            {
                {
                    Cvaluate::TokenKind::NUMERIC,
                    11259375,
                },
            }
        },

        {
            "Hex with uppercase",
            "0xABCDEF",
            {
                {
                    Cvaluate::TokenKind::NUMERIC,
                    11259375,
                },
            }
        },

        {
            "Single string",
            "'foo'",
            {
                {
                    Cvaluate::TokenKind::STRING,
                    std::string("foo"),
                },
            }
        },

        {
            "Single boolen",
            "true",
            {
                {
                    Cvaluate::TokenKind::BOOLEAN,
                    true,
                },
            }
        },

        {
            "Single false boolen",
            "false",
            {
                {
                    Cvaluate::TokenKind::BOOLEAN,
                    false,
                },
            }
        },

        {
            "Single large numeric",
            "1234567890",
            {
                {
                    Cvaluate::TokenKind::NUMERIC,
                    float(1234567890),
                },
            }
        },

        {
            "Single floating-point",
            "0.5",
            {
                {
                    Cvaluate::TokenKind::NUMERIC,
                    (float)0.5,
                },
            }
        },

        {
            "Single large floating point",
            "3.14567471",
            {
                {
                    Cvaluate::TokenKind::NUMERIC,
                    (float)3.14567471,
                },
            }
        },

        {
			"Parameterless function",
			"foo()",
			{
				{
					Cvaluate::TokenKind::FUNCTION,
                    Cvaluate::ExpressionFunction(nullptr),
				},
				{
					Cvaluate::TokenKind::CLAUSE,
                    std::string("("),
				},
				{
					Cvaluate::TokenKind::CLAUSE_CLOSE,
                    std::string(")"),
				},
			},
            {{"foo", nullptr}},
		},

        {
			"Single parameter function",
			"foo('bar')",
			{
				{
					Cvaluate::TokenKind::FUNCTION,
					Cvaluate::ExpressionFunction(nullptr),
				},
				{
					Cvaluate::TokenKind::CLAUSE,
                    std::string("("),
				},
				{
					Cvaluate::TokenKind::STRING,
					std::string("bar"),
				},
				{
					Cvaluate::TokenKind::CLAUSE_CLOSE,
                    std::string(")"),
				},
			},
            {{"foo", nullptr}},
		},

        {
		    "Multiple parameter function",
	        "foo('bar', 1.0)",
			{
				{
					Cvaluate::TokenKind::FUNCTION,
					Cvaluate::ExpressionFunction(nullptr),
				},
				{
					Cvaluate::TokenKind::CLAUSE,
                    std::string("("),
				},
				{
					Cvaluate::TokenKind::STRING,
					std::string("bar"),
				},
				{
					Cvaluate::TokenKind::SEPARATOR,
                    std::string(","),
				},
				{
					Cvaluate::TokenKind::NUMERIC,
					(float)1.0,
				},
				{
					Cvaluate::TokenKind::CLAUSE_CLOSE,
                    std::string(")"),
				},
			},
            {{"foo", nullptr}},
		},

        {
			"Nested function",
			"foo(foo('bar'), 1.0, foo(2.0))",
			{
				{
					Cvaluate::TokenKind::FUNCTION,
					Cvaluate::ExpressionFunction(nullptr),
				},
				{
					Cvaluate::TokenKind::CLAUSE,
                    std::string("("),
				},

				{
					Cvaluate::TokenKind::FUNCTION,
					Cvaluate::ExpressionFunction(nullptr),
				},
				{
					Cvaluate::TokenKind::CLAUSE,
                    std::string("("),
				},
				{
					Cvaluate::TokenKind::STRING,
					std::string("bar"),
				},
				{
					Cvaluate::TokenKind::CLAUSE_CLOSE,
                    std::string(")"),
				},
				{
					Cvaluate::TokenKind::SEPARATOR,
                    std::string(","),
				},

				{
					Cvaluate::TokenKind::NUMERIC,
					float(1.0),
				},

			    {
					Cvaluate::TokenKind::SEPARATOR,
                    std::string(","),
				},

				{
					Cvaluate::TokenKind::FUNCTION,
					Cvaluate::ExpressionFunction(nullptr),
				},
				{
					Cvaluate::TokenKind::CLAUSE,
                    std::string("("),
				},
				{
					Cvaluate::TokenKind::NUMERIC,
					(float)2.0,
				},
				{
					Cvaluate::TokenKind::CLAUSE_CLOSE,
                    std::string(")"),
				},

				{
					Cvaluate::TokenKind::CLAUSE_CLOSE,
                    std::string(")"),
				},
			},
            {{"foo", nullptr}},
		},

		{
			"Function with modifier afterwards (#28)",
			"foo() + 1",
			{
				{
					Cvaluate::TokenKind::FUNCTION,
					Cvaluate::ExpressionFunction(nullptr),
				},
				{
					Cvaluate::TokenKind::CLAUSE,
                    std::string("("),
				},
				{
					Cvaluate::TokenKind::CLAUSE_CLOSE,
                    std::string(")"),
				},
				{
					Cvaluate::TokenKind::MODIFIER,
					std::string("+"),
				},
				{
					Cvaluate::TokenKind::NUMERIC,
					(int)1,
				},
			},
            {{"foo", nullptr}},
		},

        {
			"Function with modifier afterwards and comparator",
			"(foo()-1) > 3",
			{
				{
					Cvaluate::TokenKind::CLAUSE,
                    std::string("("),
				},
				{
					Cvaluate::TokenKind::FUNCTION,
					Cvaluate::ExpressionFunction(nullptr),
				},
				{
					Cvaluate::TokenKind::CLAUSE,
                    std::string("("),
				},
				{
					Cvaluate::TokenKind::CLAUSE_CLOSE,
                    std::string(")"),
				},
				{
					Cvaluate::TokenKind::MODIFIER,
					std::string("-"),
				},
				{
					Cvaluate::TokenKind::NUMERIC,
					(int)1,
				},
				{
					Cvaluate::TokenKind::CLAUSE_CLOSE,
                    std::string(")"),
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string(">"),
				},
				{
					Cvaluate::TokenKind::NUMERIC,
					(int)3,
				},
			},
            {{"foo", nullptr}},
		},

		{
			"Double-quoted string added to square-brackted param (#59)",
			"\"a\" + [foo]",
			{
				{
					Cvaluate::TokenKind::STRING,
					std::string("a"),
				},
				{
					Cvaluate::TokenKind::MODIFIER,
					std::string("+"),
				},
				{
					Cvaluate::TokenKind::VARIABLE,
					std::string("foo"),
				},
			},
		},
		{
			"Accessor variable",
			"foo.Var",
			{
				{
					Cvaluate::TokenKind::ACCESSOR,
					std::vector<std::string>{"foo", "Var"},
				},
			},
		},
        // Don't support
		// TokenParsingTest{
		// 	"Accessor function",
		// 	"foo.Operation()",
		// 	{
		// 		{
		// 			Cvaluate::TokenKind::ACCESSOR,
		// 			std::vector<std::string>{"foo", "Operation"},
		// 		},
		// 		{
		// 			Cvaluate::TokenKind::CLAUSE,
        //             std::string("("),
		// 		},
		// 		{
		// 			Cvaluate::TokenKind::CLAUSE_CLOSE,
        //             std::string(")"),
		// 		},
		// 	},
		// },
    };

    RunTokenParsingTest(token_parsing_tests);
}

TEST(TestParse, TestLogicalOperatorParsing) {
    std::vector<TokenParsingTest> token_parsing_tests = {
		{

			"Boolean AND",
			"true && false",
			{
			    {
					Cvaluate::TokenKind::BOOLEAN,
					(bool)true,
				},
				{
					Cvaluate::TokenKind::LOGICALOP,
					std::string("&&"),
				},
				{
					Cvaluate::TokenKind::BOOLEAN,
					(bool)false,
				},
			},
		},

		{

			"Boolean OR",
			"true || false",
			{
				{
					Cvaluate::TokenKind::BOOLEAN,
					(bool)true,
				},
				{
					Cvaluate::TokenKind::LOGICALOP,
					std::string("||"),
				},
				{
					Cvaluate::TokenKind::BOOLEAN,
					(bool)false,
				},
			},
		},
		
        {

			"Multiple logical operators",
			"true || false && true",
			{
				{
					Cvaluate::TokenKind::BOOLEAN,
					(bool)true,
				},
				{
					Cvaluate::TokenKind::LOGICALOP,
					std::string("||"),
				},
				{
					Cvaluate::TokenKind::BOOLEAN,
					(bool)false,
				},
				{
					Cvaluate::TokenKind::LOGICALOP,
					std::string("&&"),
				},
				{
					Cvaluate::TokenKind::BOOLEAN,
					(bool)true,
				},
			},
		},
        
    };

    RunTokenParsingTest(token_parsing_tests);
}

TEST(TestParse, TestComparatorParsing) {
    std::vector<TokenParsingTest> token_parsing_tests = {
		{

			"Numeric EQ",
			"1 == 2",
			{
				{
					Cvaluate::TokenKind::NUMERIC,
					(int)1,
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string("=="),
				},
				{
					Cvaluate::TokenKind::NUMERIC,
					(int)2,
				},
			},
		},
		{

			"Numeric NEQ",
			"1 != 2",
			{
				{
					Cvaluate::TokenKind::NUMERIC,
					(int)1,
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string("!="),
				},
				{
					Cvaluate::TokenKind::NUMERIC,
					(int)2,
				},
			},
		},
		{

			"Numeric GT",
			"1 > 0",
			{
				{
					Cvaluate::TokenKind::NUMERIC,
					(int)1,
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string(">"),
				},
				{
					Cvaluate::TokenKind::NUMERIC,
					(int)0,
				},
			},
		},
		{

			"Numeric LT",
			"1 < 2",
			{
				{
					Cvaluate::TokenKind::NUMERIC,
					int(1),
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string("<"),
				},
				{
					Cvaluate::TokenKind::NUMERIC,
					int(2),
				},
			},
		},
		{

			"Numeric GTE",
			"1 >= 2",
			{
				{
					Cvaluate::TokenKind::NUMERIC,
					int(1),
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string(">="),
				},
				{
					Cvaluate::TokenKind::NUMERIC,
					int(2),
				},
			},
		},
		{

		    "Numeric LTE",
			"1 <= 2",
			{
				{
					Cvaluate::TokenKind::NUMERIC,
					int(1),
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string("<="),
				},
				{
					Cvaluate::TokenKind::NUMERIC,
					int(2),
				},
			},
		},
		TokenParsingTest{

			"String LT",
			"'ab.cd' < 'abc.def'",
			{
				{
					Cvaluate::TokenKind::STRING,
					std::string("ab.cd"),
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string("<"),
				},
				{
					Cvaluate::TokenKind::STRING,
					std::string("abc.def"),
				},
			},
		},
		{

			"String LTE",
			"'ab.cd' <= 'abc.def'",
			{
				{
					Cvaluate::TokenKind::STRING,
					std::string("ab.cd"),
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string("<="),
				},
				{
					Cvaluate::TokenKind::STRING,
					std::string("abc.def"),
				},
			},
		},
		{

			"String GT",
			"'ab.cd' > 'abc.def'",
			{
				{
					Cvaluate::TokenKind::STRING,
					std::string("ab.cd"),
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string(">"),
				},
				{
					Cvaluate::TokenKind::STRING,
					std::string("abc.def"),
				},
			},
		},
		{

			"String GTE",
			"'ab.cd' >= 'abc.def'",
			{
				{
					Cvaluate::TokenKind::STRING,
					std::string("ab.cd"),
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string(">="),
				},
				{
					Cvaluate::TokenKind::STRING,
					std::string("abc.def"),
				},
			},
		},
		// {

		// 	"String REQ",
		// 	"'foobar' =~ 'bar'",
		// 	{
		// 		{
		// 			Cvaluate::TokenKind::STRING,
		// 			std::string("foobar"),
		// 		},
		// 		{
		// 			Cvaluate::TokenKind::COMPARATOR,
		// 			std::string("=~"),
		// 		},

		// 		// it's not particularly clean to test for the contents of a pattern, (since it means modifying the harness below)
		// 		// so pattern contents are left untested.
		// 		ExpressionToken{
		// 			Cvaluate::TokenKind::PATTERN,
        //             std::string("");
		// 		},
		// 	},
		// },
		// TokenParsingTest{

		// 	Name:  "String NREQ",
		// 	Input: "'foobar' !~ 'bar'",
		// 	Expected: []ExpressionToken{
		// 		ExpressionToken{
		// 			Cvaluate::TokenKind::STRING,
		// 			Value: "foobar",
		// 		},
		// 		ExpressionToken{
		// 			Cvaluate::TokenKind::COMPARATOR,
		// 			Value: "!~",
		// 		},
		// 		ExpressionToken{
		// 			Kind: PATTERN,
		// 		},
		// 	},
		// },
		{

			"Comparator against modifier string additive (#22)",
			"'foo' == '+'",
			{
				{
					Cvaluate::TokenKind::STRING,
					std::string("foo"),
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string("=="),
				},
				{
					Cvaluate::TokenKind::STRING,
					std::string("+"),
				},
			},
		},
		{

			"Comparator against modifier string multiplicative (#22)",
			"'foo' == '/'",
			{
				{
					Cvaluate::TokenKind::STRING,
					std::string("foo"),
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string("=="),
				},
				{
					Cvaluate::TokenKind::STRING,
					std::string("/"),
				},
			},
		},
		{

			"Comparator against modifier string exponential (#22)",
			"'foo' == '**'",
			{
				{
					Cvaluate::TokenKind::STRING,
					std::string("foo"),
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string("=="),
				},
				{
					Cvaluate::TokenKind::STRING,
					std::string("**"),
				},
			},
		},
		{

			"Comparator against modifier string bitwise (#22)",
			"'foo' == '^'",
			{
				{
					Cvaluate::TokenKind::STRING,
					std::string("foo"),
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string("=="),
				},
				{
					Cvaluate::TokenKind::STRING,
					std::string("^"),
				},
			},
		},
		{

			"Comparator against modifier string shift (#22)",
			"'foo' == '>>'",
			{
				{
					Cvaluate::TokenKind::STRING,
					std::string("foo"),
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string("=="),
				},
				{
					Cvaluate::TokenKind::STRING,
					std::string(">>"),
				},
			},
		},
		TokenParsingTest{

			"Comparator against modifier string ternary (#22)",
			"'foo' == '?'",
			{
				{
					Cvaluate::TokenKind::STRING,
					std::string("foo"),
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string("=="),
				},
				{
					Cvaluate::TokenKind::STRING,
					std::string("?"),
				},
			},
		},
		{

			"Array membership lowercase",
			"'foo' in ('foo', 'bar')",
			{
				{
					Cvaluate::TokenKind::STRING,
					std::string("foo"),
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string("in"),
				},
				{
					Cvaluate::TokenKind::CLAUSE,
                    std::string("("),
				},
				{
					Cvaluate::TokenKind::STRING,
					std::string("foo"),
				},
				{
					Cvaluate::TokenKind::SEPARATOR,
                    std::string(","),
				},
				{
					Cvaluate::TokenKind::STRING,
				    std::string("bar"),
				},
				{
					Cvaluate::TokenKind::CLAUSE_CLOSE,
                    std::string(")"),
				},
			},
		},
		{

			"Array membership uppercase",
			"'foo' IN ('foo', 'bar')",
			{
				{
					Cvaluate::TokenKind::STRING,
					std::string("foo"),
				},
				{
					Cvaluate::TokenKind::COMPARATOR,
					std::string("in"),
				},
				{
					Cvaluate::TokenKind::CLAUSE,
                    std::string("("),
				},
				{
					Cvaluate::TokenKind::STRING,
					std::string("foo"),
				},
				{
					Cvaluate::TokenKind::SEPARATOR,
                    std::string(","),
				},
				{
					Cvaluate::TokenKind::STRING,
					std::string("bar"),
				},
				{
					Cvaluate::TokenKind::CLAUSE_CLOSE,
                    std::string(")"),
				},
			},
		},
        
    };

    RunTokenParsingTest(token_parsing_tests);
}

TEST(TestParse, TestPrefixParsing) {
    std::vector<TokenParsingTest> token_parsing_tests = {

		{

			"Sign prefix",
			"-1",
			{
				{
					Cvaluate::TokenKind::PREFIX,
					std::string("-"),
				},
				{
					Cvaluate::TokenKind::NUMERIC,
					int(1),
				},
			},
		},
		{

			"Sign prefix on variable",
			"-foo",
			{
				{
					Cvaluate::TokenKind::PREFIX,
					std::string("-"),
				},
				{
					Cvaluate::TokenKind::VARIABLE,
					std::string("foo"),
				},
			},
		},
		{

			"Boolean prefix",
			"!true",
			{
				{
					Cvaluate::TokenKind::PREFIX,
					std::string("!"),
				},
				{
					Cvaluate::TokenKind::BOOLEAN,
					bool(true),
				},
			},
		},
		{

			"Boolean prefix on variable",
			"!foo",
			{
				{
					Cvaluate::TokenKind::PREFIX,
					std::string("!"),
				},
				{
					Cvaluate::TokenKind::VARIABLE,
					std::string("foo"),
				},
			},
		},
		{

			"Bitwise not prefix",
			"~1",
			{
				{
					Cvaluate::TokenKind::PREFIX,
					std::string("~"),
				},
				{
					Cvaluate::TokenKind::NUMERIC,
					int(1),
				},
			},
		},
		{

			"Bitwise not prefix on variable",
			"~foo",
			{
				{
					Cvaluate::TokenKind::PREFIX,
					std::string("~"),
				},
				{
					Cvaluate::TokenKind::VARIABLE,
					std::string("foo"),
				},
			},
		},

    };

    RunTokenParsingTest(token_parsing_tests);
}

} // namespace