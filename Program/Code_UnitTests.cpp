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
    SECTION("compile and recreate a simple print statement")
    {
        std::istringstream iss {"print"};
        ProgramCode program_line {iss};

        REQUIRE(program_line.recreateLine(0) == "print");
    }
    SECTION("compile and recreate an end statement")
    {
        std::istringstream iss {"end"};
        ProgramCode program_line {iss};

        REQUIRE(program_line.recreateLine(0) == "end");
    }
}
