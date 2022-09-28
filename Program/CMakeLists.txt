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
)
set(PROGRAM_SOURCES
  Code.cpp
  Commands.cpp
  OpCode.cpp
  OpCodes.cpp
  Recreator.cpp
  Runner.cpp
)
add_library(Program OBJECT ${PROGRAM_HEADERS} ${PROGRAM_SOURCES})

add_executable(ProgramOpCode_UnitTests
  OpCode.h
  OpCode.cpp
  OpCode_UnitTests.cpp
  ${CATCH_SOURCES}
)
add_covered_test(ProgramOpCode_UnitTests OpCode*)

add_executable(ProgramWord_UnitTests
  OpCode.cpp
  Word.h
  Word_UnitTests.cpp
  ${CATCH_SOURCES}
)
add_covered_test(ProgramWord_UnitTests Word*)

add_executable(ProgramCode_UnitTests
  Code_UnitTests.cpp
  ${CATCH_SOURCES}
)
target_link_libraries(ProgramCode_UnitTests
  $<TARGET_OBJECTS:Compiler>
  $<TARGET_OBJECTS:Parser>
  $<TARGET_OBJECTS:Program>
)
add_covered_test(ProgramCode_UnitTests Code*)

add_executable(OpCodes_UnitTests
  OpCodes_UnitTests.cpp
  ${CATCH_SOURCES}
)
target_link_libraries(OpCodes_UnitTests
  $<TARGET_OBJECTS:Compiler>
  $<TARGET_OBJECTS:Parser>
  $<TARGET_OBJECTS:Program>
)
add_covered_test(OpCodes_UnitTests OpCodes*)

add_executable(Commands_UnitTests
  Commands_UnitTests.cpp
  ${CATCH_SOURCES}
)
target_link_libraries(Commands_UnitTests
  $<TARGET_OBJECTS:Compiler>
  $<TARGET_OBJECTS:Parser>
  $<TARGET_OBJECTS:Program>
)
add_covered_test(Commands_UnitTests Commands*)

add_executable(Recreator_UnitTests
  Recreator_UnitTests.cpp
  ${CATCH_SOURCES}
)
target_link_libraries(Recreator_UnitTests
  $<TARGET_OBJECTS:Compiler>
  $<TARGET_OBJECTS:Parser>
  $<TARGET_OBJECTS:Program>
)
add_covered_test(Recreator_UnitTests Recreator*)

add_executable(Runner_UnitTests
  Runner_UnitTests.cpp
  ${CATCH_SOURCES}
)
target_link_libraries(Runner_UnitTests
  $<TARGET_OBJECTS:Compiler>
  $<TARGET_OBJECTS:Parser>
  $<TARGET_OBJECTS:Program>
)
add_covered_test(Runner_UnitTests Runner*)
