/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <sstream>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include "Error.h"
#include "Parser.h"

using namespace std::string_view_literals;


TEST_CASE("parsing integer constants from a string", "[integers]")
{
    SECTION("input stream does not contain a constant (caller will determine action)")
    {
        std::istringstream iss {"%"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE_FALSE(number);
    }
    SECTION("parse a single digit number")
    {
        std::istringstream iss {"1"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(number);
        REQUIRE(*number == 1);
    }
    SECTION("parse a multiple digit number")
    {
        std::istringstream iss {"123"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(*number == 123);
    }
    SECTION("parse a negative number")
    {
        std::istringstream iss {"-234"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(*number == -234);
    }
    SECTION("terminate parsing at correct character")
    {
        std::istringstream iss {"345+"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(*number == 345);
        REQUIRE(iss.peek() == '+');
    }
}


TEST_CASE("parsing floating point constants from a string", "[doubles]")
{
    SECTION("parse a number with a decimal point")
    {
        std::istringstream iss {"0.5"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(*number == 0.5);
    }
    SECTION("parse a number with a decimal point at the beginning")
    {
        std::istringstream iss {".75"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(*number == 0.75);
    }
    SECTION("parse a number with a second decimal point (should ignore second one)")
    {
        std::istringstream iss {"0.1."};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(*number == 0.1);
        REQUIRE(iss.peek() == '.');
    }
    SECTION("parse a number with an exponent")
    {
        std::istringstream iss {"1e0"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(*number == 1.0);
    }
    SECTION("make sure parsing stops before a second 'E'")
    {
        std::istringstream iss {"1e0E"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(*number == 1.0);
        REQUIRE(iss.peek() == 'E');
    }
    SECTION("parse a number with a minus exponent")
    {
        std::istringstream iss {"1e-2"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(*number == 0.01);
    }
    SECTION("parse a number with a minus exponent terminated be a minus operator")
    {
        std::istringstream iss {"1e-2-"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(*number == 0.01);
        REQUIRE(iss.peek() == '-');
    }
    SECTION("parse a number with a plus exponent")
    {
        std::istringstream iss {"1e+2"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(*number == 100.0);
    }
}


TEST_CASE("handle leading zero of a constant correctly including errors", "[zero]")
{
    SECTION("check for an error when a leading zero is followed by another digit")
    {
        std::istringstream iss {"01"};

        SECTION("check for error message")
        {
            REQUIRE_THROWS_WITH(Parser{iss}.parseNumber(),
                "expected decimal point after leading zero");
        }
        SECTION("check for error column")
        {
            try {
                Parser{iss}.parseNumber();
            }
            catch (const Error &error) {
                REQUIRE(error.column == 1);
            }
        }
    }
    SECTION("check for the correct error column after another token")
    {
        std::istringstream iss {"token 01"};

        std::string skip_token;
        iss >> skip_token >> std::ws;
        try {
            Parser{iss}.parseNumber();
        }
        catch (const Error &error) {
            REQUIRE(error.what() == "expected decimal point after leading zero"sv);
            REQUIRE(error.column == 7);
        }
    }
    SECTION("check parsing ends when followed by a non-period non-digit")
    {
        std::istringstream iss {"0-"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(number);
        REQUIRE(*number == 0);
        REQUIRE(iss.peek() == '-');
    }
}


TEST_CASE("handle leading period of a constant correctly including errors", "[period]")
{
    SECTION("check for an error when a leading period is followed by another period")
    {
        std::istringstream iss {".."};

        SECTION("check for error message")
        {
            REQUIRE_THROWS_WITH(Parser{iss}.parseNumber(), "expected digit after decimal point");
        }
        SECTION("check for error column")
        {
            try {
                Parser{iss}.parseNumber();
            }
            catch (const Error &error) {
                REQUIRE(error.column == 1);
            }
        }
    }
}


TEST_CASE("check for correct exponent format", "[exponent]")
{
    SECTION("check for sign or digits at the start of an exponent")
    {
        std::istringstream iss {"1e."};

        SECTION("check for error message")
        {
            REQUIRE_THROWS_WITH(Parser{iss}.parseNumber(), "expected sign or digit for exponent");
        }
        SECTION("check for error column")
        {
            try {
                Parser{iss}.parseNumber();
            }
            catch (const Error &error) {
                REQUIRE(error.column == 2);
            }
        }
    }
    SECTION("error for possible EQV operator (all operators are symbols)")
    {
        std::istringstream iss {"1eq"};

        REQUIRE_THROWS_WITH(Parser{iss}.parseNumber(), "expected sign or digit for exponent");
    }
    SECTION("make sure there is a digit after an exponent sign")
    {
        std::istringstream iss {"1e+"};

        REQUIRE_THROWS_AS(Parser{iss}.parseNumber(), Error);
    }
    SECTION("check error message and column if no digit after exponent sign")
    {
        std::istringstream iss {"1e-A"};
        try {
            Parser{iss}.parseNumber();
        }
        catch (const Error &error) {
            REQUIRE(error.what() == "expected digit after exponent sign"sv);
            REQUIRE(error.column == 3);
        }
    }
    SECTION("check correct error column when terminated by end of line")
    {
        std::istringstream iss {"1e-"};
        try {
            Parser{iss}.parseNumber();
        }
        catch (const Error &error) {
            REQUIRE(error.column == 3);
        }

    }
}


TEST_CASE("look for possible exit conditions", "[exit]")
{
    SECTION("look for possible negate operator ('-' not followed by '.' or digit)")
    {
        std::istringstream iss {"-e"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE_FALSE(number);
    }
    SECTION("allow a period after a negative sign")
    {
        std::istringstream iss {"-.1"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(*number == -0.1);
    }
    SECTION("look for negate operator status (false if not negate operator)")
    {
        std::istringstream iss {"-1-"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(*number == -1);
        REQUIRE(iss.peek() == '-');
    }
    SECTION("if negate operator then next character should still be negate operator")
    {
        std::istringstream iss {"-e"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE_FALSE(number);
        REQUIRE(iss.peek() == '-');
    }
    SECTION("look for possible operator status (no operator starting with 'E')")
    {
        std::istringstream iss {"-1e1-"};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(*number == -10);
        REQUIRE(iss.peek() == '-');
    }
    SECTION("look for possible operator status (all operators are symbols)")
    {
        std::istringstream iss {"-1eqv"};

        REQUIRE_THROWS_WITH(Parser{iss}.parseNumber(), "expected sign or digit for exponent");
    }
    SECTION("check when 'E' character not followed by a another letter throws an error")
    {
        std::istringstream iss {"-1e$"};

        REQUIRE_THROWS_WITH(Parser{iss}.parseNumber(), "expected sign or digit for exponent");
    }
}


TEST_CASE("check other numeric constants from the IBCP tests", "[other]")
{
    SECTION("various error tests")
    {
        struct ErrorTest {
            const char *input;
            const char *expected_what;
            unsigned expected_column;
        };

        std::vector<ErrorTest> error_tests = {
            {".A", "expected digit after decimal point", 1},
            {".e", "expected digit after decimal point", 1},
            {"..01", "expected digit after decimal point", 1},
            {".", "expected digit after decimal point", 1},
            {"1e", "expected sign or digit for exponent", 2}
        };

        SECTION("verify correct error is thrown")
        {
            for (auto &test : error_tests) {
                std::istringstream iss {test.input};

                CAPTURE(test.input);
                REQUIRE_THROWS_WITH(Parser{iss}.parseNumber(), test.expected_what);
            }
        }
        SECTION("verify correct error column")
        {
            for (auto &test : error_tests) {
                std::istringstream iss {test.input};

                CAPTURE(test.input);
                try {
                    Parser{iss}.parseNumber();
                }
                catch (const Error &error) {
                    REQUIRE(error.column == test.expected_column);
                }
            }
        }
    }

    SECTION("miscellenous test")
    {
        std::istringstream iss {"1.."};

        auto number = Parser{iss}.parseNumber();

        REQUIRE(*number == 1.0);
        REQUIRE(iss.peek() == '.');
    }
}
