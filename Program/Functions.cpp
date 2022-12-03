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

struct AlternateFunction {
    std::string_view name;
    OpCode primary_opcode;
    OpCode alternate_opcode;
};


Functions::Functions(std::initializer_list<Function> functions,
    std::initializer_list<AlternateFunction> alternates)
{
    for (auto &[name, opcode] : functions) {
        opcodes[name] = opcode;
        names[opcode.getValue()] = name;
        num_arguments[opcode.getValue()] = 1;
    }
    for (auto &[name, primary_opcode, alternate_opcode] : alternates) {
        alternate_opcodes[primary_opcode.getValue()] = alternate_opcode;
        names[alternate_opcode.getValue()] = name;
        num_arguments[alternate_opcode.getValue()] = 0;
    }
}

OpCode abs_opcode;
OpCode int_opcode;
OpCode rnd_opcode;
OpCode rnd0_opcode;
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
        {
            {"abs", abs_opcode},
            {"int", int_opcode},
            {"rnd", rnd_opcode},
            {"sgn", sgn_opcode},
            {"sqr", sqr_opcode},
            {"log", log_opcode},
            {"exp", exp_opcode},
            {"cos", cos_opcode},
            {"sin", sin_opcode},
            {"tan", tan_opcode},
            {"atn", atn_opcode}
        }, {
            {"rnd", rnd_opcode, rnd0_opcode}
        }
    };
    return functions;
}

std::optional<OpCode> Functions::getOpcode(const Token &token)
{
    auto iterator = functions().opcodes.find(token.getValue());
    return iterator->second;
}

std::optional<OpCode> Functions::getAlternateOpcode(OpCode &opcode)
{
    if (auto iterator = functions().alternate_opcodes.find(opcode.getValue());
            iterator != functions().alternate_opcodes.end()) {
        return iterator->second;
    }
    return {};
}

std::string_view Functions::getName(WordType opcode)
{
    return functions().names[opcode];
}

int Functions::getNumArguments(WordType opcode)
{
    return functions().num_arguments[opcode];
}
