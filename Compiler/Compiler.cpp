/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <map>
#include <string_view>
#include <Parser/Error.h>
#include <Program/Code.h>
#include <Program/Commands.h>
#include <Program/Operators.h>
#include <Program/Recreator.h>
#include "Compiler.h"


extern OpCode const_num_opcode;
OpCode bottom_opcode;

bool operator==(OpCode &lhs, OpCode &rhs)
{
    return lhs.getValue() == rhs.getValue();
}

bool operator!=(OpCode &lhs, OpCode &rhs)
{
    return !(lhs == rhs);
}


Compiler::Compiler(ProgramCode &code, std::istream &is) :
    code {code},
    parser {is}
{
}

void Compiler::compileLine()
{
    unsigned column = parser.getColumn();
    auto keyword = parser.parseIdentifier();
    if (auto command_opcode = Commands::getOpCode(keyword)) {
        Commands::getCompileFunction(*command_opcode)(*this);
    } else {
        throw ParseError {"expected valid command or variable for assignment", column};
    }
}

bool Compiler::compileExpression()
{
    operator_stack.emplace(bottom_opcode, Operators::bottom_precedence);
    parser.skipWhiteSpace();
    if (compileUnaryExpression()) {
        if (compileBinaryOperator()) {
            compileUnaryExpression();
        }
        flushOperatorStack(Operators::lowest_precedence);
        return true;
    }
    return false;
}

bool Compiler::compileUnaryExpression()
{
    for (;;) {
        if (compileNumConst()) {
            return true;
        } else if (compileUnaryOperator()) {
            parser.skipWhiteSpace();
        } else if (operator_stack.top().opcode != bottom_opcode) {
            unsigned column = parser.getColumn();
            throw ParseError {"expected unary operator or operand", column};
        } else {
            return false;
        }
    }
}

bool Compiler::compileNumConst()
{
    auto number = parser.parseNumber();
    if (number) {
        code.addOpCode(const_num_opcode);
        auto index = code.addConstNum(*number);
        code.addOperand(index);
        return true;
    }
    return false;
}

bool Compiler::compileUnaryOperator()
{
    auto opcode = Operators::getUnaryOpcode(parser.peekNextChar());
    if (opcode) {
        parser.getNextChar();
        operator_stack.emplace(*opcode, Operators::getPrecedence(*opcode));
        return true;
    }
    return false;
}

bool Compiler::compileBinaryOperator()
{
    auto opcode = Operators::getBinaryOpcode(parser.peekNextChar());
    if (opcode) {
        parser.getNextChar();
        auto operator_precendence = Operators::getPrecedence(*opcode);
        flushOperatorStack(operator_precendence);
        operator_stack.emplace(*opcode, operator_precendence);
        return true;
    }
    return false;
}

void Compiler::flushOperatorStack(Precedence higher_or_same)
{
    while (operator_stack.top().precedence >= higher_or_same) {
        code.addOpCode(operator_stack.top().opcode);
        operator_stack.pop();
    }
}

void Compiler::addOpCode(OpCode opcode)
{
    code.addOpCode(opcode);
}
