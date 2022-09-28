/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <sstream>

#include <Catch/catch.hpp>
#include "Code.h"

using namespace std::string_view_literals;


TEST_CASE("compile line", "[compile]")
{
    ProgramCode program_line;

    SECTION("compile and recreate a simple print statement")
    {
        std::istringstream iss {"print"};
        program_line.compileLine(iss);

        REQUIRE(program_line.recreateLine(0) == "print");
    }
    SECTION("compile and recreate an end statement")
    {
        std::istringstream iss {"end"};
        program_line.compileLine(iss);

        REQUIRE(program_line.recreateLine(0) == "end");
    }
}


TEST_CASE("modify program", "[modify]")
{
    ProgramCode code;

    SECTION("add constant numbers")
    {
        SECTION("first constant number has index of 0")
        {
            auto index = code.addConstNum(123);

            REQUIRE(index == 0);
        }
        SECTION("second constant number has index of 1")
        {
            code.addConstNum(123);
            auto index = code.addConstNum(456);

            REQUIRE(index == 1);
        }
        SECTION("read constant number by index")
        {
            code.addConstNum(123);
            code.addConstNum(456);

            REQUIRE(code.getConstNum(1) == 456);
        }
    }
    SECTION("add operand to code")
    {
        code.addOperand(135);

        REQUIRE(code.getWord(0));
    }
}
