/*
* Copyright 2021 The casbin Authors. All Rights Reserved.
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
*
* This is a test file for benchmarking the performance of casbin::Model
*/

#include <benchmark/benchmark.h>
#include <cvaluate/cvaluate.h>
#include "../test_config.h"

static void BenchmarkSingleParse(benchmark::State& state) {
    for(auto _ : state)
        Cvaluate::EvaluableExpression("1");
}

BENCHMARK(BenchmarkSingleParse);

static void BenchmarkSimpleParse(benchmark::State& state) {
    for(auto _ : state)
        Cvaluate::EvaluableExpression("(requests_made * requests_succeeded / 100) >= 90");
}

BENCHMARK(BenchmarkSimpleParse);

static void BenchmarkFullParse(benchmark::State& state) {
    std::string expression = std::string("2 > 1 &&") +
		"\'something\' != \'nothing\' || " +
		"[escapedVariable name with spaces] <= unescaped\\-variableName &&" +
		"modifierTest + 1000 / 2 > (80 * 100 % 2)";
    for(auto _ : state)
        auto x = Cvaluate::EvaluableExpression(expression);
}

BENCHMARK(BenchmarkFullParse);

static void BenchmarkEvaluationSingle(benchmark::State& state) {
    auto expression = Cvaluate::EvaluableExpression("1");
    for(auto _ : state)
        expression.Evaluate();
}

BENCHMARK(BenchmarkEvaluationSingle);

static void BenchmarkEvaluationNumericLiteral(benchmark::State& state) {
    auto expression = Cvaluate::EvaluableExpression("(2) > (1)");
    for(auto _ : state)
        expression.Evaluate();
}

BENCHMARK(BenchmarkEvaluationNumericLiteral);

static void BenchmarkEvaluationLiteralModifiers(benchmark::State& state) {
    auto expression = Cvaluate::EvaluableExpression("(2) + (2) == (4)");
    for(auto _ : state)
        expression.Evaluate();
}

BENCHMARK(BenchmarkEvaluationLiteralModifiers);

static void BenchmarkEvaluationParameter(benchmark::State& state) {
    auto expression = Cvaluate::EvaluableExpression("requests_made");
    auto parameters = Cvaluate::Parameters({
        {"requests_made", float(99.0)},
    });
    for(auto _ : state)
        expression.Evaluate(parameters);
}

BENCHMARK(BenchmarkEvaluationParameter);

static void BenchmarkEvaluationParameters(benchmark::State& state) {
    auto expression = Cvaluate::EvaluableExpression("requests_made > requests_succeeded");
    auto parameters = Cvaluate::Parameters({
        {"requests_made", float(99.0)},
        {"requests_succeeded", float(90.0)},
    });
    for(auto _ : state)
        expression.Evaluate(parameters);
}

BENCHMARK(BenchmarkEvaluationParameters);

static void BenchmarkEvaluationParametersModifiers(benchmark::State& state) {
    auto expression = Cvaluate::EvaluableExpression("(requests_made * requests_succeeded / 100) >= 90");
    auto parameters = Cvaluate::Parameters({
        {"requests_made", float(99.0)},
        {"requests_succeeded", float(90.0)},
    });
    for(auto _ : state)
        expression.Evaluate(parameters);
}

BENCHMARK(BenchmarkEvaluationParametersModifiers);

// static void BenchmarkComplexExpression(benchmark::State& state) {
//     std::string expressionString = std::string("2 > 1 &&") +
// 		"'something' != 'nothing' || " +
// 		"[escapedVariable name with spaces] <= unescaped\\-variableName &&" +
// 		"modifierTest + 1000 / 2 > (80 * 100 % 2)";
//     auto expression = Cvaluate::EvaluableExpression(expressionString);
//     auto parameters = Cvaluate::Parameters({
//         {"escapedVariable name with spaces", float(99.0)},
//         {"unescaped\\-variableName", float(90.0)},
//         {"modifierTest", float(5.0)},
//     });
//     for(auto _ : state)
//         expression.Evaluate(parameters);
// }

// BENCHMARK(BenchmarkComplexExpression);

static void BenchmarkAccessors(benchmark::State& state) {
    std::string expression_string = "foo.Int";
    auto expression = Cvaluate::EvaluableExpression(expression_string);
    auto parameters = fooParameter;
    for(auto _ : state)
        expression.Evaluate(parameters);
}

BENCHMARK(BenchmarkAccessors);

static void BenchmarkNestedAccessors(benchmark::State& state) {
    std::string expression_string = "foo.Nested.Funk";
    auto expression = Cvaluate::EvaluableExpression(expression_string);
    auto parameters = fooParameter;
    for(auto _ : state)
        expression.Evaluate(parameters);
}

BENCHMARK(BenchmarkNestedAccessors);