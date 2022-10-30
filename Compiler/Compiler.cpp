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
    parser.skipWhiteSpace();
    if (compileUnaryExpression()) {
        if (compileBinaryOperator()) {
            compileUnaryExpression();
        }
        flushOpcodeStack();
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
        } else if (!opcode_stack.empty()) {
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
        opcode_stack.emplace(*opcode);
        return true;
    }
    return false;
}

bool Compiler::compileBinaryOperator()
{
    auto opcode = Operators::getBinaryOpcode(parser.peekNextChar());
    if (opcode) {
        parser.getNextChar();
        opcode_stack.emplace(*opcode);
        return true;
    }
    return false;
}

void Compiler::flushOpcodeStack()
{
    while (!opcode_stack.empty()) {
        code.addOpCode(opcode_stack.top());
        opcode_stack.pop();
    }
}

void Compiler::addOpCode(OpCode opcode)
{
    code.addOpCode(opcode);
}
