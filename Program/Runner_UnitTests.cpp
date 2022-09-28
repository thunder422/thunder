/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <sstream>
#include <Catch/catch.hpp>
#include "Code.h"
#include "Runner.h"


extern OpCode end_opcode;

class TestProgram : public ProgramCode {
public:
    void compile(std::string program_line) {
        std::istringstream iss {program_line};
        compileLine(iss);
        addOpCode(end_opcode);
    }

private:
};

TEST_CASE("runner", "[runner]")
{
    TestProgram program;
    std::ostringstream oss;
    Runner runner {program, oss};

    SECTION("run a simple print statement")
    {
        program.compile("print");

        runner.runProgram();

        REQUIRE(oss.str() == "\nEnd Program.\n");
    }
    SECTION("run a print constant statement")
    {
        program.compile("print 123.45");

        runner.runProgram();

        REQUIRE(oss.str() == "123.45\nEnd Program.\n");
    }
}
