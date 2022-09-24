/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <string>


class CommandOpCode;
class ProgramCode;

class Recreator {
public:
    Recreator(ProgramCode &code);
    void addCommandKeyword();
    std::size_t getOperand();
    double getConstNum(std::size_t index);
    void pushString(std::string string);
    std::string &&recreateLine(std::size_t line_offset);

private:
    ProgramCode &code;
    std::size_t offset;
    std::string line;
    bool is_done {false};
};
