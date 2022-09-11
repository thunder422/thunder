/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include "CommandOpCode.h"
#include "Recreator.h"


void Recreator::addCommandKeyword(CommandOpCode opcode)
{
    std::string string {opcode.getKeyword(opcode.getValue())};
    line.append(string);
}

std::string &&Recreator::getLine()
{
    return std::move(line);
}
