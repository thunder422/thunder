/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Catch/catch.hpp>
#include "Word.h"


class Recreator {
public:
    bool was_called {false};
};


void recreateCode(Recreator &recreator)
{
    recreator.was_called = true;
}

TEST_CASE("generate program words", "[words]")
{
    OpCode first_opcode {recreateCode};
    OpCode second_opcode {recreateCode};
    OpCode third_opcode {recreateCode};

    SECTION("create a program word from a code")
    {
        ProgramWord word {second_opcode};

        REQUIRE(word == second_opcode.getValue());
    }
    SECTION("create a program word from a value (operand)")
    {
        ProgramWord word {15};

        REQUIRE(word == 15);
    }
    SECTION("recreate mechanism")
    {
        Recreator test_recreator;

        OpCode::recreate(third_opcode.getValue(), test_recreator);

        REQUIRE(test_recreator.was_called);
    }
}
