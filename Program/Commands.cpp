/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Compiler/Compiler.h>
#include <Program/Find.h>
#include "Commands.h"
#include "OpCode.h"


struct CommandOpCode {
    OpCode opcode;
    std::string_view keyword;
    CompilerFunction compile_function;
};

Commands::Commands(std::initializer_list<CommandOpCode> initializers)
{
    for (auto &[opcode, keyword, compile_function] : initializers) {
        codes[keyword] = opcode;
        keywords[opcode.getValue()] = keyword;
        compile_functions[opcode.getValue()] = compile_function;
    }
}

OpCode print_opcode;
OpCode print_num_opcode;
OpCode end_opcode;

void compilePrint(Compiler &compiler);
void compileEnd(Compiler &compiler);

Commands &commands()
{
    static Commands commands {
        {print_opcode, "print", compilePrint},
        {end_opcode, "end", compileEnd}
    };
    return commands;
}

std::optional<OpCode> Commands::getOpCode(std::string_view keyword)
{
    return find(commands().codes, keyword);
}

CompilerFunction Commands::getCompileFunction(OpCode opcode)
{
    return commands().compile_functions[opcode.getValue()];
}

std::string_view Commands::getKeyword(WordType opcode)
{
    return commands().keywords[opcode];
}


void compilePrint(Compiler &compiler)
{
    if (compiler.compileExpression()) {
        compiler.addOpCode(print_num_opcode);
    }
    compiler.addOpCode(print_opcode);
}

void compileEnd(Compiler &compiler)
{
    compiler.addOpCode(end_opcode);
}
