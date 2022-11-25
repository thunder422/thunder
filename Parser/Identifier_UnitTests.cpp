/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <sstream>

#include <catch2/catch_test_macros.hpp>
#include "Parser.h"
#include "Token.h"

using namespace std::string_view_literals;


TEST_CASE("parsing identifiers from a string", "[identifier]")
{
    SECTION("input stream does not contain an identifier")
    {
        std::istringstream iss {"%"};

        auto identifier = Parser{iss}.parseIdentifier();

        REQUIRE(identifier.empty());
    }
    SECTION("parse a single letter")
    {
        std::istringstream iss {"P"};

        auto identifier = Parser{iss}.parseIdentifier();

        REQUIRE(identifier == "P");
    }
    SECTION("parse multiple letters")
    {
        std::istringstream iss {"PRINT"};

        auto identifier = Parser{iss}.parseIdentifier();

        REQUIRE(identifier == "PRINT");
    }
    SECTION("stop parsing on a non-alphabetic character")
    {
        std::istringstream iss {"VAR="};

        auto identifier = Parser{iss}.parseIdentifier();

        REQUIRE(identifier == "VAR");
        REQUIRE(iss.peek() == '=');
    }
}

TEST_CASE("get tokens (identifiers with modifiers)", "[tokens]")
{
    SECTION("get a token with no open parenthesis modifier")
    {
        std::istringstream iss {"VAR="};

        auto token = Parser{iss}.parseToken();

        REQUIRE(token.getValue() == "VAR");
        REQUIRE_FALSE(token.hasParen());
    }
    SECTION("get a token with a open parenthesis modifier")
    {
        std::istringstream iss {"FUNC(1"};

        auto token = Parser{iss}.parseToken();

        REQUIRE(token.getValue() == "FUNC");
        REQUIRE(token.hasParen());
        REQUIRE(iss.peek() == '1');
    }
    SECTION("no token returned if there is no tokwn")
    {
        std::istringstream iss {"123"};

        auto token = Parser{iss}.parseToken();

        REQUIRE_FALSE(token);
    }
}

TEST_CASE("parsing at end if stream", "[end]")
{
    SECTION("return column at end of stream")
    {
        std::istringstream iss {"identifier"};
        Parser parser {iss};

        parser.parseIdentifier();

        REQUIRE(parser.getColumn() == 10);
    }
}
