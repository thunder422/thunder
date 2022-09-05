/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <iostream>
#include <optional>


class Parser {
public:
    Parser(std::istream &is);

    std::string parseIdentifier();
    std::optional<double> parseNumber();

    std::streampos getColumn();
    char peekNextChar();
    char getNextChar();
    void ungetChar();

private:
    std::istream &is;
};


inline Parser::Parser(std::istream &is) :
    is {is}
{
}

inline std::streampos Parser::getColumn()
{
    return is.tellg();
}

inline char Parser::peekNextChar()
{
    return is.peek();
}

inline char Parser::getNextChar()
{
    return is.get();
}

inline void Parser::ungetChar()
{
    is.unget();
}
