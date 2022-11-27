/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <cstddef>
#include <memory>
#include <stack>
#include <string_view>


class ProgramCode;
class RandomNumberGenerator;

class Runner {
public:
    Runner(ProgramCode &code, std::ostream &os);
    ~Runner();
    void runProgram();
    std::size_t getOperand();
    double getConstNum(std::size_t index);
    void pushNumber(double number);
    double &topNumber();
    double popNumber();
    double getRandomDraw(double scale);
    void setTopToLastRandomDraw();
    void output(std::string_view string);
    void endProgram();

private:
    ProgramCode &code;
    std::size_t pc;
    std::unique_ptr<RandomNumberGenerator> random_number_generator;
    std::stack<double> stack;
    std::ostream &os;
    bool is_done {false};
};

inline void Runner::pushNumber(double number)
{
    stack.emplace(number);
}

inline double &Runner::topNumber()
{
    return stack.top();
}

inline double Runner::popNumber()
{
    auto number = stack.top();
    stack.pop();
    return number;
}
