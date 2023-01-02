/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <optional>
#include <string_view>
#include <variant>


enum class CommandId : int;

class Client {
public:
    virtual std::optional<CommandId> getCommandId(char c, std::optional<char> next) const = 0;
};

class CommandParser {
public:
    void setInput(std::string_view input);
    std::optional<std::size_t> getNumber();
    std::optional<CommandId> getCommand(const Client &client);
    std::string_view getRemainder();
    bool isAtEnd();
    size_t getColumn() const;

private:
    void setColumn();
    void setLength();
    std::size_t calculateColumn() const;
    void skipSpaces();

    std::string_view command;
    const char *pointer;
    std::size_t column;
    std::size_t length;
};
