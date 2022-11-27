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


struct Function;
struct AlternateFunction;
class OpCode;
class Token;


class Functions {
public:
    static std::optional<OpCode> getOpcode(const Token &token);
    static std::optional<OpCode> getAlternateOpcode(OpCode &opcode);
    static std::string_view getName(WordType opcode);
    static int getNumArguments(WordType opcode);

    Functions(std::initializer_list<Function> functions,
        std::initializer_list<AlternateFunction> alternates);

private:
    std::map<std::string_view, OpCode> opcodes;
    std::map<WordType, std::string_view> names;
    std::map<WordType, OpCode> alternate_opcodes;
    std::map<WordType, int> num_arguments;
};
