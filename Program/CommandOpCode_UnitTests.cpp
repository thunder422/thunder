/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Catch/catch.hpp>
#include "CommandOpCode.h"


class ProgramCode { };

bool compile_was_called;

void testCompileFunction(ProgramCode &)
{
    compile_was_called = true;
}

TEST_CASE("command op code]", "[cmdopcode")
{
    SECTION("instantiate a command op code with a name")
    {
        CommandOpCode test_opcode {"test", testCompileFunction};

        REQUIRE(test_opcode.getValue() == 0);
    }
    SECTION("find a valid command from a keyword")
    {
        CommandOpCode print_opcode {"print", testCompileFunction};
        CommandOpCode end_opcode {"end", testCompileFunction};

        SECTION("find an opcode")
        {
            auto opcode = CommandOpCode::find("end");

            REQUIRE(opcode->getValue() == end_opcode.getValue());
        }
        SECTION("compile an opcode")
        {
            ProgramCode dummy_code;
            compile_was_called = false;

            CommandOpCode::compile(end_opcode, dummy_code);

            REQUIRE(compile_was_called);
        }
        SECTION("get keyword for an opcode")
        {
            REQUIRE(CommandOpCode::getKeyword(end_opcode.getValue()) == "end");
        }
    }
    SECTION("null pointer if keyword is not a command")
    {
        auto opcode = CommandOpCode::find("bad");

        REQUIRE(opcode == nullptr);
    }
    SECTION("get command keyword")
    {
        CommandOpCode test_opcode {"test", testCompileFunction};

        auto keyword = CommandOpCode::getKeyword(test_opcode.getValue());

        REQUIRE(keyword == "test");
    }
}
