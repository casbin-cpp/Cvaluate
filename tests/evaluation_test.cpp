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
		{

			"Exponent precedence",
			"1 + 5 ** 3 % 2 * 5",
			float(6),
		},

    };

    RunEvaluationTests(token_evaluation_tests);
}

} // namespace