/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <string>


class Token {
public:
    Token(std::string &&value, bool has_paren);
    operator bool() const;
    const std::string &getValue() const;
    bool hasParen() const;

private:
    std::string value;
    bool has_paren;
};

inline Token::Token(std::string &&value, bool has_paren) :
    value {std::move(value)},
    has_paren {has_paren}
{
}

inline Token::operator bool() const
{
    return !value.empty();
}

inline const std::string &Token::getValue() const
{
    return value;
}

inline bool Token::hasParen() const
{
    return has_paren;
}
