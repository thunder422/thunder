# Thunder

Thunder Interactive Compiler

## Purpose

To create a simple programming environment for learning programming. This was the intention of the Interactive BASIC Compiler Project, but that code had became rather complicated.  The Incremental BASIC Compiler Experiment was intended as a way to start over and experiment with some concepts.  This Thunder project is again starting over but this time instead of implementing yet another BASIC (among dozens of varieties), a brand new language will be created named Thunder, which will start simply and will take inspiration from the original BASIC, Dartmouth BASIC.

The Thunder programming environment will be pretty much a terminal application just like the original BASICs with no graphical user interface (GUIs are complicated to implement as seen with the IBCP).

The Test Driven Development (TDD) methodology will be used, which consists of these three simple steps:

1. Develop a test for a new feature (which will initially fail as the feature is not yet implement).
2. Implement just enough code to make the test pass.
3. Clean up (refactor) the code (using the passing tests to validate the changes).

These steps are repeated as development progresses.  This project will utilize C++17 and the C++ Standard Library (STL).

## Testing

The [Catch](https://github.com/philsquared/Catch) unit test platform will be used for developing the tests.  Catch is easy to use and only consists of a single header file.  The programs will be created under CMake and run with CTest.

## Notes

There is no blog associated with this project.  Details of changes will be contained in commit messages of the repository along with the [wiki](https://github.com/thunder422/thunder/wiki) containing documentation and details about tool setup, building, testing, etc.

## Status

This section will contain the current status of the project.  The current goal is to stand up a very rudimentary programming environment with the bare minimum programming language.  This means the ability to enter and run program statements, with the only a simple PRINT and END statements.
