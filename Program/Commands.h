/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <map>
#include <optional>
#include <string_view>
#include "WordType.h"


struct CommandOpCode;
class Compiler;
class OpCode;

using CompilerFunction = void(*)(Compiler &);

class Commands {
public:
    static std::optional<OpCode> getOpCode(std::string_view keyword);
    static CompilerFunction getCompileFunction(OpCode opcode);
    static std::string_view getKeyword(WordType opcode);

    Commands(std::initializer_list<CommandOpCode> initializers);

private:
    std::map<std::string_view, OpCode> codes;
    std::map<WordType, std::string_view> keywords;
    std::map<WordType, CompilerFunction> compile_functions;
};
