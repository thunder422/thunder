/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <iosfwd>
#include "Program/Unit.h"


class Console {
public:
    Console(std::istream &is, std::ostream &os);
    void commandLoop();

private:
    void insert();
    void list();
    void run();
    void quit();

    std::istream &is;
    std::ostream &os;
    ProgramUnit program;

    bool is_running;
    bool issue_ready_prompt {true};
};
