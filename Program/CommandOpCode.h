/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <string_view>
#include "OpCode.h"


class Compiler;
using CompilerFunction = void(*)(Compiler &);

class CommandOpCode : public OpCode {
public:
    static const CommandOpCode *find(std::string_view keyword);
    static void compile(const CommandOpCode opcode, Compiler &compiler);
    static std::string_view getKeyword(WordType opcode);

    CommandOpCode(std::string_view keyword, CompilerFunction compile_function);
};
