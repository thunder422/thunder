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

void Console::commandLoop()
{
    for (is_running = true; is_running; ) {
        if (issue_ready_prompt) {
            os << "Ready" << std::endl;
            issue_ready_prompt = false;
        }
        std::string command;
        os << ":" << std::flush;
        std::getline(is, command);
        switch (command.front()) {
        case 'i':
            insert();
            break;
        case 'l':
            list();
            break;
        case 'r':
            run();
            break;
        case 'q':
            quit();
            break;
        }
    }
    os << "Good-bye." << std::endl;
}

void Console::insert()
{
    for (;;) {
        os << insert_line_number + 1 << ' ' << std::flush;
        std::string line;
        std::getline(is, line);
        if (line.empty()) {
            break;
        }
        program.insertLine(insert_line_number, line);
        ++insert_line_number;
    }
}

void Console::list()
{
    size_t line_number = 0;
    for (;;) {
        auto line = program.recreateLine(line_number);
        if (line.empty()) {
            break;
        }
        os << line_number + 1 << ' ' << line << std::endl;
        ++line_number;
    }
}

void Console::run()
{
    program.run(os);
    issue_ready_prompt = true;
}

void Console::quit()
{
    is_running = false;
}
