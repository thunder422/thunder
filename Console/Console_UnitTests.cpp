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

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("list the default program containing the automatic end statement and exit")
    {
        auto input {
            "l\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":1 end\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("insert a print command, list it, and exit")
    {
        auto input {
            "i\n"
            "print 123\n"
            "\n"
            "l\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":1 "
            "2 "
            ":1 print 123\n"
            "2 end\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("insert print commands, run them, and exit")
    {
        auto input {
            "i\n"
            "print 1.23\n"
            "print 4.56\n"
            "\n"
            "r\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":1 "
            "2 "
            "3 "
            ":1.23\n"
            "4.56\n"
            "End Program.\n"
            "Ready\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("insert by default continues at same line from last insert")
    {
        auto input {
            "i\n"
            "print 1.23\n"
            "print 4.56\n"
            "\n"
            "i\n"
            "print 7.89\n"
            "\n"
            "l\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":1 "
            "2 "
            "3 "
            ":3 "
            "4 "
            ":1 print 1.23\n"
            "2 print 4.56\n"
            "3 print 7.89\n"
            "4 end\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
}
