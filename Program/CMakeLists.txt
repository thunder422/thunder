# vim:ts=2:sw=2:et:sts=2:
#
# Copyright 2022 Thunder422.  All rights reserved.
# Distributed under GNU General Public License Version 3
# (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)

set(PROGRAM_HEADERS
  Code.h
  Commands.h
  OpCode.h
  OpCodes.h
  Recreator.h
  Runner.h
  Unit.h
)
set(PROGRAM_SOURCES
  Code.cpp
  Commands.cpp
  OpCode.cpp
  OpCodes.cpp
  Recreator.cpp
  Runner.cpp
  Unit.cpp
)
add_library(Program OBJECT ${PROGRAM_HEADERS} ${PROGRAM_SOURCES})

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
