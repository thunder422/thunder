/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Catch/catch.hpp>
#include "OpCode.h"


TEST_CASE("generate unique codes for each unique program element", "[opcodes]")
{
    SECTION("first op code has value of zero")
    {
        OpCode first_op_code;

        REQUIRE(first_op_code.getValue() == 0);
    }
    SECTION("second op code has next value")
    {
        OpCode second_op_code;

        REQUIRE(second_op_code.getValue() == 1);
    }
}
