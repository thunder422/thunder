/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <iostream>
#include <sstream>
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
        std::string input;
        os << ":" << std::flush;
        std::getline(is, input);
        std::istringstream iss {input};
        char command;
        iss >> command;
        switch (command) {
        case 'i':
            insert(iss);
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

void Console::insert(std::istream &command_is)
{
    if (command_is.peek() != std::char_traits<char>::eof()) {
        std::size_t number;
        command_is >> number;
        insert_line_number = number - 1;
    }
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
