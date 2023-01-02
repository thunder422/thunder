# vim:ts=2:sw=2:et:sts=2:
#
# Copyright 2022 Thunder422.  All rights reserved.
# Distributed under GNU General Public License Version 3
# (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)

set(CONSOLE_HEADERS
  CommandParser.h
  Console.h
)
set(CONSOLE_SOURCES
  CommandParser.cpp
  Console.cpp
)
add_library(Console OBJECT ${CONSOLE_HEADERS} ${CONSOLE_SOURCES})

add_executable(Console_UnitTests
  ${CONSOLE_HEADERS}
  ${CONSOLE_SOURCES}
  Console_UnitTests.cpp
)
target_link_libraries(Console_UnitTests
  PUBLIC
  $<TARGET_OBJECTS:Compiler>
  $<TARGET_OBJECTS:Parser>
  $<TARGET_OBJECTS:Program>
  PRIVATE Catch2::Catch2WithMain
)
add_covered_test(Console_UnitTests)
