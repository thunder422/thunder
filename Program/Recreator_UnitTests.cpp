/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Catch/catch.hpp>
#include "CommandOpCode.h"
#include "Recreator.h"


class Compiler { };

void compileTest(Compiler &)
{
}

void recreateCode(Recreator &recreator);

CommandOpCode test_opcode {"test", compileTest, recreateCode};

void recreateCode(Recreator &recreator)
{
    recreator.addCommandKeyword(test_opcode);
}


TEST_CASE("recreator", "[recreator]")
{
    Recreator recreator;

    SECTION("add a command keyword to the line")
    {
        CommandOpCode::recreate(test_opcode.getValue(), recreator);

        REQUIRE(recreator.getLine() == "test");
    }
    SECTION("call compile code function to complete coverage")
    {
        Compiler dummy_compiler;
        compileTest(dummy_compiler);
    }
}
