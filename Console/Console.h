/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <iosfwd>
#include <optional>
#include "Program/Unit.h"


class CommandParser;
class Error;

class Console {
public:
    Console(std::istream &is, std::ostream &os);
    void commandLoop();

    void insert();
    void list();
    void run();
    void quit();

private:
    void outputPrompt();
    void insertStatement(std::string_view statement);
    void validateInitialLineNumber();
    void insertMode(std::string_view statement);
    std::size_t getValidLineNumber() const;
    void validateLineNumber(std::size_t number) const;
    [[noreturn]] void throwInvalidLineNumberError(std::string_view number) const;
    void outputInsertLineNumber();
    void outputError(const Error &error) const;

    std::istream &is;
    std::ostream &os;
    CommandParser &parser;
    ProgramUnit program;

    std::size_t start_column;
    bool is_running;
    bool issue_ready_prompt {true};
    std::optional<std::size_t> initial_number;
    std::size_t insert_line_number {0};
};
