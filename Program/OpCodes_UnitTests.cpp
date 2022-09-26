/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Catch/catch.hpp>
#include "OpCode.h"
#include "OpCodes.h"


TEST_CASE("opcodes", "[opcodes]")
{
    SECTION("get recreate functions")
    {
        extern OpCode const_num_opcode;
        void recreateConstNum(Recreator &recreator);

        auto recreate_function = OpCodes::getRecreateFunction(const_num_opcode.getValue());

        REQUIRE(recreate_function == recreateConstNum);
    }
}
