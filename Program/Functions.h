/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <map>
#include <optional>
#include <string_view>
#include "WordType.h"


struct Function;
class OpCode;
class Token;


class Functions {
public:
    static std::optional<OpCode> getFunctionOpcode(const Token &token);
    static std::string_view getFunctionName(WordType opcode);

    Functions(std::initializer_list<Function> function_initializers);

private:
    std::map<std::string_view, OpCode> function_opcodes;
    std::map<WordType, std::string_view> function_names;
};
