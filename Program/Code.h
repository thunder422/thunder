/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <iosfwd>
#include <vector>

#include "Word.h"


using ProgramWords = std::vector<ProgramWord>;


class ProgramCode {
public:
    ProgramCode() = default;
    ProgramCode(std::istream &is);

    std::string recreateLine(std::size_t line_offset);

    void addOpCode(const OpCode &opcode);
    WordType addConstNum(double number);
    void addOperand(WordType operand);
    WordType getWord(std::size_t offset) const;
    double getConstNum(std::size_t index) const;

private:
    ProgramWords words;
    std::vector<double> const_nums;
};
