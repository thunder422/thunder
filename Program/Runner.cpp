/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <charconv>
#include <cmath>
#include <iostream>
#include <random>
#include "Code.h"
#include "OpCodes.h"
#include "Runner.h"


class RandomNumberGenerator {
public:
    RandomNumberGenerator() : uniform_distribution {0.0, 1.0} { }
    double draw(double scale);
    double getLastDraw() const;

private:
    std::mt19937_64 random_generator;
    std::uniform_real_distribution<double> uniform_distribution;
    double last_draw {0};
};

double RandomNumberGenerator::draw(double scale)
{
    last_draw = uniform_distribution(random_generator) * scale;
    return last_draw;
}

double RandomNumberGenerator::getLastDraw() const
{
    return last_draw;
}


void runPrint(Runner &runner)
{
    runner.output("\n");
}

void runPrintNum(Runner &runner)
{
    char string[24];
    auto number = runner.popNumber();
    auto [end, error_code] = std::to_chars(string, string + sizeof(string), number);
    runner.output(std::string{string, end});
}

void runEnd(Runner &runner)
{
    runner.output("End Program.\n");
    runner.endProgram();
}

void runConstNum(Runner &runner)
{
    auto index = runner.getOperand();
    auto number = runner.getConstNum(index);
    runner.pushNumber(number);
}

void runNeg(Runner &runner)
{
    runner.topNumber() = -runner.topNumber();
}

void runAdd(Runner &runner)
{
    auto rhs = runner.popNumber();
    runner.topNumber() += rhs;
}

void runSub(Runner &runner)
{
    auto rhs = runner.popNumber();
    runner.topNumber() -= rhs;
}

void runMul(Runner &runner)
{
    auto rhs = runner.popNumber();
    runner.topNumber() *= rhs;
}

void runDiv(Runner &runner)
{
    auto rhs = runner.popNumber();
    runner.topNumber() /= rhs;
}

void runMod(Runner &runner)
{
    auto rhs = runner.popNumber();
    runner.topNumber() = std::fmod(runner.topNumber(), rhs);
}

void runIdiv(Runner &runner)
{
    auto rhs = runner.popNumber();
    runner.topNumber() = std::trunc(runner.topNumber() / rhs);
}

void runPow(Runner &runner)
{
    auto rhs = runner.popNumber();
    runner.topNumber() = std::pow(runner.topNumber(), rhs);
}

void runAbs(Runner &runner)
{
    runner.topNumber() = std::abs(runner.topNumber());
}

void runInt(Runner &runner)
{
    runner.topNumber() = std::floor(runner.topNumber());
}

void runRnd(Runner &runner)
{
    if (runner.topNumber() == 0) {
        runner.setTopToLastRandomDraw();
    } else {
        runner.topNumber() = runner.getRandomDraw(runner.topNumber());
    }
}

void runRnd0(Runner &runner)
{
    runner.pushNumber(runner.getRandomDraw(1));
}

void runSgn(Runner &runner)
{
    auto argument = runner.topNumber();
    if (argument > 0) {
        runner.topNumber() = 1;
    } else if (argument < 0) {
        runner.topNumber() = -1;
    } else {
        runner.topNumber() = 0;
    }
}

void runSqr(Runner &runner)
{
    runner.topNumber() = std::sqrt(runner.topNumber());
}

void runLog(Runner &runner)
{
    runner.topNumber() = std::log(runner.topNumber());
}

void runExp(Runner &runner)
{
    runner.topNumber() = std::exp(runner.topNumber());
}

void runCos(Runner &runner)
{
    runner.topNumber() = std::cos(runner.topNumber());
}

void runSin(Runner &runner)
{
    runner.topNumber() = std::sin(runner.topNumber());
}

void runTan(Runner &runner)
{
    runner.topNumber() = std::tan(runner.topNumber());
}

void runAtn(Runner &runner)
{
    runner.topNumber() = std::atan(runner.topNumber());
}

Runner::Runner(ProgramCode &code, std::ostream &os) :
    code {code},
    os {os}
{
}

Runner::~Runner() = default;

void Runner::runProgram()
{
    pc = 0;
    random_number_generator = std::make_unique<RandomNumberGenerator>();
    while (!is_done) {
        auto opcode = code.getWord(pc);
        OpCodes::getRunFunction(opcode)(*this);
        ++pc;
    }
}

std::size_t Runner::getOperand()
{
    return code.getWord(++pc);
}

double Runner::getConstNum(std::size_t index)
{
    return code.getConstNum(index);
}

double Runner::getRandomDraw(double scale)
{
    return random_number_generator->draw(scale);
}

void Runner::setTopToLastRandomDraw()
{
    topNumber() = random_number_generator->getLastDraw();
}

void Runner::output(std::string_view string)
{
    os << string;
}

void Runner::endProgram()
{
   is_done = true;
}
