# vim:ts=2:sw=2:et:sts=2:
#
# Copyright 2022 Thunder422.  All rights reserved.
# Distributed under GNU General Public License Version 3
# (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)

set(PROGRAM_HEADERS
  Code.h
  Commands.h
  Find.h
  Functions.h
  Lines.h
  OpCode.h
  OpCodes.h
  Operators.h
  OtherOperator.h
  OtherOperatorFwd.h
  Precedence.h
  PrecendenceFwd.h
  Recreator.h
  Runner.h
  Unit.h
  View.h
)
set(PROGRAM_SOURCES
  Code.cpp
  Commands.cpp
  Functions.cpp
  Lines.cpp
  OpCode.cpp
  OpCodes.cpp
  Operators.cpp
  Recreator.cpp
  Runner.cpp
  Unit.cpp
)
add_library(Program OBJECT ${PROGRAM_HEADERS} ${PROGRAM_SOURCES})

add_executable(Expression_UnitTests
  Expression_UnitTests.cpp
)
target_link_libraries(Expression_UnitTests
  PUBLIC
  $<TARGET_OBJECTS:Compiler>
  $<TARGET_OBJECTS:Parser>
  $<TARGET_OBJECTS:Program>
  PRIVATE Catch2::Catch2WithMain
)
add_covered_test(Expression_UnitTests)

add_executable(Runner_UnitTests
  Runner_UnitTests.cpp
)
target_link_libraries(Runner_UnitTests
  PUBLIC
  $<TARGET_OBJECTS:Compiler>
  $<TARGET_OBJECTS:Parser>
  $<TARGET_OBJECTS:Program>
  PRIVATE Catch2::Catch2WithMain
)
add_covered_test(Runner_UnitTests)
