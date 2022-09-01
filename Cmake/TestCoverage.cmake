# vim:ts=2:sw=2:et:sts=2:
#
# Copyright 2022 Thunder422.  All rights reserved.
# Distributed under GNU General Public License Version 3
# (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
#
# testCoverage.cmake zeros coverage counters (*.gcda files), runs the specified executable,
# and if the executable succeeds, tests the coverage for that executable for 100% covered lines.
# a list of source files is provided for arguments, wildcards are permitted for file names.

if(NOT DEFINED TEST_PROGRAM)
  message(FATAL_ERROR "Requires TEST_PROGRAM to be defined")
endif()

set(LCOV_PROGRAM lcov)
set(GENHTML_PROGRAM genhtml)
message(STATUS "Running Coverage Test of ${TEST_PROGRAM}")

message(STATUS "Zeroing Coverage Counters")
execute_process(
  COMMAND ${LCOV_PROGRAM} -q --directory . --zerocounters
  WORKING_DIRECTORY ${REAL_BINARY_DIR}
)

message(STATUS "Running ${TEST_PROGRAM}:")
execute_process (
  COMMAND ${CMAKE_BINARY_DIR}/${TEST_PROGRAM}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  RESULT_VARIABLE TEST_PROGRAM_RESULT
)
if(TEST_PROGRAM_RESULT)
  message(FATAL_ERROR
    "Failed: Coverage not validated because test program failed: ${TEST_PROGRAM_RESULT}\n"
    "        Test program must succeed before coverage can be validated"
  )
endif()

message(STATUS "Generating Coverage Data")
execute_process(
  COMMAND ${LCOV_PROGRAM} -q -c -d . -o all.cov
  WORKING_DIRECTORY ${REAL_BINARY_DIR}
)

message(STATUS "Filtering coverage for: ${TEST_SOURCE_DIR}/*")
execute_process(
  COMMAND ${LCOV_PROGRAM} -q -e all.cov ${TEST_SOURCE_DIR}/* -o filtered.cov
  WORKING_DIRECTORY ${REAL_BINARY_DIR}
)
file(REMOVE ${REAL_BINARY_DIR}/all.cov)

file(REMOVE ${REAL_BINARY_DIR}/final.cov)
foreach(file IN LISTS TEST_FILES)
  message(STATUS "-- Extracting coverage for: ${TEST_SOURCE_DIR}/${file}")
  execute_process(
    COMMAND ${LCOV_PROGRAM} -q -e filtered.cov ${TEST_SOURCE_DIR}/${file} -o single.cov
    WORKING_DIRECTORY ${REAL_BINARY_DIR}
  )
  file(READ ${REAL_BINARY_DIR}/single.cov COV_CONTENTS)
  file(APPEND ${REAL_BINARY_DIR}/final.cov ${COV_CONTENTS})
endforeach()
file(REMOVE ${REAL_BINARY_DIR}/filtered.cov ${REAL_BINARY_DIR}/single.cov)

message(STATUS "Generating coverage HTML")
set(COV_DIR ${CMAKE_CURRENT_BINARY_DIR}/coverage_${TEST_PROGRAM}.html)
file(REMOVE ${COV_DIR})
execute_process(
  COMMAND ${GENHTML_PROGRAM} --precision=3 --demangle-cpp -o ${COV_DIR} final.cov
  OUTPUT_VARIABLE COV_OUTPUT
  WORKING_DIRECTORY ${REAL_BINARY_DIR}
)
file(REMOVE ${REAL_BINARY_DIR}/final.cov)

string(REGEX MATCH "lines[.]+: ([^%]+)% \\([0-9]+ of ([0-9]+) lines\\)" COV_LINES ${COV_OUTPUT})
if(NOT CMAKE_MATCH_1 STREQUAL "100.000")
  message(FATAL_ERROR
      "Failed: Coverage of ${TEST_PROGRAM} is not 100% (${CMAKE_MATCH_1}%)\n"
      "        Add new tests to complete coverage, details of coverage at:\n"
      "        ${COV_DIR}/index.html"
  )
endif()

message(STATUS "Coverage Test Passed (${CMAKE_MATCH_2} lines covered)")