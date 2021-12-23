#include <gtest/gtest.h>
#include <EvaluableExpression.h>

namespace {

/*
	Represents a test of parsing all tokens correctly from a string
*/
struct TokenParsingTest {
	std::string Name;     
	std::string Input;
    Cvaluate::ExpressionFunctionMap Functions;
    std::vector<Cvaluate::ExpressionToken> Expected;
    TokenParsingTest(std::string name, std::string input, 
        std::vector<Cvaluate::ExpressionToken> expected, Cvaluate::ExpressionFunctionMap functions = {}) :
        Name(name), Input(input), Expected(expected), Functions(functions) {};
};

/*
	Because std::function don't support operator==
    So, we can't directly compare to std::function variable in std::variant
*/
template<typename T>
void Assert_Token(Cvaluate::ExpressionToken& expected_token, Cvaluate::ExpressionToken& actual_token) {

    if (auto expected_value = std::get_if<T>(&(expected_token.Value))) {
        if (auto actual_value = std::get_if<T>(&(actual_token.Value))) {
            ASSERT_TRUE(*expected_value == *actual_value);
        } else {
            ASSERT_TRUE(false);
        }
    }

}

void RunTokenParsingTest(std::vector<TokenParsingTest>& token_parsing_tests) {
    for (auto& test_case: token_parsing_tests) {
        auto expression = Cvaluate::EvaluableExpression(test_case.Input, test_case.Functions);
        
        auto actual_tokens = expression.Tokens();
        auto expected_tokens = test_case.Expected;

        ASSERT_EQ(actual_tokens.size(), expected_tokens.size());

        for (auto i = 0; i < actual_tokens.size(); i++) {
            auto actual_token = actual_tokens[i];
            auto expected_token = expected_tokens[i];

            // Token kind
            ASSERT_EQ(actual_token.Kind, expected_token.Kind);
            
            // Token value
            // int
            Assert_Token<int>(actual_token, expected_token);
            // boolen
            Assert_Token<bool>(actual_token, expected_token);
            // float
            Assert_Token<float>(actual_token, expected_token);
            // string
            Assert_Token<std::string>(actual_token, expected_token);
            // string vector
            Assert_Token<std::vector<std::string>>(actual_token, expected_token);
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
            "foo",
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
                    1234567890,
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
		TokenParsingTest{
			"Accessor function",
			"foo.Operation()",
			{
				{
					Cvaluate::TokenKind::ACCESSOR,
					std::vector<std::string>{"foo", "Operation"},
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
		},
    };

    RunTokenParsingTest(token_parsing_tests);
}

} // namespace