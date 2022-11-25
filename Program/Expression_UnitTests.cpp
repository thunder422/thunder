/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <sstream>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <Compiler/Compiler.h>
#include <Parser/Error.h>
#include "Code.h"
#include "Runner.h"
#include "View.h"

using namespace Catch::Matchers;


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

TEST_CASE("compile, recreate and run binary add operator", "[add]")
{
    ExpressionTester expression;

    SECTION("add operator")
    {
        expression.compile("12.34+56.78");

        REQUIRE(expression.recreate() == "12.34 + 56.78");
        REQUIRE(expression.run() == 69.12);
    }
    SECTION("add operator with negate operator")
    {
        expression.compile("- 12.34+56.78");

        REQUIRE(expression.recreate() == "- 12.34 + 56.78");
        REQUIRE(expression.run() == 44.44);
    }
}

TEST_CASE("compile, recreate and run binary sub operator", "[sub]")
{
    ExpressionTester expression;

    SECTION("subtract operator")
    {
        expression.compile("43.21-56.78");

        REQUIRE(expression.recreate() == "43.21 - 56.78");
        REQUIRE(expression.run() == -13.57);
    }
    SECTION("add and subtract associate left to right")
    {
        expression.compile("1-2+3");

        REQUIRE(expression.recreate() == "1 - 2 + 3");
        REQUIRE(expression.run() == 2);
    }
}

TEST_CASE("compile, recreate and run binary multiple operator", "[mul]")
{
    ExpressionTester expression;

    SECTION("multiply operator")
    {
        expression.compile("12.34*56.78");

        REQUIRE(expression.recreate() == "12.34 * 56.78");
        REQUIRE(expression.run() == 700.6652);
    }
    SECTION("multiply operator with add operator")
    {
        expression.compile("1.2+3.4*5.6");

        REQUIRE(expression.recreate() == "1.2 + 3.4 * 5.6");
        REQUIRE(expression.run() == 20.24);
    }
    SECTION("multiply operator with add operator with spaces")
    {
        expression.compile("1.2 * 3.4 + 5.6");

        REQUIRE(expression.recreate() == "1.2 * 3.4 + 5.6");
        REQUIRE(expression.run() == 9.68);
    }
}

TEST_CASE("compile, recreate and run binary divide operator", "[div]")
{
    ExpressionTester expression;

    SECTION("divide operator")
    {
        expression.compile("12.34/5.678");

        REQUIRE(expression.recreate() == "12.34 / 5.678");
        REQUIRE(expression.run() == 2.173300457907714);
    }
}

TEST_CASE("compile, recreate and run binary modulo operator", "[mod]")
{
    ExpressionTester expression;

    SECTION("modulo operator")
    {
        expression.compile("12.34%5.678");

        REQUIRE(expression.recreate() == "12.34 % 5.678");
        REQUIRE(expression.run() == 0.984);
    }
}

TEST_CASE("compile, recreate and run binary integer divide operator", "[idiv]")
{
    ExpressionTester expression;

    SECTION("integer divide operator")
    {
        expression.compile(R"(12.34\5.678)");

        REQUIRE(expression.recreate() == R"(12.34 \ 5.678)");
        REQUIRE(expression.run() == 2);
    }
}

TEST_CASE("compile, recreate and run binary power operator", "[pow]")
{
    ExpressionTester expression;

    SECTION("power operator")
    {
        expression.compile("2 ^ 3");

        REQUIRE(expression.recreate() == "2 ^ 3");
        REQUIRE(expression.run() == 8);
    }
    SECTION("power operator is higher precedence than negate operator")
    {
        expression.compile("- 3 ^ 2");

        REQUIRE(expression.recreate() == "- 3 ^ 2");
        REQUIRE(expression.run() == -9);
    }
    SECTION("power operator associates left to right")
    {
        expression.compile("4 ^ 3 ^ 2");

        REQUIRE(expression.recreate() == "4 ^ 3 ^ 2");
        REQUIRE(expression.run() == 4096);
    }
}

TEST_CASE("compile, recreate and run expressions with parentheses", "[parens]")
{
    ExpressionTester expression;

    SECTION("multiply operator with add operator on left side")
    {
        expression.compile("(1.2+3.4)*5.6");

        REQUIRE(expression.recreate() == "(1.2 + 3.4) * 5.6");
        REQUIRE_THAT(expression.run(), WithinAbs(25.76, 0.00001));
    }
    SECTION("multiply operator with add operator on right side")
    {
        expression.compile("1.2*(3.4+5.6)");

        REQUIRE(expression.recreate() == "1.2 * (3.4 + 5.6)");
        REQUIRE_THAT(expression.run(), WithinAbs(10.8, 0.00001));
    }
    SECTION("add operator with add operator on left side (removes parentheses")
    {
        expression.compile("(1.2+3.4)+5.6");

        REQUIRE(expression.recreate() == "1.2 + 3.4 + 5.6");
        REQUIRE_THAT(expression.run(), WithinAbs(10.2, 0.00001));
    }
    SECTION("add operator with add operator on right side (keeps parentheses")
    {
        expression.compile("1.2+(3.4+5.6)");

        REQUIRE(expression.recreate() == "1.2 + (3.4 + 5.6)");
        REQUIRE_THAT(expression.run(), WithinAbs(10.2, 0.00001));
    }
    SECTION("expect an error if closing parenthesis is missing")
    {
        auto missing_paren = "1.2+(3.4+5.6;";

        SECTION("check for error message")
        {
            REQUIRE_THROWS_WITH(expression.compile(missing_paren),
                "expected binary operator or closing parenthesis");
        }
        SECTION("check for error column")
        {
            try {
                expression.compile(missing_paren);
            }
            catch (const ParseError &error) {
                REQUIRE(error.column == 12);
            }
        }
    }
}

TEST_CASE("compile, recreate and run expressions with functions", "[functions]")
{
    ExpressionTester expression;

    SECTION("absolute function")
    {
        expression.compile("abs(-1.2)");

        REQUIRE(expression.recreate() == "abs(-1.2)");
        REQUIRE(expression.run() == 1.2);
    }
    SECTION("integer function (largest whole integer)")
    {
        SECTION("positive argument")
        {
            expression.compile("int(1.8)");

            REQUIRE(expression.recreate() == "int(1.8)");
            REQUIRE(expression.run() == 1);
        }
        SECTION("negative argument")
        {
            expression.compile("int(-1.8)");

            REQUIRE(expression.recreate() == "int(-1.8)");
            REQUIRE(expression.run() == -2);
        }
    }
    SECTION("square root function")
    {
        expression.compile("sqr(25)");

        REQUIRE(expression.recreate() == "sqr(25)");
        REQUIRE(expression.run() == 5);
    }
    SECTION("natural logarithm function")
    {
        expression.compile("log(25)");

        REQUIRE(expression.recreate() == "log(25)");
        REQUIRE(expression.run() == 3.21887582486820075);
    }
    SECTION("base-e exponential function")
    {
        expression.compile("exp(3.2188758248682006)");

        REQUIRE(expression.recreate() == "exp(3.2188758248682006)");
        REQUIRE_THAT(expression.run(), WithinAbs(25, 0.00001));
    }
    SECTION("signum function")
    {
        SECTION("zero value")
        {
            expression.compile("sgn(0)");

            REQUIRE(expression.recreate() == "sgn(0)");
            REQUIRE(expression.run() == 0);
        }
        SECTION("positive value")
        {
            expression.compile("sgn(2.5)");

            REQUIRE(expression.recreate() == "sgn(2.5)");
            REQUIRE(expression.run() == 1);
        }
        SECTION("negative value")
        {
            expression.compile("sgn(-5.2)");

            REQUIRE(expression.recreate() == "sgn(-5.2)");
            REQUIRE(expression.run() == -1);
        }
    }
    SECTION("cosine function")
    {
        expression.compile("cos(2)");

        REQUIRE(expression.recreate() == "cos(2)");
        REQUIRE(expression.run() == -0.4161468365471424);
    }
    SECTION("sine function")
    {
        expression.compile("sin(2)");

        REQUIRE(expression.recreate() == "sin(2)");
        REQUIRE(expression.run() == 0.9092974268256817);
    }
    SECTION("tangent function")
    {
        expression.compile("tan(2)");

        REQUIRE(expression.recreate() == "tan(2)");
        REQUIRE(expression.run() == -2.185039863261519);
    }
    SECTION("arc tangent function")
    {
        expression.compile("atn(2)");

        REQUIRE(expression.recreate() == "atn(2)");
        REQUIRE(expression.run() == 1.1071487177940905);
    }
    SECTION("functions can be entered with no parentheses (will be added)")
    {
        expression.compile("sin 2 + 1");

        REQUIRE(expression.recreate() == "sin(2) + 1");
        REQUIRE(expression.run() == 1.9092974268256817);
    }
}
