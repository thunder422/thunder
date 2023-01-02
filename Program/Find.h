/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <optional>


template <typename Container, typename KeyType>
std::optional<typename Container::mapped_type> find(const Container &container, const KeyType &key)
{
    if (auto iterator = container.find(key); iterator != container.end()) {
        return iterator->second;
    }
    return {};
}
