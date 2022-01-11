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
struct TokenEvaluationTest {
	std::string Name;     
	std::string Input;
    Cvaluate::ExpressionFunctionMap Functions;
    Cvaluate::TokenAvaiableData Expected;
    Cvaluate::Parameters Parameters;
    TokenEvaluationTest(
        std::string name,
        std::string input,
        Cvaluate::TokenAvaiableData expected,
        Cvaluate::ExpressionFunctionMap functions = {},
        Cvaluate::Parameters parameters = {}
    ) : Name(name), Input(input), Expected(expected), Functions(functions), Parameters(parameters) {};
};

Cvaluate::TokenAvaiableData dummyParameterInstance = {
        {"String", "string!"},
        {"Int", 101},
        {"BoolFalse", false},
        {"Nested", 
            {
                {"Funk", "funkalicious"},
            }
        }
    };

Cvaluate::Parameters fooParameter = {
	{"foo", dummyParameterInstance}
};

void Assert_Value(Cvaluate::TokenAvaiableData& expected, Cvaluate::TokenAvaiableData& actual, TokenEvaluationTest& test_case) {
    
    ASSERT_EQ(expected, actual) << "Vaule don't equal\n test case: " << test_case.Name;

}

void RunEvaluationTests(std::vector<TokenEvaluationTest>& token_evaluation_tests) {
    for (auto& test_case: token_evaluation_tests) {
        auto expression = Cvaluate::EvaluableExpression(test_case.Input, test_case.Functions);

        Cvaluate::Parameters parameters = test_case.Parameters;
        auto result = expression.Evaluate(parameters);

        Assert_Value(test_case.Expected, result, test_case);
    }
}

TEST(TestEvaluation, TestNoParameterEvaluation) {
    std::vector<TokenEvaluationTest> token_evaluation_tests = {
        // Basic Operator
        {
			"Single PLUS",
			"51 + 49",
            (float)100,
		},
        {

			"Single MINUS",
			"100 - 51",
			(float)49,
		},
        {

			"Single MULTIPLY",
			"5 * 20",
			(float)100,
		},
		{

			"Single DIVIDE",
			"100 / 20",
			(float)5,
		},
		{

			"Single even MODULUS",
			"100 % 2",
			(float)0,
		},
        {

			"Single odd MODULUS",
			"101 % 2",
			(float)1,
		},
        {

			"Single EXPONENT",
			"10 ** 2",
			float(100),
		},
        {

			"Compound PLUS",
			"20 + 30 + 50",
			float(100),
		},
        {

			"Mutiple operators",
			"20 * 5 - 49",
			float(51),
		},
        {

			"Parenthesis usage",
			"100 - (5 * 10)",
		    float(50),
		},

        // Bool
        // This cast can't build correct stage
        {

			"Implicit boolean",
			"2 > 1",
		    true,
		},
        {

			"Compound boolean",
			"5 < 10 && 1 < 5",
			true,
		},
        {
			"Evaluated true && false operation (for issue #8)",
			"1 > 10 && 11 > 10",
			false,
		},
	    {

			"Evaluated true && false operation (for issue #8)",
			"true == true && false == true",
			false,
		},
        {

			"Parenthesis boolean",
			"10 < 50 && (1 != 2 && 1 > 0)",
			true,
		},
		{

			"Comparison of string constants",
			"'foo' == 'foo'",
			true,
		},
        {

			"NEQ comparison of string constants",
			"'foo' != 'bar'",
			true,
		},

        // Avance calculate
        {

			"Multiplicative/additive order",
			"5 + 10 * 2",
			float(25),
		},
        {

			"Multiple constant multiplications",
			"10 * 10 * 10",
			float(1000),
		},
        {

			"Multiple adds/multiplications",
			"10 * 10 * 10 + 1 * 10 * 10",
			float(1100),
		},
        // TODO: Same priority
        // RecordStage solve this problem
		{

			"Modulus precedence", // left -> right calculate
			"1 + 101 % 2 * 5",
			float(6),
		},
		{

			"Exponent precedence",
			"1 + 5 ** 3 % 2 * 5",
			float(6),
		},

        // Bit shift
        // {

		// 	"Bit shift precedence",
		// 	"50 << 1 & 90",
		// 	float(64),
		// },
		// {

		// 	"Bit shift precedence",
		// 	"90 & 50 << 1",
		// 	64,
		// },
		// {

		// 	"Bit shift precedence amongst non-bitwise",
		// 	"90 + 50 << 1 * 5",
		// 	float(4480),
		// },
		{
			"Order of non-commutative same-precedence operators (additive)",
			"1 - 2 - 4 - 8",
			float(-13.0),
		},
		{
			"Order of non-commutative same-precedence operators (multiplicative)",
			"1 * 4 / 2 * 8",
			float(16.0),
		},

		// EvaluationTest{
		// 	Name:     "Null coalesce precedence",
		// 	Input:    "true ?? true ? 100 + 200 : 400",
		// 	Expected: 300.0,
		// },
		// EvaluationTest{

		// 	Name:     "Identical date equivalence",
		// 	Input:    "'2014-01-02 14:12:22' == '2014-01-02 14:12:22'",
		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:     "Positive date GT",
		// 	Input:    "'2014-01-02 14:12:22' > '2014-01-02 12:12:22'",
		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:     "Negative date GT",
		// 	Input:    "'2014-01-02 14:12:22' > '2014-01-02 16:12:22'",
		// 	Expected: false,
		// },
		// EvaluationTest{

		// 	Name:     "Positive date GTE",
		// 	Input:    "'2014-01-02 14:12:22' >= '2014-01-02 12:12:22'",
		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:     "Negative date GTE",
		// 	Input:    "'2014-01-02 14:12:22' >= '2014-01-02 16:12:22'",
		// 	Expected: false,
		// },
		// EvaluationTest{

		// 	Name:     "Positive date LT",
		// 	Input:    "'2014-01-02 14:12:22' < '2014-01-02 16:12:22'",
		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:     "Negative date LT",
		// 	Input:    "'2014-01-02 14:12:22' < '2014-01-02 11:12:22'",
		// 	Expected: false,
		// },
		// EvaluationTest{

		// 	Name:     "Positive date LTE",
		// 	Input:    "'2014-01-02 09:12:22' <= '2014-01-02 12:12:22'",
		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:     "Negative date LTE",
		// 	Input:    "'2014-01-02 14:12:22' <= '2014-01-02 11:12:22'",
		// 	Expected: false,
		// },
		{

			"Sign prefix comparison",
			"-1 < 0",
			true,
		},
		{

			"Lexicographic LT",
			"'ab' < 'abc'",
			true,
		},
		{

			"Lexicographic LTE",
			"'ab' <= 'abc'",
		    true,
		},
		{

			"Lexicographic GT",
			"'aba' > 'abc'",
			false,
		},
		{

			"Lexicographic GTE",
			"'aba' >= 'abc'",
			false,
		},
		{

			"Boolean sign prefix comparison",
			"!true == false",
			true,
		},
		{

			"Inversion of clause",
			"!(10 < 0)",
			true,
		},
		{

			"Negation after modifier",
			"10 * -10",
			float(-100.0),
		},
		// EvaluationTest{

		// 	Name:     "Ternary with single boolean",
		// 	Input:    "true ? 10",
		// 	Expected: 10.0,
		// },
		// EvaluationTest{

		// 	Name:     "Ternary nil with single boolean",
		// 	Input:    "false ? 10",
		// 	Expected: nil,
		// },
		// EvaluationTest{

		// 	Name:     "Ternary with comparator boolean",
		// 	Input:    "10 > 5 ? 35.50",
		// 	Expected: 35.50,
		// },
		// EvaluationTest{

		// 	Name:     "Ternary nil with comparator boolean",
		// 	Input:    "1 > 5 ? 35.50",
		// 	Expected: nil,
		// },
		// EvaluationTest{

		// 	Name:     "Ternary with parentheses",
		// 	Input:    "(5 * (15 - 5)) > 5 ? 35.50",
		// 	Expected: 35.50,
		// },
		// EvaluationTest{

		// 	Name:     "Ternary precedence",
		// 	Input:    "true ? 35.50 > 10",
		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:     "Ternary-else",
		// 	Input:    "false ? 35.50 : 50",
		// 	Expected: 50.0,
		// },
		// EvaluationTest{

		// 	Name:     "Ternary-else inside clause",
		// 	Input:    "(false ? 5 : 35.50) > 10",
		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:     "Ternary-else (true-case) inside clause",
		// 	Input:    "(true ? 1 : 5) < 10",
		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:     "Ternary-else before comparator (negative case)",
		// 	Input:    "true ? 1 : 5 > 10",
		// 	Expected: 1.0,
		// },
		// EvaluationTest{

		// 	Name:     "Nested ternaries (#32)",
		// 	Input:    "(2 == 2) ? 1 : (true ? 2 : 3)",
		// 	Expected: 1.0,
		// },
		// EvaluationTest{

		// 	Name:     "Nested ternaries, right case (#32)",
		// 	Input:    "false ? 1 : (true ? 2 : 3)",
		// 	Expected: 2.0,
		// },
		// EvaluationTest{

		// 	Name:     "Doubly-nested ternaries (#32)",
		// 	Input:    "true ? (false ? 1 : (false ? 2 : 3)) : (false ? 4 : 5)",
		// 	Expected: 3.0,
		// },
		{

			"String to string concat",
			"'foo' + 'bar' == 'foobar'",
			true,
		},
		{

			"String to float64 concat",
			"'foo' + 123 == 'foo123'",
			true,
		},
		{

			"Float64 to string concat",
			"123 + 'bar' == '123bar'",
			true,
		},
		// {

		// 	Name:     "String to date concat",
		// 	Input:    "'foo' + '02/05/1970' == 'foobar'",
		// 	Expected: false,
		// },
		{

			"String to bool concat",
			"'foo' + true == 'footrue'",
			true,
		},
		{

			"Bool to string concat",
			"true + 'bar' == 'truebar'",
			true,
		},
		// EvaluationTest{

		// 	Name:     "Null coalesce left",
		// 	Input:    "1 ?? 2",
		// 	Expected: 1.0,
		// },
		// EvaluationTest{

		// 	Name:     "Array membership literals",
		// 	Input:    "1 in (1, 2, 3)",
		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:     "Array membership literal with inversion",
		// 	Input:    "!(1 in (1, 2, 3))",
		// 	Expected: false,
		// },
		{

			"Logical operator reordering (#30)",
			"(true && true) || (true && false)",
			true,
		},
		{

			"Logical operator reordering without parens (#30)",
			"true && true || true && false",
			true,
		},
		{

			"Logical operator reordering with multiple OR (#30)",
			"false || true && true || false",
			true,
		},
		{

			"Left-side multiple consecutive (should be reordered) operators",
			"(10 * 10 * 10) > 10",
			true,
		},
		{

			"Three-part non-paren logical op reordering (#44)",
			"false && true || true",
			true,
		},
		{

			"Three-part non-paren logical op reordering (#44), second one",
			"true || false && true",
			true,
		},
		{

			"Logical operator reordering without parens (#45)",
			"true && true || false && false",
			true,
		},
		{

			"Single function",
			"foo()",
            true,
			{
				{
                    "foo",
                    [] (Cvaluate::TokenAvaiableData data) -> Cvaluate::TokenAvaiableData {
                        return true;
                    }
                }
			},
		},
		{

		    "Function with argument",
			"passthrough(1)",
            float(1.0),
			{
				{
                    "passthrough", 
                    [] (Cvaluate::TokenAvaiableData data) -> Cvaluate::TokenAvaiableData {
                        return data;
                    }
                }
			},
		},

		{

			"Function with arguments",
			"passthrough(1, 2)",
            float(3),
			{
				{
                    "passthrough",
                    [] (Cvaluate::TokenAvaiableData input) -> Cvaluate::TokenAvaiableData {
                        return input[0].get<int>() + input[1].get<int>();
                    }
				},
			},
		},
		// EvaluationTest{

		// 	Name:  "Nested function with precedence",
		// 	Input: "sum(1, sum(2, 3), 2 + 2, true ? 4 : 5)",
		// 	Functions: map[string]ExpressionFunction{
		// 		"sum": func(arguments ...interface{}) (interface{}, error) {

		// 			sum := 0.0
		// 			for _, v := range arguments {
		// 				sum += v.(float64)
		// 			}
		// 			return sum, nil
		// 		},
		// 	},

		// 	Expected: 14.0,
		// },
		{

			"Empty function and modifier, compared",
			"numeric()-1 > 0",
			true,
			{
				{
                    "numeric",
                    [] (Cvaluate::TokenAvaiableData) -> Cvaluate::TokenAvaiableData {
                        return 2;
                    }
				},
			},
		},
		{

			"Empty function comparator",
			"numeric() > 0",
            true,
		    {
				{
                    "numeric",
                    [] (Cvaluate::TokenAvaiableData data) -> Cvaluate::TokenAvaiableData {
                        return 2;
                    }
				},
			},
		},
		{

			"Empty function logical operator",
			"success() && !false",
            true,
			{
				{
                    "success",
                    [] (Cvaluate::TokenAvaiableData data) -> Cvaluate::TokenAvaiableData {
                        return true;
                    }
				},
			},
		},
		// EvaluationTest{

		// 	Name:  "Empty function ternary",
		// 	Input: "nope() ? 1 : 2.0",
		// 	Functions: map[string]ExpressionFunction{
		// 		"nope": func(arguments ...interface{}) (interface{}, error) {
		// 			return false, nil
		// 		},
		// 	},

		// 	Expected: 2.0,
		// },
		// EvaluationTest{

		// 	Name:  "Empty function null coalesce",
		// 	Input: "null() ?? 2",
		// 	Functions: map[string]ExpressionFunction{
		// 		"null": func(arguments ...interface{}) (interface{}, error) {
		// 			return nil, nil
		// 		},
		// 	},

		// 	Expected: 2.0,
		// },
		{

			"Empty function with prefix",
			"-ten()",
            float(-10),
			{
				{
                    "ten",
                    [] (Cvaluate::TokenAvaiableData data) -> Cvaluate::TokenAvaiableData {
                        return float(10);
                    }
				},
			},
		},
        // Same Precence Case
		// {

		//     "Empty function as part of chain",
		// 	"10 - numeric() - 2",
        //     float(3),
		// 	{
		// 		{
        //             "numeric",
        //             [] (Cvaluate::TokenAvaiableData data) -> Cvaluate::TokenAvaiableData {
        //                 return float(5);
        //             }
		// 		},
		// 	},
		// },
		// EvaluationTest{

		// 	Name:  "Empty function near separator",
		// 	Input: "10 in (1, 2, 3, ten(), 8)",
		// 	Functions: map[string]ExpressionFunction{
		// 		"ten": func(arguments ...interface{}) (interface{}, error) {
		// 			return 10.0, nil
		// 		},
		// 	},

		// 	Expected: true,
		// },
		{

			"Enclosed empty function with modifier and comparator (#28)",
			"(ten() - 1) > 3",
            true,
			{
				{
                    "ten",
                    [] (Cvaluate::TokenAvaiableData data) -> Cvaluate::TokenAvaiableData {
                        return 10;
                    }
				},
			},
		},
		// EvaluationTest{

		// 	Name:  "Ternary/Java EL ambiguity",
		// 	Input: "false ? foo:length()",
		// 	Functions: map[string]ExpressionFunction{
		// 		"length": func(arguments ...interface{}) (interface{}, error) {
		// 			return 1.0, nil
		// 		},
		// 	},
		// 	Expected: 1.0,
		// },

    };

    RunEvaluationTests(token_evaluation_tests);
}

TEST(TestEvaluation, TestParameterizedEvaluation) {
    std::vector<TokenEvaluationTest> token_evaluation_tests = {
        {

			"Single parameter modified by constant",
			"foo + 2",
            float(4),
            {},
            {
                {
                    "foo",
                    float(2)
                }
            }
		},
		{

			"Single parameter modified by variable",
			"foo * bar",
            float(10),
            {},
			{

				{
					"foo",
					float(5.0),
				},
				{
					"bar",
					float(2.0),
				},
			},
		},
		{

			"Multiple multiplications of the same parameter",
			"foo * foo * foo",
            float(1000.0),
            {},
			{

				{
					"foo",
					float(10.0),
				},
			},
			
		},
		{

			"Multiple additions of the same parameter",
			"foo + foo + foo",
            float(30.0),
            {},
			{

			    {
					"foo",
					float(10.0),
				},
			},
		},
		{

			"Parameter name sensitivity",
			"foo + FoO + FOO",
            float(14.0),
            {},
			{

				{
					"foo",
				    float(8.0),
				},
				{
					"FoO",
					float(4.0),
				},
				{
					"FOO",
					float(2.0),
				},
			},

		},
        {

			"Sign prefix comparison against prefixed variable",
			"-1 < -foo",
            true,
            {},
			{

				{
					"foo",
					-8.0,
				},
			},
		},
		{

			"Fixed-point parameter",
			"foo > 1",
            true,
            {},
			{

				{
					"foo",
					2,
				},
			},
		},
		{

            "Modifier after closing clause",
            "(2 + 2) + 2 == 6",
			true,
		},
		{

            "Comparator after closing clause",
            "(2 + 2) >= 4",
			true,
		},
		{

			"Two-boolean logical operation (for issue #8)",
			"(foo == true) || (bar == true)",
            true,
            {},
			{
				{
					"foo",
					true,
				},
				{
					"bar",
					false,
				},
			},
		},
		{

			"Two-variable integer logical operation (for issue #8)",
			"foo > 10 && bar > 10",
            false,
            {},
			{
				{
					"foo",
					1,
				},
				{
					"bar",
					11,
				},
			},
			
		},
		// {

		// 	"Regex against right-hand parameter",
		// 	"'foobar' =~ foo",
		// 	{
		// 		{
		// 			"foo",
		// 			"obar",
		// 		},
		// 	},
		// 	true,
		// },
		// {

		// 	"Not-regex against right-hand parameter",
		// 	"'foobar' !~ foo",
		// 	{
		// 		{
		// 			"foo",
		// 			"baz",
		// 		},
		// 	},
		// 	true,
		// },
		// {

		// 	"Regex against two parameters",
		// 	"foo =~ bar",
		// 	{
		// 		{
		// 			"foo",
		// 			"foobar",
		// 		},
		// 		{
		// 			"bar",
		// 			"oba",
		// 		},
		// 	},
		// 	true,
		// },
		// {

		// 	"Not-regex against two parameters",
		// 	"foo !~ bar",
		// 	{
		// 		{
		// 			"foo",
		// 			"foobar",
		// 		},
		// 		{
		// 			"bar",
		// 			"baz",
		// 		},
		// 	},
		// 	true,
		// },
		// {

		// 	"Pre-compiled regex",
		// 	"foo =~ bar",
		// 	{
		// 		{
		// 			"foo",
		// 			"foobar",
		// 		},
		// 		{
		// 			"bar",
		// 			regexp.MustCompile("[fF][oO]+"),
		// 		},
		// 	},
		// 	true,
		// },
		// {

		// 	"Pre-compiled not-regex",
		// 	"foo !~ bar",
		// 	{
		// 		{
		// 			"foo",
		// 			"foobar",
		// 		},
		// 		{
		// 			"bar",
		// 			regexp.MustCompile("[fF][oO]+"),
		// 		},
		// 	},
		// 	false,
		// },
		// {

		// 	"Single boolean parameter",
		// 	"commission ? 10",
		// 	{
		// 		{
		// 			"commission",
		// 			true,
		// 		},
		// 	},
		// 	10.0,
		// },
		// {

		// 	"True comparator with a parameter",
		// 	"partner == 'amazon' ? 10",
		// 	{
		// 		{
		// 			"partner",
		// 			"amazon",
		// 		},
		// 	},
		// 	10.0,
		// },
		// {

		// 	"False comparator with a parameter",
		// 	"partner == 'amazon' ? 10",
		// 	{
		// 		{
		// 			"partner",
		// 			"ebay",
		// 		},
		// 	},
		// 	nil,
		// },
		// {

		// 	"True comparator with multiple parameters",
		// 	"theft && period == 24 ? 60",
		// 	{
		// 		{
		// 			"theft",
		// 			true,
		// 		},
		// 		{
		// 			"period",
		// 			24,
		// 		},
		// 	},
		// 	60.0,
		// },
		// {

		// 	"False comparator with multiple parameters",
		// 	"theft && period == 24 ? 60",
		// 	{
		// 		{
		// 			"theft",
		// 			false,
		// 		},
		// 		{
		// 			"period",
		// 			24,
		// 		},
		// 	},
		// 	nil,
		// },
		{

			"String concat with single string parameter",
			"foo + 'bar'",
            "bazbar",
            {},
			{
				{
					"foo",
					"baz",
				},
			},
			
		},
		{

			"String concat with multiple string parameter",
			"foo + bar",
            "bazquux",
            {},
			{
				{
					"foo",
					"baz",
				},
				{
					"bar",
					"quux",
				},
			},
			
		},
		{

			"String concat with float parameter",
			"foo + bar",
            "baz123",
            {},
			{
				{
					"foo",
					"baz",
				},
				{
					"bar",
					int(123),
				},
			},
		},
		{

			"Mixed multiple string concat",
			"foo + 123 + 'bar' + true",
            "baz123bartrue",
            {},
			{
				{
					"foo",
					"baz",
				},
			},
		},
		{

			"Integer width spectrum",
			"uint8 + uint16 + uint32 + uint64 + int8 + int16 + int32 + int64",
            float(0.0),
            {},
			{
				{
					"uint8",
					(uint8_t)0,
				},
				{
					"uint16",
					(uint16_t)0,
				},
				{
					"uint32",
					(uint32_t)0,
				},
				{
					"uint64",
					(uint64_t)0,
				},
				{
					"int8",
					0,
				},
				{
					"int16",
					0,
				},
				{
					"int32",
					0,
				},
				{
					"int64",
					0,
				},
			},
		},
		{

			"Floats",
			"float32 + float64",
			float(0.0),
            {},
			{
				{
					"float32",
					float(0.0),
				},
				{
					"float64",
					float(0.0),
				},
			},
		},
		// {

		// 	"Null coalesce right",
		// 	"foo ?? 1.0",
		// 	{
		// 		{
		// 			"foo",
		// 			nil,
		// 		},
		// 	},
		// 	1.0,
		// },
		{

			"Multiple comparator/logical operators (#30)",
			"(foo >= 2887057408 && foo <= 2887122943) || (foo >= 168100864 && foo <= 168118271)",
			true,
            {},
			{
				{
					"foo",
					float(2887057409),
				},
			},
		},
		{

			"Multiple comparator/logical operators, opposite order (#30)",
			"(foo >= 168100864 && foo <= 168118271) || (foo >= 2887057408 && foo <= 2887122943)",
            true,
            {},
			{
				{
					"foo",
					float(2887057409),
				},
			},
		},
		{

			"Multiple comparator/logical operators, small value (#30)",
			"(foo >= 2887057408 && foo <= 2887122943) || (foo >= 168100864 && foo <= 168118271)",
            true,
            {},
			{
				{
					"foo",
					168100865,
				},
			},	
		},
		{

			"Multiple comparator/logical operators, small value, opposite order (#30)",
			"(foo >= 168100864 && foo <= 168118271) || (foo >= 2887057408 && foo <= 2887122943)",
            true,
            {},
			{
				{
					"foo",
					168100865,
				},
			},
		},
		{

			"Incomparable array equality comparison",
			"arr == arr",
            true,
            {},
			{
				{
					"arr",
					{0, 0, 0}
				},
			},
		},
		{

			"Incomparable array not-equality comparison",
			"arr != arr",
            false,
            {},
			{
				{
					"arr",
					{0, 0, 0},
				},
			},
		},
		{

			"Mixed function and parameters",
			"sum(1.2, amount) + name",
			"2awesome",
		    {
                {
                    "sum",
                    [] (Cvaluate::TokenAvaiableData data) -> Cvaluate::TokenAvaiableData {
                        float ans;
                        for (auto& x: data) {
                            if (x.is_number_float()) {
                                ans += x.get<float>();
                            }
                        }

                        return ans;
                    }
                }
            },
			{
				{
					"amount",
					0.8,
				},
				{
					"name",
					"awesome",
				},
			},
		},
		// {

		// 	"Short-circuit OR",
		// 	"true || fail()",
		// 	Functions: map[string]ExpressionFunction{
		// 		"fail": func(arguments ...interface{}) (interface{}, error) {
		// 			return nil, errors.New("Did not short-circuit")
		// 		},
		// 	},
		// 	true,
		// },
		// {

		// 	"Short-circuit AND",
		// 	"false && fail()",
		// 	Functions: map[string]ExpressionFunction{
		// 		"fail": func(arguments ...interface{}) (interface{}, error) {
		// 			return nil, errors.New("Did not short-circuit")
		// 		},
		// 	},
		// 	false,
		// },
		// {

		// 	"Short-circuit ternary",
		// 	"true ? 1 : fail()",
		// 	Functions: map[string]ExpressionFunction{
		// 		"fail": func(arguments ...interface{}) (interface{}, error) {
		// 			return nil, errors.New("Did not short-circuit")
		// 		},
		// 	},
		// 	1.0,
		// },
		// {

		// 	"Short-circuit coalesce",
		// 	"'foo' ?? fail()",
		// 	Functions: map[string]ExpressionFunction{
		// 		"fail": func(arguments ...interface{}) (interface{}, error) {
		// 			return nil, errors.New("Did not short-circuit")
		// 		},
		// 	},
		// 	"foo",
		// },
		{

			"Simple parameter call",
			"foo.String",
            fooParameter["foo"]["String"],
            {},
			fooParameter,
		},
		// {

		// 	"Simple parameter function call",
		// 	"foo.Func()",
		// 	{fooParameter},
		// 	"funk",
		// },
		// {

		// 	"Simple parameter call from pointer",
		// 	"fooptr.String",
		// 	{fooPtrParameter},
		// 	fooParameter.Value.(dummyParameter).String,
		// },
		// {

		// 	"Simple parameter function call from pointer",
		// 	"fooptr.Func()",
		// 	{fooPtrParameter},
		// 	"funk",
		// },
		// {

		// 	"Simple parameter function call from pointer",
		// 	"fooptr.Func3()",
		// 	{fooPtrParameter},
		// 	"fronk",
		// },
		{

			"Simple parameter call",
			"foo.String == 'hi'",
            false,
            {},
			fooParameter,
		},
		{

			"Simple parameter call with modifier",
			"foo.String + 'hi'",
			fooParameter["foo"]["String"].get<std::string>() + "hi",
			{},
            fooParameter,
		},
		// {

		// 	"Simple parameter function call, two-arg return",
		// 	"foo.Func2()",
		// 	{fooParameter},
		// 	"frink",
		// },
		// {

		// 	"Parameter function call with all argument types",
		// 	"foo.TestArgs(\"hello\", 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1.0, 2.0, true)",
		// 	{fooParameter},
		// 	"hello: 33",
		// },

		// {

		// 	"Simple parameter function call, one arg",
		// 	"foo.FuncArgStr('boop')",
		// 	{fooParameter},
		// 	"boop",
		// },
		// {

		// 	"Simple parameter function call, one arg",
		// 	"foo.FuncArgStr('boop') + 'hi'",
		// 	{fooParameter},
		// 	"boophi",
		// },
		// {

		// 	"Nested parameter function call",
		// 	"foo.Nested.Dunk('boop')",
		// 	{fooParameter},
		// 	"boopdunk",
		// },
		{

			"Nested parameter call",
			"foo.Nested.Funk",
            "funkalicious",
            {},
			fooParameter,
		},
		{

			"Parameter call with + modifier",
			"1 + foo.Int",
            float(102.0),
            {},
			fooParameter,
		},
		{

			"Parameter string call with + modifier",
			"'woop' + (foo.String)",
			"woopstring!",
            {},
            fooParameter
		},
		{

			"Parameter call with && operator",
			"true && foo.BoolFalse",
            false,
			{},
            fooParameter
		},
		// {

		// 	"Null coalesce nested parameter",
		// 	"foo.Nil ?? false",
		// 	{fooParameter},
		// 	false,
		// },
    };

    RunEvaluationTests(token_evaluation_tests);
}

} // namespace