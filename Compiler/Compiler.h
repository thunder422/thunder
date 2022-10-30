/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <iosfwd>
#include <stack>
#include <Parser/Parser.h>
#include <Program/WordType.h>


class OpCode;
class ProgramCode;

class Compiler {
public:
    Compiler(ProgramCode &code, std::istream &is);
    void compileLine();
    bool compileExpression();
    void addOpCode(OpCode opcode);

private:
    bool compileUnaryExpression();
    bool compileNumConst();
    bool compileUnaryOperator();
    void flushOpcodeStack();

    ProgramCode &code;
    Parser parser;
    std::stack<OpCode> opcode_stack;
};
