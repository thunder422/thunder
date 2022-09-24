/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Catch/catch.hpp>
#include <sstream>
#include <Program/Code.h>
#include <Program/CommandOpCode.h>
#include <Program/Recreator.h>
#include "Compiler.h"


extern CommandOpCode print_opcode;


TEST_CASE("compile opcode", "[opcode]")
{
    SECTION("compile a command that adds a single opcode to the program")
    {
        ProgramCode code;
        std::istringstream iss {"print"};
        Compiler compiler {code, iss};

        compiler.compileLine();

        REQUIRE(code.recreateLine(0) == "print");
    }
}

TEST_CASE("compile expression", "[expression]")
{
    SECTION("compile a command that takes an expression")
    {
        ProgramCode code;
        std::istringstream iss {"print 123"};
        Compiler compiler {code, iss};

        compiler.compileLine();

        REQUIRE(code.recreateLine(0) == "print 123");
    }
}
