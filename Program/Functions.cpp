/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Parser/Token.h>
#include "Functions.h"
#include "OpCode.h"


struct Function {
    std::string_view name;
    OpCode opcode;
};


Functions::Functions(std::initializer_list<Function> function_initializers)
{
    for (auto &[name, opcode] : function_initializers) {
        function_opcodes[name] = opcode;
        function_names[opcode.getValue()] = name;
    }
}

OpCode abs_opcode;
OpCode int_opcode;
OpCode sgn_opcode;
OpCode sqr_opcode;
OpCode log_opcode;
OpCode exp_opcode;
OpCode cos_opcode;
OpCode sin_opcode;
OpCode tan_opcode;
OpCode atn_opcode;

Functions &functions()
{
    static Functions functions {
        {"abs", abs_opcode},
        {"int", int_opcode},
        {"sgn", sgn_opcode},
        {"sqr", sqr_opcode},
        {"log", log_opcode},
        {"exp", exp_opcode},
        {"cos", cos_opcode},
        {"sin", sin_opcode},
        {"tan", tan_opcode},
        {"atn", atn_opcode}
    };
    return functions;
}

std::optional<OpCode> Functions::getFunctionOpcode(const Token &token)
{
    auto it = functions().function_opcodes.find(token.getValue());
    return it->second;
}

std::string_view Functions::getFunctionName(WordType opcode)
{
    return functions().function_names[opcode];
}
