/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <map>
#include <string_view>
#include <Parser/Error.h>
#include <Program/Code.h>
#include <Program/Recreator.h>
#include "Compiler.h"


using CompilerFunction = void(*)(Compiler &);

OpCode print_opcode;
OpCode end_opcode;
OpCode const_num_opcode;

namespace {

struct CommandOpCode {
    OpCode opcode;
    std::string_view keyword;
    CompilerFunction compile_function;
};

class Commands {
public:
    Commands(std::initializer_list<CommandOpCode> initializers);

    std::map<std::string_view, OpCode> codes;
    std::map<WordType, std::string_view> keywords;
    std::map<WordType, CompilerFunction> compile_functions;
};

Commands::Commands(std::initializer_list<CommandOpCode> initializers)
{
    for (auto &[opcode, keyword, function] : initializers) {
        codes[keyword] = opcode;
        keywords[opcode.getValue()] = keyword;
        compile_functions[opcode.getValue()] = function;
    }
}

void compilePrint(Compiler &compiler)
{
    compiler.compileExpression();
    compiler.addOpCode(print_opcode);
}

void compileEnd(Compiler &compiler)
{
    compiler.addOpCode(end_opcode);
}

Commands &commands()
{
    static Commands commands {
        {print_opcode, "print", compilePrint},
        {end_opcode, "end", compileEnd}
    };
    return commands;
}

std::optional<OpCode> findCommand(std::string_view keyword)
{
    if (auto it = commands().codes.find(keyword); it != commands().codes.end()) {
        return it->second;
    }
    return {};
}

void compileCommand(OpCode opcode, Compiler &compiler)
{
    commands().compile_functions[opcode.getValue()](compiler);
}

}  // namespace

std::string_view Compiler::getCommandKeyword(WordType opcode)
{
    return commands().keywords[opcode];
}

Compiler::Compiler(ProgramCode &code, std::istream &is) :
    code {code},
    parser {is}
{
}

void Compiler::compileLine()
{
    unsigned column = parser.getColumn();
    auto keyword = parser.parseIdentifier();
    if (auto command_opcode = findCommand(keyword)) {
        compileCommand(*command_opcode, *this);
    } else {
        throw ParseError {"expected valid command or variable for assignment", column};
    }
}

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
