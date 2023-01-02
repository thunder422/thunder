/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <sstream>
#include <catch2/catch_test_macros.hpp>
#include "CommandParser.h"
#include "Console.h"


enum class CommandId : int {
    Test,
    Only
};

class TestClient : public Client {
public:
    std::optional<CommandId> getCommandId(char c, std::optional<char> next) const override {
        if (c == 't') {
            next_char = next;
            return CommandId::Test;
        }
        if (c == 'o' && !next) {
            return CommandId::Only;
        }
        return {};
    }

    mutable std::optional<char> next_char;
};


TEST_CASE("console", "[console]")
{
    std::istringstream iss;
    std::ostringstream oss;
    Console console {iss, oss};

    SECTION("ready prompt, command prompt, quit input and exit")
    {
        iss.str("q\n");

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("list the default program containing the automatic end statement and exit")
    {
        auto input {
            "l\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":1 end\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("insert a print command, list it, and exit")
    {
        auto input {
            "i\n"
            "print 123\n"
            "\n"
            "l\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":1 "
            "2 "
            ":1 print 123\n"
            "2 end\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("insert print commands, run them, and exit")
    {
        auto input {
            "i\n"
            "print 1.23\n"
            "print 4.56\n"
            "\n"
            "r\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":1 "
            "2 "
            "3 "
            ":1.23\n"
            "4.56\n"
            "End Program.\n"
            "Ready\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("insert by default continues at same line from last insert")
    {
        auto input {
            "i\n"
            "print 1.23\n"
            "print 4.56\n"
            "\n"
            "i\n"
            "print 7.89\n"
            "\n"
            "l\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":1 "
            "2 "
            "3 "
            ":3 "
            "4 "
            ":1 print 1.23\n"
            "2 print 4.56\n"
            "3 print 7.89\n"
            "4 end\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("insert commands, insert at a line number, list them, and exit")
    {
        auto input {
            "i\n"
            "print 1.11\n"
            "print 3.33\n"
            "\n"
            "i2\n"
            "print 2.22\n"
            "\n"
            "l\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":1 "
            "2 "
            "3 "
            ":2 "
            "3 :1 print 1.11\n"
            "2 print 2.22\n"
            "3 print 3.33\n"
            "4 end\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("error if try to insert at a non-existing line number")
    {
        auto input {
            "i\n"
            "print 1.11\n"
            "print 2.22\n"
            "\n"
            "i4\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":1 "
            "2 "
            "3 "
            ":"
            "  ^-- invalid line number '4' (valid lines 1-3)!\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("insert at line 0 is also invalid")
    {
        auto input {
            "i0\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":"
            "  ^-- invalid line number '0' (valid line 1)!\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("insert command with line number in front of 'i'")
    {
        auto input {
            "i\n"
            "print 1.11\n"
            "print 3.33\n"
            "\n"
            "2i\n"
            "print 2.22\n"
            "\n"
            "l\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":1 "
            "2 "
            "3 "
            ":2 "
            "3 :1 print 1.11\n"
            "2 print 2.22\n"
            "3 print 3.33\n"
            "4 end\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("some commands don't allow a number in front of command")
    {
        auto input {
            "i\n"
            "print 1.11\n"
            "\n"
            "2r\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":1 "
            "2 "
            ":"
            " ^-- command does not allow a line number\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("some commands don't allow any arguments")
    {
        auto input {
            "r2\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":"
            "  ^-- command does not allow arguments\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("line number only enters insert at that line")
    {
        auto input {
            "i\n"
            "print 1.11\n"
            "print 3.33\n"
            "\n"
            "2\n"
            "print 2.22\n"
            "\n"
            "l\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":1 "
            "2 "
            "3 "
            ":2 "
            "3 :1 print 1.11\n"
            "2 print 2.22\n"
            "3 print 3.33\n"
            "4 end\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("line number plus statement inserts at that line and continues insert")
    {
        auto input {
            "i\n"
            "print 1.11\n"
            "print 3.33\n"
            "\n"
            "2print 2.22\n"
            "\n"
            "l\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":1 "
            "2 "
            "3 "
            ":"
            "3 :1 print 1.11\n"
            "2 print 2.22\n"
            "3 print 3.33\n"
            "4 end\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
}

TEST_CASE("random number generator", "[random]")
{
    std::istringstream iss;
    std::ostringstream oss;
    Console console {iss, oss};

    SECTION("random sequence should repeat each time program is run")
    {
        auto input {
            "i\n"
            "print rnd(1)\n"
            "print rnd(1)\n"
            "print rnd(1)\n"
            "print rnd(0)\n"
            "\n"
            "r\n"
            "r\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":1 "
            "2 "
            "3 "
            "4 "
            "5 "
            ":"
            "0.786820954867802\n"
            "0.2504803406880287\n"
            "0.7106712289786555\n"
            "0.7106712289786555\n"
            "End Program.\n"
            "Ready\n"
            ":"
            "0.786820954867802\n"
            "0.2504803406880287\n"
            "0.7106712289786555\n"
            "0.7106712289786555\n"
            "End Program.\n"
            "Ready\n"
            ":"
            "Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
}

TEST_CASE("console command parser", "[commands]")
{
    TestClient client;
    CommandParser parser;

    SECTION("parse numbers")
    {
        SECTION("parse a number from the input")
        {
            parser.setInput("123");

            auto number = parser.getNumber();

            REQUIRE(number);
            REQUIRE(*number == 123);
        }
        SECTION("no number return if there is no number in the input")
        {
            parser.setInput("");

            auto number = parser.getNumber();

            REQUIRE_FALSE(number);
        }
        SECTION("parse a number with preceding spaces")
        {
            parser.setInput(" 123");

            auto number = parser.getNumber();

            REQUIRE(number);
            REQUIRE(*number == 123);
        }
    }
    SECTION("parse commands")
    {
        SECTION("no command id if input is empty")
        {
            parser.setInput("");

            auto id = parser.getCommand(client);

            REQUIRE_FALSE(id);
        }
        SECTION("command id if valid command found")
        {
            parser.setInput("t");

            auto id = parser.getCommand(client);

            REQUIRE(id);
            REQUIRE(*id == CommandId::Test);
        }
        SECTION("invalid command id if for bad commands")
        {
            parser.setInput("x");

            auto id = parser.getCommand(client);

            REQUIRE_FALSE(id);
        }
        SECTION("make sure there is nothing after a command")
        {
            SECTION("command at end of input")
            {
                parser.setInput("t");
                parser.getCommand(client);

                auto is_at_end = parser.isAtEnd();

                REQUIRE(is_at_end);
            }
            SECTION("allow spaces after command before end of input")
            {
                parser.setInput("t  ");
                parser.getCommand(client);

                auto is_at_end = parser.isAtEnd();

                REQUIRE(is_at_end);
            }
        }
    }
    SECTION("parse a number followed by a command")
    {
        SECTION("parse number first")
        {
            parser.setInput("234t");

            auto number = parser.getNumber();

            REQUIRE(number);
            REQUIRE(*number == 234);
        }
        SECTION("parse command after number")
        {
            parser.setInput("234t");
            parser.getNumber();

            auto id = parser.getCommand(client);

            REQUIRE(id);
            REQUIRE(*id == CommandId::Test);
        }
        SECTION("parse command after number with space")
        {
            parser.setInput("234 t");
            parser.getNumber();

            auto id = parser.getCommand(client);

            REQUIRE(id);
            REQUIRE(*id == CommandId::Test);
        }
        SECTION("no next character if nothing after command")
        {
            parser.setInput("234t");
            parser.getNumber();
            parser.getCommand(client);

            REQUIRE_FALSE(client.next_char);
        }
    }
    SECTION("parse after a valid command")
    {
        SECTION("with a number after command")
        {
            parser.setInput("t345");

            auto id = parser.getCommand(client);

            REQUIRE(id);
            REQUIRE(*id == CommandId::Test);
        }
        SECTION("get number after command")
        {
            parser.setInput("t345");
            parser.getCommand(client);

            auto number = parser.getNumber();

            REQUIRE(number);
            REQUIRE(*number == 345);
        }
    }
    SECTION("parse a number followed by a string (statement)")
    {
        SECTION("parse number first")
        {
            parser.setInput("12test");

            auto number = parser.getNumber();

            REQUIRE(number);
            REQUIRE(*number == 12);
        }
        SECTION("current and next character passed to client when getting command id")
        {
            parser.setInput("12t34");
            parser.getNumber();

            auto id = parser.getCommand(client);

            REQUIRE(id);
            REQUIRE(*id == CommandId::Test);
            REQUIRE(client.next_char);
            REQUIRE(*client.next_char == '3');
        }
        SECTION("only valid command if nothing after command character")
        {
            parser.setInput("12o");
            parser.getNumber();

            auto id = parser.getCommand(client);

            REQUIRE(id);
            REQUIRE(*id == CommandId::Only);
        }
        SECTION("number followed by a statement")
        {
            parser.setInput("12o23");
            parser.getNumber();

            SECTION("not a valid command if characters after command")
            {
                auto id = parser.getCommand(client);

                REQUIRE_FALSE(id);
            }
            SECTION("get statement after number (remainder of input)")
            {
                auto remainder = parser.getRemainder();

                REQUIRE(remainder == "o23");
            }
        }
    }
}

TEST_CASE("error reporting", "[errors]")
{
    std::istringstream iss;
    std::ostringstream oss;
    Console console {iss, oss};

    SECTION("insert a command with an error")
    {
        auto input {
            "i\n"
            "print (5+\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":1 "
            "           ^-- expected unary operator or operand\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("enter a bad command")
    {
        auto input {
            "y\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":"
            " ^-- unknown command 'y'\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("enter a bad command with preceding spaces")
    {
        auto input {
            "  y\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":"
            "   ^-- unknown command 'y'\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("invalid line number")
    {
        auto input {
            "iabc\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":"
            "  ^-- invalid line number 'abc' (valid line 1)!\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
    SECTION("invalid initial line number")
    {
        auto input {
            "34i\n"
            "q\n"
        };
        iss.str(input);

        console.commandLoop();

        auto expected_output {
            "Ready\n"
            ":"
            " ^-- invalid line number '34' (valid line 1)!\n"
            ":Good-bye.\n"
        };
        REQUIRE(oss.str() == expected_output);
    }
}
