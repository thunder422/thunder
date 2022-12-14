/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include "Parser.h"
#include "Token.h"


void Parser::skipWhiteSpace()
{
    while (std::isspace(peekNextChar())) {
        getNextChar();
    }
}

Token Parser::parseToken()
{
    auto identifier = parseIdentifier();
    bool has_paren = false;
    if (peekNextChar() == '(') {
        has_paren = true;
        getNextChar();
    }
    return Token {std::move(identifier), has_paren};
}
