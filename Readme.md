## Cvaluate

Provides support for evaluating arbitrary C-like artithmetic/string expressions like [govaluate](https://github.com/Knetic/govaluate).

##  Development Target 

+ Support basic math expression, example `"1 + 1 == 2"`.
+ Support variable in expression, example `"a + 1 == 2"`.
+ Support Accessors, example `"a.b == 1"`.
+ Support Functions in expression, example `"foo(a, b)"`.

## Development Plan

- [x] Parse expression and get tokens.
    - [x] Parse expression.
    - [ ] Check tokens syntax and  check balance.
    - [x] Test parse tokens.
- [x] Build expression tree by tokens.
    - [x] Support baisc numeric add expression `1 + 49`.
    - [x] Design  expression tree like [evaluationStage](https://github.com/Knetic/govaluate/blob/master/evaluationStage.go).
    - [ ] Test expression tree.
- [x] Support basic math expression.
    - [x] Support `+-*/`.
    - [x] Support compare operator.
    - [ ] If two operator have same prority, like `"1 + 101 % 2 * 5"`. Because `2 * 5` is in the right stage of operator `%`, so it will calculate `2 * 5` first. In this case, we will get wrong answer `2` rather than `6`. This issue can be fixed by [reorderStages](https://github.com/Knetic/govaluate/blob/9aa49832a739dcd78a5542ff189fb82c3e423116/stagePlanner.go#L556).
    - [ ] Support bit shift.  
- [ ] Support variable and accessors.
    - [x] Support `Eval(params)`, `params` is a `map` like `name: object` in `gvaluate `.
    - [x] ~~Basic data type of `params` is `int ,float, string`.~~ Use [json](https://github.com/nlohmann/json) as base data type.
    - [x] Support simple field accessor using json.
    - [ ] Object data of `params` should reload `operator[]` and `operator` for `access` and compare. Maybe we can accomplish it by define a base object or use template to support accessors.

- [ ] Add benchmark for Cvaluate.


## Install Cvaluate

1. Download this repository `git clone https://github.com/casbin-cpp/Cvaluate`.

2. Make a directory to complie `mkdir build && cd build`.

3. Prepare build file `cmake -DCMAKE_BUILD_TYPE=Release ..`.

4. Build and install `make install`.

## Usage

``` cpp
void test_cvaluate() {
    using namespace std;
    string s = "1 + 2 > 1.3";
    auto expression = Cvaluate::EvaluableExpression(s);

    cout << expression.Evaluate({}) << endl;    // output true
}
```