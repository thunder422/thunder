/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include <cctype>
#include <iostream>

#include "Identifier.h"

class IdentifierParser {
public:
    IdentifierParser(std::istream &is);

    std::string operator()();

private:
    std::istream &is;
    std::string identifier;
};

IdentifierParser::IdentifierParser(std::istream &is) :
    is {is}
{
}

std::string IdentifierParser::operator()()
{
    for (;;) {
        auto next_char = is.peek();
        if (std::isalpha(next_char)) {
            identifier += is.get();
        } else {
            return std::move(identifier);
        }
    }
}

std::string parseIdentifier(std::istream &is)
{
    return IdentifierParser{is}();
}
