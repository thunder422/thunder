/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <iostream>
#include <optional>


class Token;


class Parser {
public:
    Parser(std::istream &is);

    void skipWhiteSpace();
    Token parseToken();
    std::string parseIdentifier();
    std::optional<double> parseNumber();

    std::size_t getColumn();
    char peekNextChar();
    char getNextChar();
    void ungetChar();

private:
    std::istream &is;
    std::size_t column {0};
    bool is_at_end {false};
};


inline Parser::Parser(std::istream &is) :
    is {is}
{
}

inline std::size_t Parser::getColumn()
{
    return column;
}

inline char Parser::peekNextChar()
{
    char next_char = is.peek();
    if (std::char_traits<char>::not_eof(next_char)) {
        column = is.tellg();
    } else if (!is_at_end) {
        ++column;
        is_at_end = true;
    }
    return next_char;
}

inline char Parser::getNextChar()
{
    return is.get();
}

inline void Parser::ungetChar()
{
    is.unget();
}
