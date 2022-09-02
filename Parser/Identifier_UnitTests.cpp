/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <sstream>

#include <Catch/catch.hpp>
#include "Identifier.h"

using namespace std::string_view_literals;


TEST_CASE("parsing identifiers from a string", "[identifier]")
{
    SECTION("input stream does not contain an identifier")
    {
        std::istringstream iss {"%"};

        auto identifier = parseIdentifier(iss);

        REQUIRE(identifier.empty());
    }
    SECTION("parse a single letter")
    {
        std::istringstream iss {"P"};

        auto identifier = parseIdentifier(iss);

        REQUIRE(identifier == "P");
    }
    SECTION("parse multiple letters")
    {
        std::istringstream iss {"PRINT"};

        auto identifier = parseIdentifier(iss);

        REQUIRE(identifier == "PRINT");
    }
    SECTION("stop parsing on a non-alphabetic character")
    {
        std::istringstream iss {"VAR="};

        auto identifier = parseIdentifier(iss);

        REQUIRE(identifier == "VAR");
        REQUIRE(iss.peek() == '=');
    }
}
