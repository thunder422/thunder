/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <cctype>
#include <iostream>

#include "Parser.h"


class IdentifierParser {
public:
    IdentifierParser(Parser &parser);

    std::string operator()();

private:
    Parser &parser;
    std::string identifier;
};

IdentifierParser::IdentifierParser(Parser &parser) :
    parser {parser}
{
}

std::string IdentifierParser::operator()()
{
    for (;;) {
        auto next_char = parser.peekNextChar();
        if (std::isalpha(next_char)) {
            identifier += parser.getNextChar();
        } else {
            return std::move(identifier);
        }
    }
}


std::string Parser::parseIdentifier()
{
    return IdentifierParser{*this}();
}
