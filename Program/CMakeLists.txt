# vim:ts=2:sw=2:et:sts=2:
#
# Copyright 2022 Thunder422.  All rights reserved.
# Distributed under GNU General Public License Version 3
# (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)

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
  OpCode.cpp
  Word.h
  Word_UnitTests.cpp
  ${CATCH_SOURCES}
)
add_covered_test(ProgramWord_UnitTests Word*)
