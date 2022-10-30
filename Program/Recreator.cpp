/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <charconv>
#include "Code.h"
#include "Commands.h"
#include "OpCodes.h"
#include "Operators.h"
#include "Recreator.h"
#include "View.h"


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

void recreateUnaryOperator(Recreator &recreator)
{
    std::string string {Operators::getUnaryChar(recreator.getOpcode())};
    auto &top_string = recreator.topString();
    if (isdigit(top_string.front())) {
        string.append(1, ' ');
    }
    string.append(top_string);
    recreator.swapTopString(string);
}

Recreator::Recreator(ProgramCode &code) :
    code {code}
{
}

void Recreator::addCommandKeyword()
{
    std::string string {Commands::getKeyword(code.getWord(offset))};
    if (!line.empty()) {
        string.append(1, ' ').append(line);
    }
    swapTopString(string);
}

std::size_t Recreator::getOpcode()
{
    return opcode;
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

const std::string &Recreator::topString() const
{
    return line;
}

void Recreator::swapTopString(std::string &string)
{
    line.swap(string);
}

std::string &&Recreator::recreateLine(const ProgramView &line_view)
{
    offset = line_view.offset;
    auto end_offset = offset + line_view.size;
    while (offset < end_offset) {
        opcode = code.getWord(offset);
        OpCodes::getRecreateFunction(opcode)(*this);
        ++offset;
    }
    return std::move(line);
}
