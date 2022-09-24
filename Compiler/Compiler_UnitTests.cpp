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


void compileTest(Compiler &compiler);
void recreateTest(Recreator &recreator);
CommandOpCode test_opcode {"test", compileTest, recreateTest};

void compileTest(Compiler &compiler)
{
    compiler.addOpCode(test_opcode);
}

void recreateTest(Recreator &recreator)
{
    recreator.addCommandKeyword();
}


TEST_CASE("compile opcode", "[opcode]")
{
    SECTION("compile a command that adds a single opcode to the program")
    {
        ProgramCode code;
        std::istringstream iss {"test"};
        Compiler compiler {code, iss};

        compiler.compileLine();

        auto line = code.recreateLine(0);
        REQUIRE(line == "test");
    }
}

void compileSecond(Compiler &compiler);
void recreateSecond(Recreator &recreator);
CommandOpCode second_opcode {"second", compileSecond, recreateSecond};

void compileSecond(Compiler &compiler)
{
    compiler.compileExpression();
    compiler.addOpCode(second_opcode);
}

void recreateSecond(Recreator &recreator)
{
    recreator.addCommandKeyword();
}

TEST_CASE("compile expression", "[expression]")
{
    SECTION("compile a command that takes an expression")
    {
        ProgramCode code;
        std::istringstream iss {"second 123"};
        Compiler compiler {code, iss};

        compiler.compileLine();

        auto line = code.recreateLine(0);
        REQUIRE(line == "second 123");
    }
}
