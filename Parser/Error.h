/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <stdexcept>


struct ParseError : public std::runtime_error {
    ParseError(const char *message, long column) : runtime_error {message}, column {column} { }

    long column;
};
