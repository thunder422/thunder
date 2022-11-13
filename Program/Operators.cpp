/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Compiler/Compiler.h>
#include "OpCode.h"
#include "Operators.h"
#include "OtherOperator.h"
#include "Precedence.h"


struct UnaryOperator {
    char c;
    Operator operator_;
};

struct BinaryOperator {
    char c;
    Operator operator_;
};

Operators::Operators(std::initializer_list<UnaryOperator> unary_initializers,
    std::initializer_list<UnaryOperator> binary_initializers)
{
    for (auto &[c, operator_] : unary_initializers) {
        unary_codes[c] = operator_;
        unary_chars[operator_.opcode.getValue()] = c;
        precedences[operator_.opcode.getValue()] = operator_.precedence;
    }
    for (auto &[c, operator_] : binary_initializers) {
        binary_codes[c] = operator_;
        binary_chars[operator_.opcode.getValue()] = c;
        precedences[operator_.opcode.getValue()] = operator_.precedence;
    }
}

extern OpCode null_opcode;
OpCode neg_opcode;
OpCode add_opcode;
OpCode sub_opcode;
OpCode mul_opcode;
OpCode div_opcode;
OpCode mod_opcode;
OpCode idiv_opcode;
OpCode pow_opcode;

Operators &operators()
{
    static Operators operators {
        {
            {'-', {neg_opcode, Precedence::Neq}},
            {'(', {null_opcode, Precedence::OpenParen}}
        },
        {
            {'+', {add_opcode, Precedence::Add}},
            {'-', {sub_opcode, Precedence::Add}},
            {'*', {mul_opcode, Precedence::Mul}},
            {'/', {div_opcode, Precedence::Mul}},
            {'%', {mod_opcode, Precedence::Mul}},
            {'\\', {idiv_opcode, Precedence::Mul}},
            {'^', {pow_opcode, Precedence::Pow}},
            {')', {null_opcode, Precedence::CloseParen}}
        }
    };
    return operators;
}

std::optional<Operator> Operators::getUnaryOpcode(char c)
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

bool isOperatorAllowed(Operator &operator_, OtherOperator other);

std::optional<Operator> Operators::getBinaryOpcode(char c, OtherOperator other)
{
    if (auto it = operators().binary_codes.find(c); it != operators().binary_codes.end()) {
        if (isOperatorAllowed(it->second, other)) {
            return it->second;
        }
    }
    return {};
}

bool isOperatorAllowed(Operator &operator_, OtherOperator other)
{
    if (operator_.precedence == Precedence::CloseParen) {
        if (other == OtherOperator::CloseParen) {
            return true;
        }
        return false;
    }
    return true;
}

std::string_view Operators::getBinaryChar(WordType opcode)
{
    auto &c = operators().binary_chars[opcode];
    return {&c, 1};
}

Precedence Operators::getPrecedence(WordType opcode)
{
    return operators().precedences[opcode];
}
