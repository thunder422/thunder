/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <sstream>
#include <Catch/catch.hpp>
#include "Parser.h"


TEST_CASE("parser", "[parser")
{
    SECTION("skip spaces")
    {
        std::istringstream iss {"   c"};
        Parser parser {iss};

        parser.skipWhiteSpace();

        REQUIRE(parser.getNextChar() == 'c');
    }
}
