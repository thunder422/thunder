/* vim:ts=4:sw=4:et:sts=4:
 *
 * Copyright 2022 Thunder422.  All rights reserved.
 * Distributed under GNU General Public License Version 3
 * (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
 */

#include "OpCode.h"
#include "OpCodes.h"


struct AllOpCode {
    OpCode opcode;
    RecreateFunction recreate_function;
    RunFunction run_function;
};

OpCodes::OpCodes(std::initializer_list<AllOpCode> initializers) :
    recreate_functions(OpCode::getCount()),
    run_functions(OpCode::getCount())
{
    for (auto &[opcode, recreate_function, run_function] : initializers) {
        recreate_functions[opcode.getValue()] = recreate_function;
        run_functions[opcode.getValue()] = run_function;
    }
}

OpCode const_num_opcode;
extern OpCode neg_opcode;
extern OpCode add_opcode;
extern OpCode sub_opcode;
extern OpCode mul_opcode;
extern OpCode div_opcode;
extern OpCode mod_opcode;
extern OpCode idiv_opcode;
extern OpCode pow_opcode;
extern OpCode abs_opcode;
extern OpCode int_opcode;
extern OpCode rnd_opcode;
extern OpCode rnd0_opcode;
extern OpCode sgn_opcode;
extern OpCode sqr_opcode;
extern OpCode log_opcode;
extern OpCode exp_opcode;
extern OpCode cos_opcode;
extern OpCode sin_opcode;
extern OpCode tan_opcode;
extern OpCode atn_opcode;
extern OpCode print_opcode;
extern OpCode print_num_opcode;
extern OpCode end_opcode;

void runPrint(Runner &runner);
void runPrintNum(Runner &runner);
void runEnd(Runner &runner);
void runConstNum(Runner &runner);
void runNeg(Runner &runner);
void runAdd(Runner &runner);
void runSub(Runner &runner);
void runMul(Runner &runner);
void runDiv(Runner &runner);
void runMod(Runner &runner);
void runIdiv(Runner &runner);
void runPow(Runner &runner);
void runAbs(Runner &runner);
void runInt(Runner &runner);
void runRnd(Runner &runner);
void runRnd0(Runner &runner);
void runSgn(Runner &runner);
void runSqr(Runner &runner);
void runLog(Runner &runner);
void runExp(Runner &runner);
void runCos(Runner &runner);
void runSin(Runner &runner);
void runTan(Runner &runner);
void runAtn(Runner &runner);

void recreateCommand(Recreator &recreator);
void recreateConstNum(Recreator &recreator);
void recreateUnaryOperator(Recreator &recreator);
void recreateBinaryOperator(Recreator &recreator);
void recreateFunction(Recreator &recreator);

void recreateNothing(Recreator &)
{
}

OpCodes &opcodes()
{
    static OpCodes opcodes {
        {print_opcode, recreateCommand, runPrint},
        {print_num_opcode, recreateNothing, runPrintNum},
        {end_opcode, recreateCommand, runEnd},
        {const_num_opcode, recreateConstNum, runConstNum},
        {neg_opcode, recreateUnaryOperator, runNeg},
        {add_opcode, recreateBinaryOperator, runAdd},
        {sub_opcode, recreateBinaryOperator, runSub},
        {mul_opcode, recreateBinaryOperator, runMul},
        {div_opcode, recreateBinaryOperator, runDiv},
        {mod_opcode, recreateBinaryOperator, runMod},
        {idiv_opcode, recreateBinaryOperator, runIdiv},
        {pow_opcode, recreateBinaryOperator, runPow},
        {abs_opcode, recreateFunction, runAbs},
        {int_opcode, recreateFunction, runInt},
        {rnd_opcode, recreateFunction, runRnd},
        {rnd0_opcode, recreateFunction, runRnd0},
        {sgn_opcode, recreateFunction, runSgn},
        {sqr_opcode, recreateFunction, runSqr},
        {log_opcode, recreateFunction, runLog},
        {exp_opcode, recreateFunction, runExp},
        {cos_opcode, recreateFunction, runCos},
        {sin_opcode, recreateFunction, runSin},
        {tan_opcode, recreateFunction, runTan},
        {atn_opcode, recreateFunction, runAtn}
    };
    return opcodes;
}

RecreateFunction OpCodes::getRecreateFunction(WordType opcode)
{
    return opcodes().recreate_functions[opcode];
}

RunFunction OpCodes::getRunFunction(WordType opcode)
{
    return opcodes().run_functions[opcode];
}
