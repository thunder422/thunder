/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Catch/catch.hpp>
#include "Unit.h"


TEST_CASE("unit", "[unit]")
{
    ProgramUnit program;

    SECTION("default program contains a single line with 'end'")
    {
        auto line = program.recreateLine(0);

        REQUIRE(line == "1 end");
    }
}
