/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <cstdint>
#include "PrecendenceFwd.h"
#include "WordType.h"


class OpCode {
public:
    static std::size_t getCount();

    OpCode();

    WordType getValue() const;

private:
    WordType value;
};


inline WordType OpCode::getValue() const
{
    return value;
}


class Operator {
public:
    Operator() = default;
    Operator(OpCode opcode, Precedence precedence) :
        opcode {opcode}, precedence {precedence} { }

    OpCode opcode;
    Precedence precedence;
};
