# vim:ts=2:sw=2:et:sts=2:
#
# Copyright 2022 Thunder422.  All rights reserved.
# Distributed under GNU General Public License Version 3
# (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)

set(PARSER_HEADERS
  Error.h
  Parser.h
)
set(PARSER_SOURCES
  Identifier.cpp
  Number.cpp
  Parser.cpp
)
add_library(Parser OBJECT ${PARSER_HEADERS} ${PARSER_SOURCES})

add_executable(Identifier_UnitTests
  Identifier.cpp
  Identifier_UnitTests.cpp
  Parser.h
  ${CATCH_SOURCES}
)
add_covered_test(Identifier_UnitTests)

add_executable(Number_UnitTests
  Number.cpp
  Number_UnitTests.cpp
  Parser.h
  ${CATCH_SOURCES}
)
add_covered_test(Number_UnitTests)
