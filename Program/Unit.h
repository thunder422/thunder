/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <string>
#include "Code.h"


class ProgramUnit {
public:
    ProgramUnit();
    std::string recreateLine(std::size_t line);

private:
    std::size_t getOffset(std::size_t line);

    ProgramCode code;
};
