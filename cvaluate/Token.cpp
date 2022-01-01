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

#include <Token.h>
#include <Exception.h>

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

    std::string GetTokenValueString(TokenAvaiableValue token_value) {
        if (auto value = std::get_if<std::string>(&token_value)) {
            return *value;
        } else {
            throw CvaluateException("Can't get string from current token");
        }
    }

    int GetTokenValueInt(TokenAvaiableValue token_value) {
        if (auto value = std::get_if<int>(&token_value)) {
            return *value;
        } else {
            throw CvaluateException("Can't get int from current token");
        }
    }

    float GetTokenValueFloat(TokenAvaiableValue token_value) {
        if (auto value = std::get_if<float>(&token_value)) {
            return *value;
        } else {
            throw CvaluateException("Can't get float from current token");
        }
    }


    float GetTokenValueNumeric(TokenAvaiableValue token_value) {
        if (auto value = std::get_if<int>(&token_value)) {
            return *value;
        } else if (auto value = std::get_if<float>(&token_value)) {
            return *value;
        } else {
            throw CvaluateException("Can't get float from current token");
        }
    }

    bool GetTokenValueBool(TokenAvaiableValue token_value) {
        if (auto value = std::get_if<bool>(&token_value)) {
            return *value;
        } else {
            throw CvaluateException("Can't get bool from current token");
        }
    }
} // Cvaluate