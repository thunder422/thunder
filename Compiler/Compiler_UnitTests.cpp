/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Catch/catch.hpp>
#include <sstream>
#include <Parser/Error.h>
#include <Program/Code.h>
#include <Program/Recreator.h>
#include "Compiler.h"


extern OpCode print_opcode;


TEST_CASE("compile print command", "[print]")
{
    ProgramCode code;

    SECTION("compile a simple print command with no arguments")
    {
        std::istringstream iss {"print"};
        Compiler compiler {code, iss};

        compiler.compileLine();

        REQUIRE(code.recreateLine(0) == "print");
    }
    SECTION("compile a print command with a single constant")
    {
        std::istringstream iss {"print 123"};
        Compiler compiler {code, iss};

        compiler.compileLine();

        REQUIRE(code.recreateLine(0) == "print 123");
    }
}

TEST_CASE("compile end command", "[end]")
{
    ProgramCode code;

    SECTION("compile a simple end command")
    {
        std::istringstream iss {"end"};
        Compiler compiler {code, iss};

        compiler.compileLine();

        REQUIRE(code.recreateLine(0) == "end");
    }
}

TEST_CASE("compile an invalid command", "[invalid]")
{
    ProgramCode code;

    SECTION("expect an error if keyword is not a valid command")
    {
        std::istringstream iss {"bogus"};
        Compiler compiler {code, iss};


        SECTION("check for error message")
        {
            REQUIRE_THROWS_WITH(compiler.compileLine(),
                "expected valid command or variable for assignment");
        }
        SECTION("check for error column")
        {
            try {
                compiler.compileLine();
            }
            catch (const ParseError &error) {
                REQUIRE(error.column == 0);
            }
        }
    }
}
