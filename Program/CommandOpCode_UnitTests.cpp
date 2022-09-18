/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <Catch/catch.hpp>
#include "CommandOpCode.h"


class Compiler {
public:
    bool was_called {false};
};

void compileCode(Compiler &code)
{
    code.was_called = true;
}

class Recreator {
public:
    bool was_called {false};
};

void recreateCode(Recreator &recreator)
{
    recreator.was_called = true;
}


TEST_CASE("command op code]", "[cmdopcode")
{
    SECTION("instantiate a command op code with a name")
    {
        CommandOpCode test_opcode {"test", compileCode, recreateCode};

        REQUIRE(test_opcode.getValue() == 0);
    }
    SECTION("find a valid command from a keyword")
    {
        CommandOpCode print_opcode {"print", compileCode, recreateCode};
        CommandOpCode end_opcode {"end", compileCode, recreateCode};

        SECTION("find an opcode")
        {
            auto opcode = CommandOpCode::find("end");

            REQUIRE(opcode->getValue() == end_opcode.getValue());
        }
        SECTION("compile an opcode")
        {
            Compiler test_compiler;

            CommandOpCode::compile(end_opcode, test_compiler);

            REQUIRE(test_compiler.was_called);
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
    SECTION("recreate mechanism")
    {
        Recreator test_recreator;
        CommandOpCode command_opcode {"command", compileCode, recreateCode};

        OpCode::recreate(command_opcode.getValue(), test_recreator);

        REQUIRE(test_recreator.was_called);
    }
}
