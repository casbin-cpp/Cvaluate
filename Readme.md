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
    - [ ] Support bit shift.  
- [ ] Support variable and accessors.
    - [x] Support `Eval(params)`, `params` is a `map` like `name: object` in `gvaluate `.
    - [x] ~~Basic data type of `params` is `int ,float, string`.~~ Use [json](https://github.com/nlohmann/json) as base data type.
    - [x] Support simple
    - [ ] Object data of `params` should reload `operator[]` and `operator` for `access` and compare. Maybe we can accomplish it by define a base object or use template to support accessors.

- [ ] Add benchmark for Cvaluate.