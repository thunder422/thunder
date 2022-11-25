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
#include <Program/OtherOperatorFwd.h>
#include <Program/WordType.h>


class ProgramCode;

class Compiler {
public:
    Compiler(ProgramCode &code, std::istream &is);
    void compileLine();
    bool compileExpression();
    void addOpCode(OpCode opcode);
    char peekNextChar();

private:
    enum class Operand {
        None,
        SubExpression
    };

    bool compileUnaryExpression();
    bool compileNumConst();
    bool compileUnaryOperator();
    Operand compileOperand();
    bool compileBinaryOperator();
    void flushOperatorStack(Precedence higher_or_same);

    class SubExpression {
    public:
        SubExpression(OtherOperator other_operator) : other_operator {other_operator} { }

        OtherOperator other_operator;
    };

    ProgramCode &code;
    Parser parser;
    std::stack<Operator> operator_stack;
    std::stack<SubExpression> sub_expression;
};
