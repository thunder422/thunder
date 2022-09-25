/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Catch/catch.hpp>
#include "Code.h"
#include "Recreator.h"


extern OpCode print_opcode;
extern OpCode const_num_opcode;

TEST_CASE("recreator", "[recreator]")
{
    ProgramCode code;
    Recreator recreator {code};

    SECTION("add a command keyword to the line")
    {
        code.addOpCode(print_opcode);

        auto line = recreator.recreateLine(0);

        REQUIRE(line == "print");
    }
    SECTION("add string")
    {
        code.addOpCode(const_num_opcode);
        auto index = code.addConstNum(123.45);
        code.addOperand(index);
        code.addOpCode(print_opcode);

        auto line = recreator.recreateLine(0);

        REQUIRE(line == "print 123.45");
    }
}
