/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <sstream>
#include "Runner.h"
#include "Unit.h"
#include "View.h"


extern OpCode end_opcode;

ProgramUnit::ProgramUnit()
{
    code.addOpCode(end_opcode);
    lines.insert(0, 1);
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

    auto line_view = getLineView(line_number);
    auto line_size = code.insertLine(line_view.offset, line_code);
    lines.insert(line_number, line_size);
}

std::string ProgramUnit::recreateLine(std::size_t line_number)
{
    if (line_number >= lines.size()) {
        return "";
    }
    auto line_view = getLineView(line_number);
    return code.recreateLine(line_view);
}

void ProgramUnit::run(std::ostream &os)
{
    Runner runner {code, os};
    runner.runProgram();
}

ProgramView ProgramUnit::getLineView(std::size_t line_number)
{
    return lines.getView(line_number);
}
