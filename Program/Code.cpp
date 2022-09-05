/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <iostream>
#include <Parser/Parser.h>
#include "Code.h"
#include "CommandOpCode.h"


ProgramCode::ProgramCode(std::istream &is)
{
    Parser parser {is};
    auto keyword = parser.parseIdentifier();
    auto command_opcode = CommandOpCode::find(keyword);
    words.emplace_back(*command_opcode);
}

std::string ProgramCode::recreateLine(std::size_t line_offset)
{
    std::string line {CommandOpCode::getKeyword(words[line_offset])};
    return line;
}
