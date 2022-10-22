# Thunder

Thunder Interactive Compiler

## Purpose

To create a simple programming environment for learning programming. This was the intention of the Interactive BASIC Compiler Project (IBCP), but that project became complicated trying to implement the GUI using the Qt framework.  The Incremental BASIC Compiler Experiment (IBCX) was intended another time to just implement a library with all of the interactive compiler code that could  be used to implement some type of user interface using it.

This project goes in a different direction and instead of implementing yet another variation of BASIC, a brand new language will be created named Thunder, which will start taking inspiration from the original BASIC, Dartmouth BASIC, but with one very important difference - there will be no line numbers or GOTO statements in the Thunder programming language.

The initial Thunder programming environment will be a simple terminal application just like the original BASICs with no graphical user interface (GUIs are complicated to implement as seen with the IBCP).  This project utilizes C++17 and the C++ Standard Library (STL).

## Testing

The [Catch2](https://github.com/catchorg/Catch2) unit test platform is being used for developing tests. Catch is easy to use. The programs will be created under CMake and run with CTest. Coverage tools are being utilized to insure that 100% of the code is being tested. The unit tests will also serve as documentation on user of the code.

## Notes

Details of changes are contained in commit messages of the repository along with a [wiki](https://github.com/thunder422/thunder/wiki) containing documentation and details about tool setup, building, testing, language specification, etc.

## Status

The initial goal was to stand up a very rudimentary programming environment with the bare minimum programming language.  This includes the ability to enter, list and run program statements.  Implemented statements include a simple PRINT (single constant) and END statements.
