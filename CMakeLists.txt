# vim:ts=2:sw=2:et:sts=2:
#
# Copyright 2022 Thunder422.  All rights reserved.
# Distributed under GNU General Public License Version 3
# (See accompanying file LICENSE or <http://www.gnu.org/licenses/>)

project(thunder CXX)

cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -pedantic-errors)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR
    "\nbuilding in the source directory not allowed"
    "\nplease create a separate build directory"
  )
endif()

if(NOT $ENV{CMAKE_BUILD_TYPE} STREQUAL "")
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE $ENV{CMAKE_BUILD_TYPE})
  endif()
endif()
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Cmake/Modules)

enable_testing()

find_package(Catch2 3 REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  find_program(LCOV_PROGRAM lcov)
  find_program(GENHTML_PROGRAM genhtml)
  if(LCOV_PROGRAM AND GENHTML_PROGRAM)
    set(THUNDER_COVERAGE TRUE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    message(STATUS "Coverage enabled")
  else()
    message(WARNING "Coverage disabled: lcov not found")
  endif()
endif()
include(CoverageTesting)

if(THUNDER_COVERAGE)
  add_test(NAME ZeroCoverageData
    COMMAND ${LCOV_PROGRAM} -q --directory . --zerocounters
    WORKING_DIRECTORY ${REAL_BINARY_DIR}
  )
  set_tests_properties(ZeroCoverageData PROPERTIES FIXTURES_SETUP CoverageFixture)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_covered_subdirectory(Parser)
add_covered_subdirectory(Program)
add_covered_subdirectory(Compiler)
add_covered_subdirectory(Console)

if(THUNDER_COVERAGE)
  add_test(NAME CoverageTest
    COMMAND "${CMAKE_COMMAND}"
      -D "REAL_SOURCE_DIR=${CMAKE_SOURCE_DIR}"
      -D "REAL_BINARY_DIR=${CMAKE_BINARY_DIR}"
      -D "TEST_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
      -D "TEST_DIRS=${coverage_dir_list}"
      -P "${CMAKE_SOURCE_DIR}/Cmake/TestCoverage.cmake"
  )
  set_tests_properties(CoverageTest PROPERTIES FIXTURES_CLEANUP CoverageFixture)
endif()

add_executable(thunder main.cpp)
target_link_libraries(thunder
  $<TARGET_OBJECTS:Compiler>
  $<TARGET_OBJECTS:Console>
  $<TARGET_OBJECTS:Parser>
  $<TARGET_OBJECTS:Program>
)
