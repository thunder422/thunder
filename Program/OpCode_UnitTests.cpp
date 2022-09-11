/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Catch/catch.hpp>
#include "OpCode.h"


class Recreator {
public:
    bool was_called {false};
};

void recreateCode(Recreator &recreator)
{
    recreator.was_called = true;
}

TEST_CASE("generate unique codes for each unique program element", "[opcodes]")
{
    SECTION("first op code has value of zero")
    {
        OpCode first_opcode {recreateCode};

        REQUIRE(first_opcode.getValue() == 0);
    }
    SECTION("second op code has next value")
    {
        OpCode second_opcode {recreateCode};

        REQUIRE(second_opcode.getValue() == 1);
    }
    SECTION("recreate mechanism")
    {
        Recreator test_recreator;
        OpCode third_opcode {recreateCode};

        OpCode::recreate(third_opcode.getValue(), test_recreator);

        REQUIRE(test_recreator.was_called);
    }
}
