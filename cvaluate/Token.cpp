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

#include <cvaluate/Token.h>
#include <cvaluate/Exception.h>

namespace Cvaluate {
    bool CanTransitionTo(const TokenState& cur_state, const TokenKind& next_kind) {
        for (auto& kind: cur_state.ValidNextKinds) {
            if (kind == next_kind) {
                return true;
            }
        }

        return false;
    }

    bool GetPossibleStateForToken(TokenState& state, TokenKind& kind) {
        for (auto possible_state: kValidTokenStates) {
            if (possible_state.Kind == kind) {
                state = possible_state;
                return true;
            }
        }

        state = kValidTokenStates[0];
        return false;
    }

    std::string GetTokenValueString(TokenAvaiableData token_data) {
        if (token_data.is_string()) {
            return token_data.get<std::string>();
        } else if (token_data.is_number_integer()) {
            return std::to_string(token_data.get<int>());
        } else if (token_data.is_number_float()) {
            return std::to_string(int(token_data.get<float>()));
        } else if (token_data.is_boolean()) {
            if (token_data.get<bool>()) {
                return "true";
            } else {
                return "false";
            }
        } else {
            throw CvaluateException("Can't get string from current token");
        }
    }

    std::string GetTokenValueString(TokenAvaiableValue token_value) {
        auto token_data = GetTokenValueData(token_value);
        if (token_data.is_string()) {
            return token_data.get<std::string>();
        } else {
            throw CvaluateException("Can't get string from current token");
        }
    }

    int GetTokenValueInt(TokenAvaiableData token_data) {
        if (token_data.is_number_integer()) {
            return token_data.get<int>();
        } else {
            throw CvaluateException("Can't get int from current token");
        }
    }

    float GetTokenValueFloat(TokenAvaiableData token_data) {
        if (token_data.is_number_float()) {
            return token_data.get<float>();
        } else {
            throw CvaluateException("Can't get float from current token");
        }
    }


    float GetTokenValueNumeric(TokenAvaiableData token_data) {
        if (token_data.is_number_float()) {
            return token_data.get<float>();
        } else if (token_data.is_number_integer()) {
            return token_data.get<int>();
        } else {
            throw CvaluateException("Can't get float from current token");
        }
    }

    bool GetTokenValueBool(TokenAvaiableData token_data) {
        if (token_data.is_boolean()) {
            return token_data.get<bool>();
        } else {
            throw CvaluateException("Can't get bool from current token");
        }
    }

    TokenAvaiableData GetTokenValueJson(TokenAvaiableData token_data) {
        if (token_data.is_object()) {
            return token_data.get<TokenAvaiableData>();
        } else {
            throw CvaluateException("Can't get bool from current token");
        }
    }

    TokenAvaiableData GetTokenValueData(TokenAvaiableValue token_value) {
        if (auto data = std::get_if<nlohmann::json>(&token_value)) {
            return *data;
        } else {
            throw CvaluateException("Can't get TokenAvaiableData from current token");
        }
    }

    ExpressionFunction GetTokenValueFunction(TokenAvaiableValue token_value) {
        if (auto data = std::get_if<ExpressionFunction>(&token_value)) {
            return *data;
        } else {
            throw CvaluateException("Can't get TokenAvaiableData from current token");
        }
    }
} // Cvaluate