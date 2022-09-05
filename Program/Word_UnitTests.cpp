/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Catch/catch.hpp>
#include "CommandOpCode.h"
#include "Word.h"


TEST_CASE("generate program words", "[words]")
{
    OpCode first_opcode;
    OpCode second_opcode;
    OpCode third_opcode;

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
    SECTION("add a command op code")
    {
        CommandOpCode command_opcode {"test"};

        ProgramWord word {command_opcode};

        REQUIRE(word == command_opcode.getValue());
    }
}
