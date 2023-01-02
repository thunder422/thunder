/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <stdexcept>


struct Error : public std::runtime_error {
    Error(std::string message, std::size_t column) : runtime_error {message}, column {column} { }

    std::size_t column;
};
