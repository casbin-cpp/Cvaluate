#include "EvaluableExpression.h"

namespace Cvaluate {

EvaluableExpression::EvaluableExpression(std::string expression, 
            ExpressionFunctionMap functions) {
    this->e_input = expression;

    this->e_tokens = ParseTokens(expression, functions);
};

std::vector<ExpressionToken> EvaluableExpression::Tokens() {
    return this->e_tokens;
}

}