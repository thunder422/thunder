# vim:ts=2:sw=2:et:sts=2:
#
# Copyright 2022 Thunder422.  All rights reserved.
# Distributed under GNU General Public License Version 3
# (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)

function(add_covered_test unit_test)
  add_test(${unit_test} ${unit_test})

  if(THUNDER_COVERAGE)
    set_tests_properties(${unit_test} PROPERTIES FIXTURES_REQUIRED CoverageFixture RUN_SERIAL YES)
  endif()
endfunction()

function(add_covered_subdirectory directory)
  add_subdirectory(${directory})
  if(THUNDER_COVERAGE)
    list(APPEND coverage_dir_list ${directory})
    set(coverage_dir_list ${coverage_dir_list} PARENT_SCOPE)
  endif()
endfunction()
