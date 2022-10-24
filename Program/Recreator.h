/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <string>


class ProgramCode;
class ProgramView;

class Recreator {
public:
    Recreator(ProgramCode &code);
    void addCommandKeyword();
    std::size_t getOperand();
    double getConstNum(std::size_t index);
    void pushString(std::string string);
    std::string &&recreateLine(const ProgramView &line_view);

private:
    ProgramCode &code;
    std::size_t offset;
    std::string line;
};
