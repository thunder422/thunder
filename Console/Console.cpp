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
    for (bool is_running {true}; is_running; ) {
        os << "Ready" << std::endl;
        std::string command;
        os << ":" << std::flush;
        std::getline(is, command);
        if (command == "q") {
            is_running = false;
        }
    }
    os << "Good-bye." << std::endl;
}
