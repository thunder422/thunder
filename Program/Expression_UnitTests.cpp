/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <sstream>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <Compiler/Compiler.h>
#include <Parser/Error.h>
#include "Code.h"
#include "Runner.h"
#include "View.h"


extern OpCode end_opcode;

class ExpressionTester {
public:
    ExpressionTester() :
        compiler {code, iss}, runner {code, oss} { }
    void compile(const char *expression) {
        iss.str(expression);
        compiler.compileExpression();
    }
    std::string recreate() {
        ProgramView view {0, static_cast<size_t>(std::distance(code.begin(), code.end()))};
        return code.recreateLine(view);
    }
    double run() {
        code.addOpCode(end_opcode);
        runner.runProgram();
        return runner.popNumber();
    }

private:
    std::istringstream iss;
    ProgramCode code;
    Compiler compiler;
    std::ostringstream oss;
    Runner runner;
};


TEST_CASE("compile, recreate and run unary negate operator", "[negate]")
{
    ExpressionTester expression;

    SECTION("a single number constant with no unary operators")
    {
        expression.compile("123.45");

        REQUIRE(expression.recreate() == "123.45");
        REQUIRE(expression.run() == 123.45);
    }
    SECTION("a single negative constant (minus interpreted as sign of constant, not an operator")
    {
        expression.compile("-123.45");

        REQUIRE(expression.recreate() == "-123.45");
        REQUIRE(expression.run() == -123.45);
    }
    SECTION("a negate operator in front of a constant")
    {
        expression.compile("--123.45");

        REQUIRE(expression.recreate() == "--123.45");
        REQUIRE(expression.run() == 123.45);
    }
    SECTION("a negate operator in front of a constant (space required so not interpreted as sign)")
    {
        expression.compile("- 123.45");

        REQUIRE(expression.recreate() == "- 123.45");
        REQUIRE(expression.run() == -123.45);
    }
    SECTION("multiple unary operators")
    {
        expression.compile("---123.45");

        REQUIRE(expression.recreate() == "---123.45");
        REQUIRE(expression.run() == -123.45);
    }
    SECTION("error if there is a unary operator not followed by an operand")
    {
        auto input = "- ,";

        SECTION("check for error message")
        {
            REQUIRE_THROWS_WITH(expression.compile(input),
                "expected unary operator or operand");
        }
        SECTION("check for error column")
        {
            try {
                expression.compile(input);
            }
            catch (const ParseError &error) {
                REQUIRE(error.column == 2);
            }
        }
    }
}
