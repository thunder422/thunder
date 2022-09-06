/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <map>
#include <string_view>
#include "CommandOpCode.h"


namespace {

class Command {
public:
    std::map<std::string_view, CommandOpCode *> codes;
    std::map<WordType, std::string_view> keywords;
    std::map<WordType, CompilerFunction> compile_functions;
};

Command &command()
{
    static Command command;
    return command;
}

}  // namespace

const CommandOpCode *CommandOpCode::find(std::string_view keyword)
{
    if (auto it = command().codes.find(keyword); it != command().codes.end()) {
        return it->second;
    }
    return nullptr;
}

void CommandOpCode::compile(const CommandOpCode opcode, ProgramCode &code)
{
    command().compile_functions[opcode.getValue()](code);
}

std::string_view CommandOpCode::getKeyword(WordType opcode)
{
    return command().keywords[opcode];
}

CommandOpCode::CommandOpCode(std::string_view keyword, CompilerFunction compile)
{
    command().codes[keyword] = this;
    command().keywords[getValue()] = keyword;
    command().compile_functions[getValue()] = compile;
}
