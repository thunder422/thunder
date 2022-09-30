/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <iostream>
#include "Console.h"


Console::Console(std::istream &is, std::ostream &os) :
    is {is},
    os {os}
{
}

void Console::run()
{
    bool issue_ready_prompt = true;
    for (bool is_running {true}; is_running; ) {
        if (issue_ready_prompt) {
            os << "Ready" << std::endl;
            issue_ready_prompt = false;
        }
        std::string command;
        os << ":" << std::flush;
        std::getline(is, command);
        if (command == "l") {
            auto line = program.recreateLine(0);
            os << line << std::endl;
        }
        if (command == "q") {
            is_running = false;
        }
    }
    os << "Good-bye." << std::endl;
}
