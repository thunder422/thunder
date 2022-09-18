/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <iostream>
#include <Compiler/Compiler.h>
#include <Parser/Parser.h>
#include "Code.h"
#include "CommandOpCode.h"
#include "Recreator.h"


ProgramCode::ProgramCode(std::istream &is)
{
    Compiler compiler {*this, is};
    compiler.compileLine();
}

std::string ProgramCode::recreateLine(std::size_t line_offset)
{
    Recreator recreator;
    OpCode::recreate(words[line_offset], recreator);
    return recreator.getLine();
}

void ProgramCode::addOpCode(const OpCode &opcode)
{
    words.emplace_back(opcode);
}

void compilePrint(Compiler &compiler);
void compileEnd(Compiler &compiler);

void recreatePrint(Recreator &recreator);
void recreateEnd(Recreator &recreator);

CommandOpCode print_opcode {"print", compilePrint, recreatePrint};
CommandOpCode end_opcode {"end", compileEnd, recreateEnd};

void compilePrint(Compiler &compiler)
{
    compiler.compileExpression();
    compiler.addOpCode(print_opcode);
}

void compileEnd(Compiler &compiler)
{
    compiler.addOpCode(end_opcode);
}

void recreatePrint(Recreator &recreator)
{
    recreator.addCommandKeyword(print_opcode);
}

void recreateEnd(Recreator &recreator)
{
    recreator.addCommandKeyword(end_opcode);
}
