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
    Cvaluate::TokenAvaiableValue Expected;
    TokenEvaluationTest(std::string name, std::string input, 
        Cvaluate::TokenAvaiableValue expected, Cvaluate::ExpressionFunctionMap functions = {}) :
        Name(name), Input(input), Expected(expected), Functions(functions) {};
};

template<typename T>
void Assert_Value(Cvaluate::TokenAvaiableValue& expected, Cvaluate::TokenAvaiableValue& actual, TokenEvaluationTest& test_case) {

    if (auto expected_value = std::get_if<T>(&(expected))) {
        if (auto actual_value = std::get_if<T>(&(actual))) {
            ASSERT_EQ(*expected_value, *actual_value) << "Vaule don't equal\n test case:" << test_case.Name;
        } else {
            FAIL() << test_case.Name << "\n Value type don't match.";
        }
    }
}

void RunEvaluationTests(std::vector<TokenEvaluationTest>& token_evaluation_tests) {
    for (auto& test_case: token_evaluation_tests) {
        auto expression = Cvaluate::EvaluableExpression(test_case.Input, test_case.Functions);

        auto result = expression.Evaluate({});

        Assert_Value<int>(test_case.Expected, result, test_case);
        Assert_Value<float>(test_case.Expected, result, test_case);
        Assert_Value<bool>(test_case.Expected, result, test_case);
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
		// {

		// 	"Modulus precedence", left -> right calculate
		// 	"1 + 101 % 2 * 5",
		// 	float(6),
		// },
		// {

		// 	"Exponent precedence",
		// 	"1 + 5 ** 3 % 2 * 5",
		// 	float(6),
		// },

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
		// {
		// 	"Order of non-commutative same-precedence operators (additive)",
		// 	"1 - 2 - 4 - 8",
		// 	float(-13.0),
		// },
		// {
		// 	"Order of non-commutative same-precedence operators (multiplicative)",
		// 	"1 * 4 / 2 * 8",
		// 	float(16.0),
		// },

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
		// EvaluationTest{

		// 	Name:     "Sign prefix comparison",
		// 	Input:    "-1 < 0",
		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:     "Lexicographic LT",
		// 	Input:    "'ab' < 'abc'",
		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:     "Lexicographic LTE",
		// 	Input:    "'ab' <= 'abc'",
		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:     "Lexicographic GT",
		// 	Input:    "'aba' > 'abc'",
		// 	Expected: false,
		// },
		// EvaluationTest{

		// 	Name:     "Lexicographic GTE",
		// 	Input:    "'aba' >= 'abc'",
		// 	Expected: false,
		// },
		// EvaluationTest{

		// 	Name:     "Boolean sign prefix comparison",
		// 	Input:    "!true == false",
		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:     "Inversion of clause",
		// 	Input:    "!(10 < 0)",
		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:     "Negation after modifier",
		// 	Input:    "10 * -10",
		// 	Expected: -100.0,
		// },
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
		// {

		// 	"Single function",
		// 	Input: "foo()",
		// 	Functions: map[string]ExpressionFunction{
		// 		"foo": func(arguments ...interface{}) (interface{}, error) {
		// 			return true, nil
		// 		},
		// 	},

		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:  "Function with argument",
		// 	Input: "passthrough(1)",
		// 	Functions: map[string]ExpressionFunction{
		// 		"passthrough": func(arguments ...interface{}) (interface{}, error) {
		// 			return arguments[0], nil
		// 		},
		// 	},

		// 	Expected: 1.0,
		// },

		// EvaluationTest{

		// 	Name:  "Function with arguments",
		// 	Input: "passthrough(1, 2)",
		// 	Functions: map[string]ExpressionFunction{
		// 		"passthrough": func(arguments ...interface{}) (interface{}, error) {
		// 			return arguments[0].(float64) + arguments[1].(float64), nil
		// 		},
		// 	},

		// 	Expected: 3.0,
		// },
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
		// EvaluationTest{

		// 	Name:  "Empty function and modifier, compared",
		// 	Input: "numeric()-1 > 0",
		// 	Functions: map[string]ExpressionFunction{
		// 		"numeric": func(arguments ...interface{}) (interface{}, error) {
		// 			return 2.0, nil
		// 		},
		// 	},

		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:  "Empty function comparator",
		// 	Input: "numeric() > 0",
		// 	Functions: map[string]ExpressionFunction{
		// 		"numeric": func(arguments ...interface{}) (interface{}, error) {
		// 			return 2.0, nil
		// 		},
		// 	},

		// 	Expected: true,
		// },
		// EvaluationTest{

		// 	Name:  "Empty function logical operator",
		// 	Input: "success() && !false",
		// 	Functions: map[string]ExpressionFunction{
		// 		"success": func(arguments ...interface{}) (interface{}, error) {
		// 			return true, nil
		// 		},
		// 	},

		// 	Expected: true,
		// },
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
		// EvaluationTest{

		// 	Name:  "Empty function with prefix",
		// 	Input: "-ten()",
		// 	Functions: map[string]ExpressionFunction{
		// 		"ten": func(arguments ...interface{}) (interface{}, error) {
		// 			return 10.0, nil
		// 		},
		// 	},

		// 	Expected: -10.0,
		// },
		// EvaluationTest{

		// 	Name:  "Empty function as part of chain",
		// 	Input: "10 - numeric() - 2",
		// 	Functions: map[string]ExpressionFunction{
		// 		"numeric": func(arguments ...interface{}) (interface{}, error) {
		// 			return 5.0, nil
		// 		},
		// 	},

		// 	Expected: 3.0,
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
		// EvaluationTest{

		// 	Name:  "Enclosed empty function with modifier and comparator (#28)",
		// 	Input: "(ten() - 1) > 3",
		// 	Functions: map[string]ExpressionFunction{
		// 		"ten": func(arguments ...interface{}) (interface{}, error) {
		// 			return 10.0, nil
		// 		},
		// 	},

		// 	Expected: true,
		// },
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

} // namespace