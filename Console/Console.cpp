/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <Parser/Error.h>
#include <Program/Find.h>
#include "CommandParser.h"
#include "Console.h"

using namespace std::string_literals;
using namespace std::string_view_literals;

enum class CommandId : int {
    Insert,
    List,
    Run,
    Quit
};

enum class Argument {
    None,
    Number
};

std::unordered_map<CommandId, Argument> id_to_argument {
    {CommandId::Insert, Argument::Number},
    {CommandId::List, Argument::None},
    {CommandId::Run, Argument::None},
    {CommandId::Quit, Argument::None}
};

class ParserClient : public Client {
public:
    ParserClient(CommandParser &parser) : parser {parser} { }
    std::optional<CommandId> getCommandId(char c, std::optional<char> next) const override;
    void setHasInitialNumber(bool value) {
        has_initial_number = value;
    }

private:
    CommandParser &parser;
    bool has_initial_number {false};
};

using ConsoleFunction = void(Console::*)();

std::unordered_map<CommandId, ConsoleFunction> id_to_function = {
    {CommandId::Insert, &Console::insert},
    {CommandId::List, &Console::list},
    {CommandId::Run, &Console::run},
    {CommandId::Quit, &Console::quit}
};

CommandParser command_parser;

Console::Console(std::istream &is, std::ostream &os) :
    is {is},
    os {os},
    parser {command_parser}
{
}

void Console::commandLoop()
{
    ParserClient client {parser};
    for (is_running = true; is_running; ) {
        outputPrompt();

        std::string input;
        std::getline(is, input);

        try {
            parser.setInput(input);
            initial_number = parser.getNumber();
            client.setHasInitialNumber(initial_number ? true : false);
            if (initial_number && parser.isAtEnd()) {
                insert();
                continue;
            }
            auto id = parser.getCommand(client);
            if (id) {
                if (id_to_argument[*id] == Argument::None && !parser.getRemainder().empty()) {
                    throw Error {"command does not allow arguments", parser.getColumn()};
                }
                ConsoleFunction function = id_to_function[*id];
                    (this->*function)();
            } else {
                auto statement = parser.getRemainder();
                if (statement.length() == 1) {
                    throw Error {"unknown command '"s + statement.front() + "'",
                        parser.getColumn()};
                }
                insertStatement(statement);
            }
        }
        catch (const Error &error) {
            outputError(error);
        }
    }
    os << "Good-bye." << std::endl;
}

void Console::outputPrompt()
{
    if (issue_ready_prompt) {
        os << "Ready" << std::endl;
        issue_ready_prompt = false;
    }
    os << ":" << std::flush;
    start_column = 1;
}

std::optional<CommandId> ParserClient::getCommandId(char c, std::optional<char> next) const
{
    static std::unordered_map<char, CommandId> char_to_id {
        {'i', CommandId::Insert},
        {'l', CommandId::List},
        {'r', CommandId::Run},
        {'q', CommandId::Quit}
    };
    (void)next;
    if (auto id = find(char_to_id, c)) {
        if (has_initial_number && id_to_argument[*id] == Argument::None) {
            throw Error {"command does not allow a line number", parser.getColumn()};
        }
        return id;
    } else {
        return {};
    }
}

void Console::insert()
{
    if (initial_number) {
        validateInitialLineNumber();
    } else if (!parser.isAtEnd()) {
        insert_line_number = getValidLineNumber();
    }
    insertMode({});
}

void Console::insertStatement(std::string_view statement)
{
    validateInitialLineNumber();
    insertMode(statement);
}

void Console::validateInitialLineNumber()
{
    validateLineNumber(*initial_number);
    insert_line_number = *initial_number - 1;
}

void Console::insertMode(std::string_view statement)
{
    for (;;) {
        std::string line;
        if (statement.empty()) {
            outputInsertLineNumber();

            std::getline(is, line);
            if (line.empty()) {
                break;
            }
        } else {
            line = statement;
            statement = {};
        }
        program.insertLine(insert_line_number, line);
        ++insert_line_number;
    }
}

std::size_t Console::getValidLineNumber() const
{
    ;
    if (auto number = parser.getNumber()) {
        validateLineNumber(*number);
        return *number - 1;
    }
    throwInvalidLineNumberError(parser.getRemainder());
}

void Console::validateLineNumber(std::size_t number) const
{
    if (number < 1 || number > program.getLastLineNumber()) {
        throwInvalidLineNumberError(std::to_string(number));
    }
}

void Console::throwInvalidLineNumberError(std::string_view number) const
{
    std::ostringstream error_message;
    error_message << "invalid line number '" << number << "' (valid line";
    auto last_line_number = program.getLastLineNumber();
    if (last_line_number == 1) {
        error_message << " 1";
    } else {
        error_message << "s 1-" << program.getLastLineNumber();
    }
    error_message << ")!";
    throw Error {error_message.str(), parser.getColumn()};
}

void Console::outputInsertLineNumber()
{
    std::ostringstream oss;
    oss << insert_line_number + 1 << ' ';
    os << oss.str() << std::flush;
    start_column = oss.tellp();
}

void Console::list()
{
    size_t line_number = 0;
    for (;;) {
        auto line = program.recreateLine(line_number);
        if (line.empty()) {
            break;
        }
        os << line_number + 1 << ' ' << line << std::endl;
        ++line_number;
    }
}

void Console::run()
{
    program.run(os);
    issue_ready_prompt = true;
}

void Console::quit()
{
    is_running = false;
}

void Console::outputError(const Error &error) const
{
    os << std::string(start_column + error.column, ' ') << "^-- " << error.what() << std::endl;
}
