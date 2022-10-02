# vim:ts=2:sw=2:et:sts=2:
#
# Copyright 2022 Thunder422.  All rights reserved.
# Distributed under GNU General Public License Version 3
# (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)
#
# TestCoverage.cmake collects coverage data from all of the unit tests that are assumed to have
# already been run (as configed by the top-level CMakeLists.txt, generates the HTML output of
# coverage, and tests that 100% of the lines have been covered.

set(LCOV_PROGRAM lcov)
set(GENHTML_PROGRAM genhtml)
message(STATUS "Running Coverage Test")

if(NOT DEFINED TEST_DIRS)
  message(FATAL_ERROR "Requires TEST_DIRS to be defined")
endif()
message(STATUS "Generating Coverage Data")
set(test_name "Coverage")
set(lcov_args "-q;-c;-d;.;-o;${test_name}")
foreach(dir IN LISTS TEST_DIRS)
  list(APPEND lcov_args --include ${TEST_SOURCE_DIR}/${dir}*)
endforeach()
execute_process(COMMAND ${LCOV_PROGRAM} ${lcov_args} WORKING_DIRECTORY ${REAL_BINARY_DIR})

message(STATUS "Generating coverage HTML")
set(COV_DIR ${CMAKE_CURRENT_BINARY_DIR}/coverage_html)
file(REMOVE ${COV_DIR})
execute_process(
  COMMAND ${GENHTML_PROGRAM} --precision=3 --demangle-cpp -o ${COV_DIR} ${test_name}
  OUTPUT_VARIABLE COV_OUTPUT
  WORKING_DIRECTORY ${REAL_BINARY_DIR}
)
file(REMOVE ${REAL_BINARY_DIR}/${test_name})

string(REGEX MATCH "lines[.]+: ([^%]+)% \\([0-9]+ of ([0-9]+) lines\\)" COV_LINES ${COV_OUTPUT})
if(NOT CMAKE_MATCH_1 STREQUAL "100.000")
  message(FATAL_ERROR
      "Failed: Coverage is not 100% (${CMAKE_MATCH_1}%)\n"
      "        Add new tests to complete coverage, details of coverage at:\n"
      "        ${COV_DIR}/index.html"
  )
endif()

message(STATUS "Coverage Test Passed (${CMAKE_MATCH_2} lines covered)")
