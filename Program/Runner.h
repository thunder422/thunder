/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <cstddef>
#include <string_view>


class ProgramCode;

class Runner {
public:
    Runner(ProgramCode &code, std::ostream &os);
    void runProgram();
    std::size_t getOperand();
    double getConstNum(std::size_t index);
    void pushNumber(double number);
    double popNumber();
    void output(std::string_view string);
    void endProgram();

private:
    ProgramCode &code;
    std::size_t pc;
    double stack;
    std::ostream &os;
    bool is_done {false};
};

