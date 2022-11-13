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
#include "OtherOperatorFwd.h"
#include "Precedence.h"
#include "WordType.h"


struct UnaryOperator;
struct BinaryOperator;
class Compiler;
class OpCode;
class Operator;


class Operators {
public:
    static std::optional<Operator> getUnaryOpcode(char c);
    static std::string_view getUnaryChar(WordType opcode);
    static std::optional<Operator> getBinaryOpcode(char c, OtherOperator other);
    static std::string_view getBinaryChar(WordType opcode);
    static Precedence getPrecedence(WordType opcode);

    Operators(std::initializer_list<UnaryOperator> unary_initializers,
        std::initializer_list<UnaryOperator> binary_initializers);

private:
    std::map<char, Operator> unary_codes;
    std::map<WordType, char> unary_chars;
    std::map<char, Operator> binary_codes;
    std::map<WordType, char> binary_chars;
    std::map<WordType, Precedence> precedences;
};
