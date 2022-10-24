/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <iostream>
#include <unordered_map>
#include <Compiler/Compiler.h>
#include <Parser/Parser.h>
#include "Code.h"
#include "Recreator.h"


class OperandTranslator;
using TranslateFunction = void(*)(OperandTranslator &translator);

struct ProgramOpCode {
    OpCode opcode;
    TranslateFunction translate_function;
};

class ProgramOpCodes {
public:
    ProgramOpCodes(std::initializer_list<ProgramOpCode> initializers);
    static TranslateFunction getInsertFunction(WordType opcode);

private:
    std::unordered_map<WordType, TranslateFunction> translate_functions;
};

ProgramOpCodes::ProgramOpCodes(std::initializer_list<ProgramOpCode> initializers)
{
    for (auto &[opcode, translate_function] : initializers) {
        translate_functions[opcode.getValue()] = translate_function;
    }
}

class OperandTranslator {
public:
    OperandTranslator(ProgramCode &program, ProgramCode &line) :
        program {program}, line {line}, iterator {line.begin()} { }
    void operator()();
    ProgramWord getOperand() {
        return *iterator;
    }
    void writeOperand(WordType operand) {
        *iterator = ProgramWord{operand};
        ++iterator;
    }

    ProgramCode &program;
    ProgramCode &line;

private:
    ProgramIterator iterator;
};

void OperandTranslator::operator()()
{
    while (iterator != line.end()) {
        auto opcode = *iterator;
        ++iterator;
        auto translate_function = ProgramOpCodes::getInsertFunction(opcode);
        translate_function(*this);
    }
}

extern OpCode const_num_opcode;

void translateConstNum(OperandTranslator &translator);

void translateNothing(OperandTranslator &)
{
}

ProgramOpCodes &program_opcodes()
{
    static ProgramOpCodes program_opcodes {
        ProgramOpCode{const_num_opcode, translateConstNum}
    };
    return program_opcodes;
}

TranslateFunction ProgramOpCodes::getInsertFunction(WordType opcode)
{
    auto iterator = program_opcodes().translate_functions.find(opcode);
    if (iterator == program_opcodes().translate_functions.end()) {
        return translateNothing;
    }
    return iterator->second;
}

void translateConstNum(OperandTranslator &translator)
{
    WordType index = translator.getOperand();
    auto number = translator.line.getConstNum(index);
    index = translator.program.addConstNum(number);
    translator.writeOperand(index);
}

void ProgramCode::compileLine(std::istream &is)
{
    Compiler compiler {*this, is};
    compiler.compileLine();
}

std::size_t ProgramCode::insertLine(std::size_t offset, ProgramCode &line)
{
    OperandTranslator{*this, line}();

    words.insert(words.begin() + offset, line.words.begin(), line.words.end());
    return line.words.size();
}

std::string ProgramCode::recreateLine(const ProgramView &line_view)
{
    Recreator recreator {*this};
    return recreator.recreateLine(line_view);
}

void ProgramCode::addOpCode(const OpCode &opcode)
{
    words.emplace_back(opcode);
}

WordType ProgramCode::addConstNum(double number)
{
    WordType index = const_nums.size();
    const_nums.emplace_back(number);
    return index;
}

void ProgramCode::addOperand(WordType operand)
{
    words.emplace_back(operand);
}

WordType ProgramCode::getWord(std::size_t offset) const
{
    return words[offset];
}

double ProgramCode::getConstNum(std::size_t index) const
{
    return const_nums[index];
}
