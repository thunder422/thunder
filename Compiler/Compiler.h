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
#include <Program/OpCode.h>
#include <Program/WordType.h>


enum class Precedence : int;
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
    bool compileBinaryOperator();
    void flushOperatorStack(Precedence higher_or_same);

    class Operator {
    public:
        Operator(OpCode opcode, Precedence precedence) :
            opcode {opcode}, precedence {precedence} { }

        OpCode opcode;
        Precedence precedence;
    };

    ProgramCode &code;
    Parser parser;
    std::stack<Operator> operator_stack;
};
