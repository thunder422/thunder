/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Compiler/Compiler.h>
#include "OpCode.h"
#include "Operators.h"


struct UnaryOpCode {
    OpCode opcode;
    char c;
};

Operators::Operators(std::initializer_list<UnaryOpCode> unary_initializers)
{
    for (auto &[opcode, c] : unary_initializers) {
        unary_codes[c] = opcode;
        unary_chars[opcode.getValue()] = c;
    }
}

OpCode neg_opcode;

Operators &operators()
{
    static Operators operators {
        {neg_opcode, '-'}
    };
    return operators;
}

std::optional<OpCode> Operators::getUnaryOpcode(char c)
{
    if (auto it = operators().unary_codes.find(c); it != operators().unary_codes.end()) {
        return it->second;
    }
    return {};
}

std::string_view Operators::getUnaryChar(WordType opcode)
{
    auto &c = operators().unary_chars[opcode];
    return {&c, 1};
}
