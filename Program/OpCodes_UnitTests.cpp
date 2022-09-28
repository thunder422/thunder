/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Catch/catch.hpp>
#include "OpCode.h"
#include "OpCodes.h"


class Recreator { };

TEST_CASE("opcodes", "[opcodes]")
{
    SECTION("get recreate function")
    {
        extern OpCode const_num_opcode;
        void recreateConstNum(Recreator &recreator);

        auto recreate_function = OpCodes::getRecreateFunction(const_num_opcode.getValue());

        REQUIRE(recreate_function == recreateConstNum);
    }
    SECTION("get run function")
    {
        extern OpCode const_num_opcode;
        void runConstNum(Runner &runner);

        auto run_function = OpCodes::getRunFunction(const_num_opcode.getValue());

        REQUIRE(run_function == runConstNum);
    }
    SECTION("call recreate nothing function")
    {
        extern OpCode print_num_opcode;

        auto recreate_nothing_function = OpCodes::getRecreateFunction(print_num_opcode.getValue());

        Recreator dummy_recreator;
        recreate_nothing_function(dummy_recreator);
    }
}
