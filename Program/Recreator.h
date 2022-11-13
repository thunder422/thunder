/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <stack>
#include <string>
#include "PrecendenceFwd.h"
#include "WordType.h"


class ProgramCode;
class ProgramView;

class Recreator {
public:
    class String {
    public:
        String(std::string string, Precedence precedence) :
            string {string}, precedence {precedence} { }

        std::string string;
        Precedence precedence;
    };

    Recreator(ProgramCode &code);
    void addCommandKeyword();
    std::size_t getOpcode();
    std::size_t getOperand();
    double getConstNum(std::size_t index);
    void pushString(std::string string, Precedence precedence);
    std::string &topString();
    Precedence topPrecedence();
    void swapTopString(std::string &string);
    void setTopPrecedence(Precedence precedence);
    String popStack();
    std::string recreateLine(const ProgramView &line_view);

private:
    ProgramCode &code;
    std::size_t offset;
    WordType opcode;
    std::stack<String> stack;
};
