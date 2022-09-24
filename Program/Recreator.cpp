/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <charconv>
#include "Code.h"
#include "CommandOpCode.h"
#include "Recreator.h"


extern CommandOpCode print_opcode;
extern CommandOpCode end_opcode;
extern OpCode const_num_opcode;

namespace {

struct OpCodeRecreate {
    OpCode opcode;
    RecreateFunction function;
};

class RecreateFunctions {
public:
    RecreateFunctions(std::initializer_list<OpCodeRecreate> initializers);

    std::vector<RecreateFunction> functions;
};

RecreateFunctions::RecreateFunctions(std::initializer_list<OpCodeRecreate> initializers) :
    functions(OpCode::getCount())
{
    for (auto &initializer : initializers) {
        functions[initializer.opcode.getValue()] = initializer.function;
    }
}

void recreateCommand(Recreator &recreator)
{
    recreator.addCommandKeyword();
}

void recreateConstNum(Recreator &recreator)
{
    auto index = recreator.getOperand();
    auto number = recreator.getConstNum(index);

    char string[20];
    auto [end, error_code] = std::to_chars(string, string + sizeof(string), number);
    recreator.pushString(std::string{string, end});
}

void recreate(WordType opcode, Recreator &recreator)
{
    static RecreateFunctions table {
        {print_opcode, recreateCommand},
        {end_opcode, recreateCommand},
        {const_num_opcode, recreateConstNum}
    };
    table.functions[opcode](recreator);
}

}  // namespace

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
        recreate(opcode, *this);
        ++offset;
    }
    return std::move(line);
}
