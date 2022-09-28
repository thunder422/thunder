/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#pragma once

#include <vector>
#include "WordType.h"


struct AllOpCode;
class Recreator;
using RecreateFunction = void(*)(Recreator &);
class Runner;
using RunFunction = void(*)(Runner &);

class OpCodes {
public:
    OpCodes(std::initializer_list<AllOpCode> initializers);
    static RecreateFunction getRecreateFunction(WordType opcode);
    static RunFunction getRunFunction(WordType opcode);

private:
    std::vector<RecreateFunction> recreate_functions;
    std::vector<RunFunction> run_functions;
};
