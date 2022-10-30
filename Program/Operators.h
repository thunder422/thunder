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


struct UnaryOpCode;
class Compiler;
class OpCode;

class Operators {
public:
    static std::optional<OpCode> getUnaryOpcode(char c);
    static std::string_view getUnaryChar(WordType opcode);

    Operators(std::initializer_list<UnaryOpCode> unary_initializers);

private:
    std::map<char, OpCode> unary_codes;
    std::map<WordType, char> unary_chars;
};
