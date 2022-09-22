/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <charconv>
#include <Program/Code.h>
#include <Program/CommandOpCode.h>
#include <Program/Recreator.h>
#include "Compiler.h"


Compiler::Compiler(ProgramCode &code, std::istream &is) :
    code {code},
    parser {is}
{
}

void Compiler::compileLine()
{
    auto keyword = parser.parseIdentifier();
    auto command_opcode = CommandOpCode::find(keyword);
    CommandOpCode::compile(*command_opcode, *this);
}

void recreateConstNum(Recreator &recreator)
{
    auto index = recreator.getOperand();
    auto number = recreator.getConstNum(index);

    char string[20];
    auto [end, error_code] = std::to_chars(string, string + sizeof(string), number);
    recreator.pushString(std::string{string, end});
}

OpCode const_num_opcode {recreateConstNum};

void Compiler::compileExpression()
{
    parser.skipWhiteSpace();
    auto number = parser.parseNumber();
    if (number) {
        code.addOpCode(const_num_opcode);
        auto index = code.addConstNum(*number);
        code.addOperand(index);
    }
}

void Compiler::addOpCode(OpCode opcode)
{
    code.addOpCode(opcode);
}
