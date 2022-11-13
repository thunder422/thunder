/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <charconv>
#include "Code.h"
#include "Commands.h"
#include "OpCodes.h"
#include "Operators.h"
#include "Recreator.h"
#include "View.h"


void recreateCommand(Recreator &recreator)
{
    recreator.addCommandKeyword();
}

void recreateConstNum(Recreator &recreator)
{
    auto index = recreator.getOperand();
    auto number = recreator.getConstNum(index);

    char string[20];
    auto [end, error_code] = std::to_chars(string, string + sizeof(string), number);
    recreator.pushString(std::string{string, end}, Precedence::Operand);
}

void recreateUnaryOperator(Recreator &recreator)
{
    auto unary_opcode = recreator.getOpcode();
    std::string string {Operators::getUnaryChar(unary_opcode)};
    auto &top_string = recreator.topString();
    if (isdigit(top_string.front())) {
        string.append(1, ' ');
    }
    string.append(top_string);
    recreator.swapTopString(string);
    recreator.setTopPrecedence(Operators::getPrecedence(unary_opcode));
}

void recreateBinaryOperator(Recreator &recreator)
{
    auto rhs = recreator.popStack();
    auto operator_opcode = recreator.getOpcode();
    auto operator_precedence = Operators::getPrecedence(operator_opcode);
    if (recreator.topPrecedence() < operator_precedence) {
        std::string lhs = '(' + recreator.topString() + ')';
        recreator.swapTopString(lhs);
    }
    recreator.topString().append(1, ' ').append(Operators::getBinaryChar(operator_opcode))
        .append(1, ' ');
    if (rhs.precedence <= operator_precedence) {
        rhs.string = '(' + rhs.string + ')';
    }
    recreator.topString().append(rhs.string);
    recreator.setTopPrecedence(operator_precedence);
}

Recreator::Recreator(ProgramCode &code) :
    code {code}
{
}

void Recreator::addCommandKeyword()
{
    std::string string {Commands::getKeyword(code.getWord(offset))};
    if (stack.empty()) {
        pushString(std::move(string), Precedence{});
    } else {
        string.append(1, ' ').append(topString());
        swapTopString(string);
    }
}

std::size_t Recreator::getOpcode()
{
    return opcode;
}

std::size_t Recreator::getOperand()
{
    return code.getWord(++offset);
}

double Recreator::getConstNum(std::size_t index)
{
    return code.getConstNum(index);
}

void Recreator::pushString(std::string string, Precedence precedence)
{
    stack.emplace(std::move(string), precedence);
}

std::string &Recreator::topString()
{
    return stack.top().string;
}

Precedence Recreator::topPrecedence()
{
    return stack.top().precedence;
}

void Recreator::swapTopString(std::string &string)
{
    stack.top().string.swap(string);
}

void Recreator::setTopPrecedence(Precedence precedence)
{
    stack.top().precedence = precedence;
}

Recreator::String Recreator::popStack()
{
    auto string = std::move(stack.top());
    stack.pop();
    return string;
}

std::string Recreator::recreateLine(const ProgramView &line_view)
{
    offset = line_view.offset;
    auto end_offset = offset + line_view.size;
    while (offset < end_offset) {
        opcode = code.getWord(offset);
        OpCodes::getRecreateFunction(opcode)(*this);
        ++offset;
    }
    return popStack().string;
}
