/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <vector>
#include "OpCode.h"

namespace {

WordType opcode_count;

WordType generateNewOpCode()
{
    auto new_opcode = opcode_count;
    ++opcode_count;
    return new_opcode;
}

}  // namespace

std::size_t OpCode::getCount()
{
    return opcode_count;
}

OpCode::OpCode() :
    value {generateNewOpCode()}
{
}
