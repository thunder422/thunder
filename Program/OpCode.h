/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <cstdint>


using WordType = uint16_t;
class Recreator;
using RecreateFunction = void(*)(Recreator &);

class OpCode {
public:
    static void recreate(WordType opcode, Recreator &recreator);

    OpCode(RecreateFunction recreate_function);

    WordType getValue() const;

private:
    WordType value;
};


inline WordType OpCode::getValue() const
{
    return value;
}
