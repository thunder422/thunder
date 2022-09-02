# vim:ts=2:sw=2:et:sts=2:
#
# Copyright 2022 Thunder422.  All rights reserved.
# Distributed under GNU General Public License Version 3
# (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)

set(ParseIdentifier_UnitTests_SOURCES
  Identifier.h
  Identifier.cpp
  Identifier_UnitTests.cpp
)
add_executable(ParseIdentifier_UnitTests ${ParseIdentifier_UnitTests_SOURCES} ${CATCH_SOURCES})
add_covered_test(ParseIdentifier_UnitTests ${ParseIdentifier_UnitTests_SOURCES})

set(ParseNumber_UnitTests_SOURCES
  Number.h
  Number.cpp
  Number_UnitTests.cpp
)
add_executable(ParseNumber_UnitTests ${ParseNumber_UnitTests_SOURCES} ${CATCH_SOURCES})
add_covered_test(ParseNumber_UnitTests ${ParseNumber_UnitTests_SOURCES})
