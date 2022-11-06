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
struct BinaryOpCode;
class Compiler;
class OpCode;
enum class Precedence : int;

class Operators {
public:
    static std::optional<OpCode> getUnaryOpcode(char c);
    static std::string_view getUnaryChar(WordType opcode);
    static std::optional<OpCode> getBinaryOpcode(char c);
    static std::string_view getBinaryChar(WordType opcode);
    static Precedence getPrecedence(OpCode operator_opcode);
    static Precedence bottom_precedence;
    static Precedence lowest_precedence;

    Operators(std::initializer_list<UnaryOpCode> unary_initializers,
        std::initializer_list<UnaryOpCode> binary_initializers);

private:
    std::map<char, OpCode> unary_codes;
    std::map<WordType, char> unary_chars;
    std::map<char, OpCode> binary_codes;
    std::map<WordType, char> binary_chars;
    std::map<WordType, Precedence> precedences;
};
