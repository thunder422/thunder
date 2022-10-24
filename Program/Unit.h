/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <string>
#include "Code.h"
#include "Lines.h"


class ProgramUnit {
public:
    ProgramUnit();
    size_t getLastLineNumber() const;
    void insertLine(std::size_t line_number, const std::string &line);
    std::string recreateLine(std::size_t line_number);
    void run(std::ostream &os);

private:
    ProgramView getLineView(std::size_t line_number);

    ProgramCode code;
    ProgramLines lines;
};
