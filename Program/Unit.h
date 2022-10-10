/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <string>
#include "Code.h"


class ProgramLine {
public:
    ProgramLine(std::size_t offset) : offset {offset} { }

    std::size_t offset;
};


class ProgramUnit {
public:
    ProgramUnit();
    void insertLine(std::size_t line_number, const std::string &line);
    std::string recreateLine(std::size_t line_number);

private:
    std::size_t getOffset(std::size_t line_number);

    ProgramCode code;
    std::vector<ProgramLine> lines;
};
