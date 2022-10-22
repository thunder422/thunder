/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include "Lines.h"


ProgramLines::ProgramLines()
{
    offset.emplace_back(0);
}

void ProgramLines::insert(std::size_t line_number, std::size_t line_size)
{
    auto index = offset.size();
    offset.emplace_back(offset.back() + line_size);
    ++line_number;
    while (index != line_number) {
        --index;
        offset[index] = offset[index - 1] + line_size;
    }
}

std::size_t ProgramLines::size() const
{
    return offset.size() - 1;
}

ProgramLines::View ProgramLines::getView(std::size_t line_number) const
{
    auto line_offset = offset[line_number];
    auto next_offset = offset[line_number + 1];
    return {line_offset, next_offset - line_offset};
}
