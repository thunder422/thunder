# vim:ts=2:sw=2:et:sts=2:
#
# Copyright 2022 Thunder422.  All rights reserved.
# Distributed under GNU General Public License Version 3
# (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)

add_executable(ParseIdentifier_UnitTests
  Identifier.cpp
  Identifier_UnitTests.cpp
  Parser.h
  ${CATCH_SOURCES}
)
add_covered_test(ParseIdentifier_UnitTests Identifier*)

add_executable(ParseNumber_UnitTests
  Number.cpp
  Number_UnitTests.cpp
  Parser.h
  ${CATCH_SOURCES}
)
add_covered_test(ParseNumber_UnitTests Number*)
