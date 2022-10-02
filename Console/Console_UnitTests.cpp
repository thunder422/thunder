/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <sstream>
#include <catch2/catch_test_macros.hpp>
#include "Console.h"


TEST_CASE("console", "[console]")
{
    std::istringstream iss;
    std::ostringstream oss;
    Console console {iss, oss};

    SECTION("ready prompt, command prompt, quit input and exit")
    {
        iss.str("q\n");

        console.run();

        auto expected_output {
            "Ready\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("list the default program containing the automatic end statement and exit")
    {
        iss.str("l\nq\n");

        console.run();

        auto expected_output {
            "Ready\n"
            ":1 end\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
}
