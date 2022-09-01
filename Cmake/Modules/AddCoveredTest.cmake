# vim:ts=2:sw=2:et:sts=2:
#
# Copyright 2022 Thunder422.  All rights reserved.
# Distributed under GNU General Public License Version 3
# (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)

file(WRITE ${CMAKE_BINARY_DIR}/coverage.html "Coverage Tests Results Links\n")

function(add_covered_test unit_test coverage_files)
  add_test(${unit_test} ${unit_test})

  if(THUNDER_COVERAGE)
    list(LENGTH ARGV count)
    math(EXPR nfiles "${count} - 1")

    foreach(index RANGE 1 ${nfiles})
      list(GET ARGV ${index} file)
      list(APPEND file_list ${file})
    endforeach()

    add_test(NAME ${unit_test}Coverage
      COMMAND "${CMAKE_COMMAND}"
        -D "REAL_SOURCE_DIR=${CMAKE_SOURCE_DIR}"
        -D "REAL_BINARY_DIR=${CMAKE_BINARY_DIR}"
        -D "TEST_PROGRAM=${unit_test}"
        -D "TEST_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
        -D "TEST_FILES=${file_list}"
        -P "${CMAKE_SOURCE_DIR}/Cmake/TestCoverage.cmake"
    )

    string(LENGTH ${CMAKE_BINARY_DIR} CURRENT_BINARY_DIR_LENGTH)
    math(EXPR CURRENT_BINARY_DIR_LENGTH "${CURRENT_BINARY_DIR_LENGTH} + 1")
    string(SUBSTRING ${CMAKE_CURRENT_BINARY_DIR} ${CURRENT_BINARY_DIR_LENGTH} -1 unit_test_dir)
    set_tests_properties(${unit_test}Coverage PROPERTIES DEPENDS ${unit_test} RUN_SERIAL YES)
    file(APPEND ${CMAKE_BINARY_DIR}/coverage.html
      "<p><a href=\"${unit_test_dir}/coverage_${unit_test}.html/index.html\">${unit_test}</a></p>"
    )
  endif()
endfunction()
