/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <vector>


class ProgramLines {
public:
    struct View {
        std::size_t offset;
        std::size_t size;
    };

    ProgramLines();
    void insert(std::size_t line_number, std::size_t line_size);
    std::size_t size() const;
    View getView(std::size_t line_number) const;

private:
    std::vector<std::size_t> offset;
};
