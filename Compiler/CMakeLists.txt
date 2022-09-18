# vim:ts=2:sw=2:et:sts=2:
#
# Copyright 2022 Thunder422.  All rights reserved.
# Distributed under GNU General Public License Version 3
# (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)

set(COMPILER_HEADERS
  Compiler.h
)
set(COMPILER_SOURCES
  Compiler.cpp
)
add_library(Compiler OBJECT ${COMPILER_HEADERS} ${COMPILER_SOURCES})

add_executable(Compiler_UnitTests
  Compiler.h
  Compiler.cpp
  Compiler_UnitTests.cpp
  ${CATCH_SOURCES}
)
target_link_libraries(Compiler_UnitTests
  $<TARGET_OBJECTS:Parser>
  $<TARGET_OBJECTS:Program>
)
add_covered_test(Compiler_UnitTests Compiler*)
