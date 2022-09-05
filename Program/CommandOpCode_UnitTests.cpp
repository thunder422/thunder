/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Catch/catch.hpp>
#include "CommandOpCode.h"


TEST_CASE("command op code]", "[cmdopcode")
{
    SECTION("instantiate a command op code with a name")
    {
        CommandOpCode test_opcode {"test"};

        REQUIRE(test_opcode.getValue() == 0);
    }
    SECTION("find a valid command from a keyword")
    {
        CommandOpCode print_opcode {"print"};
        CommandOpCode end_opcode {"end"};

        auto opcode = CommandOpCode::find("end");

        REQUIRE(opcode->getValue() == end_opcode.getValue());
    }
    SECTION("null pointer if keyword is not a command")
    {
        auto opcode = CommandOpCode::find("bad");

        REQUIRE(opcode == nullptr);
    }
}
