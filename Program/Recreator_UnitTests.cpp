/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Catch/catch.hpp>
#include "Code.h"
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
    recreator.addCommandKeyword();
}

void recreateNumber(Recreator &recreator);

OpCode test_num_opcode {recreateNumber};

void recreateNumber(Recreator &recreator)
{
    auto index = recreator.getOperand();
    auto number = recreator.getConstNum(index);
    recreator.pushString(std::to_string(number));
}


TEST_CASE("recreator", "[recreator]")
{
    ProgramCode code;
    Recreator recreator {code};

    SECTION("add a command keyword to the line")
    {
        code.addOpCode(test_opcode);

        auto line = recreator.recreateLine(0);

        REQUIRE(line == "test");
    }
    SECTION("call compile code function to complete coverage")
    {
        Compiler dummy_compiler;
        compileTest(dummy_compiler);
    }
    SECTION("add string")
    {
        code.addOpCode(test_num_opcode);
        auto index = code.addConstNum(123.45);
        code.addOperand(index);
        code.addOpCode(test_opcode);

        auto line = recreator.recreateLine(0);

        REQUIRE(line == "test 123.450000");
    }
}
