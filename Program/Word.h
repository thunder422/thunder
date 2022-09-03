/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <cstdint>

#include "OpCode.h"


class ProgramWord {
public:
    explicit ProgramWord(OpCode opcode);
    explicit ProgramWord(WordType operand);

    operator WordType() const;

private:
    const WordType word;
};


inline ProgramWord::ProgramWord(OpCode opcode) :
    word {opcode.getValue()}
{
}

inline ProgramWord::ProgramWord(WordType operand) :
    word {operand}
{
}

inline ProgramWord::operator WordType() const
{
    return word;
}
