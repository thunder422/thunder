/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <map>
#include "OpCode.h"

namespace {

WordType opcode_count;

WordType generateNewOpCode()
{
    auto new_opcode = opcode_count;
    ++opcode_count;
    return new_opcode;
}

class Table {
public:
    std::map<WordType, RecreateFunction> recreate_functions;
};

Table &table()
{
    static Table table;
    return table;
};

}  // namespace

void OpCode::recreate(WordType opcode, Recreator &recreator)
{
    table().recreate_functions[opcode](recreator);
}

OpCode::OpCode(RecreateFunction recreate_function) :
    value {generateNewOpCode()}
{
    table().recreate_functions[getValue()] = recreate_function;
}
