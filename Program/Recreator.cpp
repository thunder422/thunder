/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include "Code.h"
#include "CommandOpCode.h"
#include "Recreator.h"


Recreator::Recreator(ProgramCode &code) :
    code {code}
{
}

void Recreator::addCommandKeyword()
{
    std::string string {CommandOpCode::getKeyword(code.getWord(offset))};
    if (!line.empty()) {
        string.append(1, ' ').append(line);
    }
    line.swap(string);
    is_done = true;
}

std::size_t Recreator::getOperand()
{
    return code.getWord(++offset);
}

double Recreator::getConstNum(std::size_t index)
{
    return code.getConstNum(index);
}

void Recreator::pushString(std::string string)
{
    line.append(string);
}

std::string &&Recreator::recreateLine(std::size_t line_offset)
{
    offset = line_offset;
    while (!is_done) {
        auto opcode = code.getWord(offset);
        OpCode::recreate(opcode, *this);
        ++offset;
    }
    return std::move(line);
}
