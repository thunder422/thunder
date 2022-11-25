/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <map>
#include <string_view>
#include <Parser/Error.h>
#include <Parser/Token.h>
#include <Program/Code.h>
#include <Program/Commands.h>
#include <Program/Functions.h>
#include <Program/Operators.h>
#include <Program/OtherOperator.h>
#include <Program/Precedence.h>
#include <Program/Recreator.h>
#include "Compiler.h"


extern OpCode const_num_opcode;
OpCode null_opcode;


Compiler::Compiler(ProgramCode &code, std::istream &is) :
    code {code},
    parser {is}
{
}

void Compiler::compileLine()
{
    unsigned column = parser.getColumn();
    auto keyword = parser.parseIdentifier();
    if (auto command_opcode = Commands::getOpCode(keyword)) {
        Commands::getCompileFunction(*command_opcode)(*this);
    } else {
        throw ParseError {"expected valid command or variable for assignment", column};
    }
}

bool Compiler::compileExpression()
{
    operator_stack.emplace(null_opcode, Precedence::Bottom);
    sub_expression.emplace(OtherOperator::None);
    if (compileUnaryExpression()) {
        while (compileBinaryOperator()) {
            compileUnaryExpression();
        }
        flushOperatorStack(Precedence::Lowest);
        if (operator_stack.top().precedence == Precedence::OpenParen) {
            throw ParseError {"expected binary operator or closing parenthesis",
                parser.getColumn()};
        }
        return true;
    }
    return false;
}

bool Compiler::compileUnaryExpression()
{
    for (;;) {
        if (compileNumConst()) {
            return true;
        } else if (compileUnaryOperator()) {
            parser.skipWhiteSpace();
        } else if (compileOperand() != Operand::None) {
            continue;
        } else if (operator_stack.top().precedence != Precedence::Bottom) {
            throw ParseError {"expected unary operator or operand", parser.getColumn()};
        } else {
            return false;
        }
    }
}

bool Compiler::compileNumConst()
{
    parser.skipWhiteSpace();
    auto number = parser.parseNumber();
    if (number) {
        code.addOpCode(const_num_opcode);
        auto index = code.addConstNum(*number);
        code.addOperand(index);
        return true;
    }
    return false;
}

bool Compiler::compileUnaryOperator()
{
    parser.skipWhiteSpace();
    auto operator_ = Operators::getUnaryOpcode(parser.peekNextChar());
    if (operator_) {
        parser.getNextChar();
        operator_stack.emplace(*operator_);
        if (operator_->precedence == Precedence::OpenParen) {
            sub_expression.emplace(OtherOperator::CloseParen);
        }
        return true;
    }
    return false;
}

Compiler::Operand Compiler::compileOperand()
{
    if (auto token = parser.parseToken()) {
        auto function_opcode = Functions::getFunctionOpcode(token);
        if (token.hasParen()) {
            operator_stack.emplace(*function_opcode, Precedence::Function);
            sub_expression.emplace(OtherOperator::CloseParen);
        } else {
            operator_stack.emplace(*function_opcode, Precedence::UnaryFunc);
        }
        return Operand::SubExpression;
    }
    return Operand::None;
}

bool Compiler::compileBinaryOperator()
{
    for (;;) {
        parser.skipWhiteSpace();
        auto other_operator = sub_expression.top().other_operator;
        auto operator_ = Operators::getBinaryOpcode(parser.peekNextChar(), other_operator);
        if (operator_) {
            parser.getNextChar();
            flushOperatorStack(operator_->precedence);
            if (operator_->precedence == Precedence::CloseParen) {
                if (operator_stack.top().precedence == Precedence::Function) {
                    addOpCode(operator_stack.top().opcode);
                }
                operator_stack.pop();
                sub_expression.pop();
                continue;
            } else {
                operator_stack.emplace(*operator_);
            }
            return true;
        }
        return false;
    }
}

void Compiler::flushOperatorStack(Precedence higher_or_same)
{
    while (operator_stack.top().precedence >= higher_or_same) {
        code.addOpCode(operator_stack.top().opcode);
        operator_stack.pop();
    }
}

void Compiler::addOpCode(OpCode opcode)
{
    code.addOpCode(opcode);
}

char Compiler::peekNextChar()
{
    return parser.peekNextChar();
}
