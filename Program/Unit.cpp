/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <sstream>
#include "Runner.h"
#include "Unit.h"


extern OpCode end_opcode;

ProgramUnit::ProgramUnit()
{
    code.addOpCode(end_opcode);
    lines.emplace_back(0);
}

size_t ProgramUnit::getLastLineNumber() const
{
    return lines.size();
}

void ProgramUnit::insertLine(std::size_t line_number, const std::string &line)
{
    ProgramCode line_code;
    std::istringstream iss {line};
    line_code.compileLine(iss);

    auto offset = getOffset(line_number);
    auto size = code.insertLine(offset, line_code);

    lines.emplace_back(0);
    for (auto i = line_number; i < lines.size(); ++i) {
        lines[i + 1] = lines[i].offset + size;
    }
    lines[line_number] = offset;
}

std::string ProgramUnit::recreateLine(std::size_t line_number)
{
    if (line_number >= lines.size()) {
        return "";
    }
    auto offset = getOffset(line_number);
    return code.recreateLine(offset);
}

void ProgramUnit::run(std::ostream &os)
{
    Runner runner {code, os};
    runner.runProgram();
}

std::size_t ProgramUnit::getOffset(std::size_t line_number)
{
    return lines[line_number].offset;
}
