/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include "Unit.h"


extern OpCode end_opcode;

ProgramUnit::ProgramUnit()
{
    code.addOpCode(end_opcode);
}

std::string ProgramUnit::recreateLine(std::size_t line)
{
    auto offset = getOffset(line);
    return std::to_string(line + 1) + " " + code.recreateLine(offset);
}

std::size_t ProgramUnit::getOffset(std::size_t line)
{
    (void)line;
    return 0;
}


