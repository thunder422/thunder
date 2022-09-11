# vim:ts=2:sw=2:et:sts=2:
#
# Copyright 2022 Thunder422.  All rights reserved.
# Distributed under GNU General Public License Version 3
# (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)

set(PROGRAM_HEADERS
  CommandOpCode.h
  OpCode.h
  Recreator.h
)
set(PROGRAM_SOURCES
  CommandOpCode.cpp
  OpCode.cpp
  Recreator.cpp
)
add_library(Program OBJECT ${PROGRAM_HEADERS} ${PROGRAM_SOURCES})

add_executable(ProgramOpCode_UnitTests
  OpCode.h
  OpCode.cpp
  OpCode_UnitTests.cpp
  ${CATCH_SOURCES}
)
add_covered_test(ProgramOpCode_UnitTests OpCode*)

add_executable(ProgramCommandOpCode_UnitTests
  CommandOpCode.h
  CommandOpCode.cpp
  CommandOpCode_UnitTests.cpp
  OpCode.cpp
  ${CATCH_SOURCES}
)
add_covered_test(ProgramCommandOpCode_UnitTests CommandOpCode*)

add_executable(ProgramWord_UnitTests
  CommandOpCode.cpp
  OpCode.cpp
  Word.h
  Word_UnitTests.cpp
  ${CATCH_SOURCES}
)
add_covered_test(ProgramWord_UnitTests Word*)

add_executable(ProgramCode_UnitTests
  Code.h
  Code.cpp
  Code_UnitTests.cpp
  ${CATCH_SOURCES}
)
target_link_libraries(ProgramCode_UnitTests
  $<TARGET_OBJECTS:Parser>
  $<TARGET_OBJECTS:Program>
)
add_covered_test(ProgramCode_UnitTests Code*)

add_executable(Recreator_UnitTests
  Recreator_UnitTests.cpp
  ${CATCH_SOURCES}
)
target_link_libraries(Recreator_UnitTests
  $<TARGET_OBJECTS:Program>
)
add_covered_test(Recreator_UnitTests Recreator*)
