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
#include "Recreator.h"


ProgramCode::ProgramCode(std::istream &is)
{
    Compiler compiler {*this, is};
    compiler.compileLine();
}

std::string ProgramCode::recreateLine(std::size_t line_offset)
{
    Recreator recreator {*this};
    return recreator.recreateLine(line_offset);
}

void ProgramCode::addOpCode(const OpCode &opcode)
{
    words.emplace_back(opcode);
}

WordType ProgramCode::addConstNum(double number)
{
    WordType index = const_nums.size();
    const_nums.emplace_back(number);
    return index;
}

void ProgramCode::addOperand(WordType operand)
{
    words.emplace_back(operand);
}

WordType ProgramCode::getWord(std::size_t offset) const
{
    return words[offset];
}

double ProgramCode::getConstNum(std::size_t index) const
{
    return const_nums[index];
}
