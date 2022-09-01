# vim:ts=2:sw=2:et:sts=2:
#
# Copyright 2022 Thunder422.  All rights reserved.
# Distributed under GNU General Public License Version 3
# (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)

set(ParseNumber_UnitTests_SOURCES
  Number.h
  Number.cpp
  Number_UnitTests.cpp
)
add_executable(ParseNumber_UnitTests ${ParseNumber_UnitTests_SOURCES} ${CATCH_SOURCES})
target_link_libraries(ParseNumber_UnitTests ${GCOV_LIB})
add_covered_test(ParseNumber_UnitTests ${ParseNumber_UnitTests_SOURCES})
