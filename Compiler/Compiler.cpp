/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Program/Code.h>
#include <Program/CommandOpCode.h>
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

void Compiler::compileExpression()
{
}

void Compiler::addOpCode(OpCode opcode)
{
    code.addOpCode(opcode);
}
