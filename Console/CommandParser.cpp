/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <charconv>
#include "CommandParser.h"


void CommandParser::setInput(std::string_view input)
{
    command = input;
    pointer = command.data();
}

std::optional<std::size_t> CommandParser::getNumber()
{
    skipSpaces();
    setColumn();
    std::size_t number;
    auto [end_number, error_code] = std::from_chars(pointer, command.end(), number);
    if (error_code != std::errc{}) {
        return {};
    }
    pointer = end_number;
    setLength();
    return number;
}

std::optional<CommandId> CommandParser::getCommand(const Client &client)
{
    if (isAtEnd()) {
        return {};
    }
    auto current_pointer = pointer;
    ++pointer;
    std::optional<char> next_char;
    if (!isAtEnd()) {
        next_char = *pointer;
    }
    auto id = client.getCommandId(*current_pointer, next_char);
    if (!id) {
        pointer = current_pointer;
    }
    return id;
}

std::string_view CommandParser::getRemainder()
{
    setColumn();
    return command.substr(column);
}

void CommandParser::setColumn()
{
    column = calculateColumn();
}

void CommandParser::setLength()
{
    length = calculateColumn() - column;
}

std::size_t CommandParser::calculateColumn() const
{
    return pointer - command.data();
}

bool CommandParser::isAtEnd()
{
    skipSpaces();
    return pointer == command.end();
}

size_t CommandParser::getColumn() const
{
    return column;
}

void CommandParser::skipSpaces()
{
    while (isspace(*pointer)) {
        ++pointer;
    }
}
