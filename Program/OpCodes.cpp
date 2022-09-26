/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include "OpCode.h"
#include "OpCodes.h"


struct AllOpCode {
    OpCode opcode;
    RecreateFunction recreate_function;
};

OpCodes::OpCodes(std::initializer_list<AllOpCode> initializers) :
    recreate_functions(OpCode::getCount())
{
    for (auto &[opcode, recreate_function] : initializers) {
        recreate_functions[opcode.getValue()] = recreate_function;
    }
}

OpCode const_num_opcode;
extern OpCode print_opcode;
extern OpCode end_opcode;

void recreateCommand(Recreator &recreator);
void recreateConstNum(Recreator &recreator);

OpCodes &opcodes()
{
    static OpCodes opcodes {
        {print_opcode, recreateCommand},
        {end_opcode, recreateCommand},
        {const_num_opcode, recreateConstNum}
    };
    return opcodes;
}

RecreateFunction OpCodes::getRecreateFunction(WordType opcode)
{
    return opcodes().recreate_functions[opcode];
}
