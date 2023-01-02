// vim:ts=4:sw=4:et:sts=4:
//
// Copyright 2022 Thunder422.  All rights reserved.
// Distributed under GNU General Public License Version 3
// (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)

#include <cctype>
#include <iostream>
#include <string>

#include "Error.h"
#include "Parser.h"


class NumberParser {
public:
    NumberParser(Parser &parser);

    std::optional<double> operator()();

    void parseStart(int next_char);
    void parseNegative(int next_char);
    void parseZero(int next_char);
    void parsePeriod(int next_char);
    void parseMantissa(int next_char);
    void parseExponent(int next_char);
    void parseExponentSign(int next_char);
    void parseExponentDigits(int next_char);

    bool isValidMantissaChar(int next_char);
    void addNextChar();
    void ungetChar();

private:
    Parser &parser;
    void (NumberParser::*parse)(int next_char);
    bool is_floating_point;
    bool is_done;
    std::string number;
};

NumberParser::NumberParser(Parser &parser) :
    parser {parser},
    parse {&NumberParser::parseStart},
    is_floating_point {false},
    is_done {false}
{
}

std::optional<double> NumberParser::operator()()
{
    do {
        auto next_char = parser.peekNextChar();
        (this->*parse)(next_char);
    } while (!is_done);

    if (number.empty()) {
        return {};
    }
    return std::stod(number);
}

void NumberParser::parseStart(int next_char)
{
    if (next_char == '0') {
        parse = &NumberParser::parseZero;
    } else if (next_char == '-') {
        parse = &NumberParser::parseNegative;
    } else if (!isValidMantissaChar(next_char)) {
        is_done = true;
        return;
    }
    addNextChar();
}

void NumberParser::parseNegative(int next_char)
{
    if (isValidMantissaChar(next_char)) {
        addNextChar();
    } else {
        ungetChar();
    }
}

void NumberParser::parseZero(int next_char)
{
    if (next_char == '.') {
        parse = &NumberParser::parseMantissa;
    } else if (isdigit(next_char)) {
        throw Error {"expected decimal point after leading zero", parser.getColumn()};
    } else {
        is_done = true;
    }
}

void NumberParser::parsePeriod(int next_char)
{
    if (isdigit(next_char)) {
        parse = &NumberParser::parseMantissa;
    } else {
        throw Error {"expected digit after decimal point", parser.getColumn()};
    }
}

void NumberParser::parseMantissa(int next_char)
{
    if (next_char == '.' && !is_floating_point) {
        is_floating_point = true;
    } else if (toupper(next_char) == 'E') {
        parse = &NumberParser::parseExponent;
    } else if (!isdigit(next_char)) {
        is_done = true;;
        return;
    }
    addNextChar();
}

void NumberParser::parseExponent(int next_char)
{
    if (next_char == '-' || next_char == '+') {
        parse = &NumberParser::parseExponentSign;
    } else if (isdigit(next_char)) {
        parse = &NumberParser::parseExponentDigits;
    } else {
        throw Error {"expected sign or digit for exponent", parser.getColumn()};
    }
    is_floating_point = true;
    addNextChar();
}

void NumberParser::parseExponentSign(int next_char)
{
    if (isdigit(next_char)) {
        parse = &NumberParser::parseExponentDigits;
    } else {
        throw Error {"expected digit after exponent sign", parser.getColumn()};
    }
}

void NumberParser::parseExponentDigits(int next_char)
{
    if (isdigit(next_char)) {
        addNextChar();
    } else {
        is_done = true;
    }
}

bool NumberParser::isValidMantissaChar(int next_char)
{
    if (next_char == '.') {
        is_floating_point = true;
        parse = &NumberParser::parsePeriod;
    } else if (isdigit(next_char)) {
        parse = &NumberParser::parseMantissa;
    } else {
        return false;
    }
    return true;
}

void NumberParser::addNextChar()
{
    number += parser.getNextChar();
}

void NumberParser::ungetChar()
{
    parser.ungetChar();
    number.clear();
    is_done = true;
}


std::optional<double> Parser::parseNumber()
{
    return NumberParser{*this}();
}
