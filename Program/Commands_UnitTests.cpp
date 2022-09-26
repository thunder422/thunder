/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <sstream>
#include <Catch/catch.hpp>
#include <Compiler/Compiler.h>
#include "Code.h"
#include "Commands.h"
#include "OpCode.h"


TEST_CASE("commands", "[commands]")
{
    SECTION("find opcode for a command")
    {
        auto opcode = Commands::getOpCode("print");

        REQUIRE(opcode);
    }
    SECTION("no find for a non-command identifier")
    {
        auto opcode = Commands::getOpCode("invalid");

        REQUIRE_FALSE(opcode);
    }
    SECTION("get compile function")
    {
        auto opcode = Commands::getOpCode("print");
        void compilePrint(Compiler &compiler);

        auto print_compile_function = Commands::getCompileFunction(*opcode);

        REQUIRE(print_compile_function == compilePrint);
    }
    SECTION("compile print command")
    {
        ProgramCode code;
        std::istringstream iss {"print"};
        Compiler compiler {code, iss};
        auto opcode = Commands::getOpCode("print");

        Commands::getCompileFunction(*opcode)(compiler);

        REQUIRE(code.recreateLine(0) == "print");
    }
    SECTION("compile end")
    {
        ProgramCode code;
        std::istringstream iss {"end"};
        Compiler compiler {code, iss};
        auto opcode = Commands::getOpCode("end");

        Commands::getCompileFunction(*opcode)(compiler);

        REQUIRE(code.recreateLine(0) == "end");
    }
    SECTION("get keyword for an opcode")
    {
        auto opcode = Commands::getOpCode("print");

        REQUIRE(Commands::getKeyword(opcode->getValue()) == "print");
    }
}
