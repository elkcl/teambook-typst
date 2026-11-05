cmake_minimum_required(VERSION 3.24)

project(teambook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE ALGO_HEADERS CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/content/*.hpp")
set(TEST_SOURCES "")
foreach(header ${ALGO_HEADERS})
  get_filename_component(fname "${header}" NAME)
  if(NOT fname STREQUAL "code.hpp")
    continue()
  endif()
  file(RELATIVE_PATH rel "${CMAKE_CURRENT_SOURCE_DIR}/content" "${header}")
  get_filename_component(rel_dir "${rel}" DIRECTORY)
  set(test_file "${CMAKE_CURRENT_SOURCE_DIR}/tests/${rel_dir}/test.cpp")
  if(NOT EXISTS "${test_file}")
    message(FATAL_ERROR
      "Нет тест-заглушки для content/${rel}\n"
      "Ожидался файл tests/${rel_dir}/test.cpp\n"
      "Создайте заглушки: python3 tests/gen_stubs.py")
  endif()
  list(APPEND TEST_SOURCES "${test_file}")
endforeach()

if(NOT TEST_SOURCES)
  message(FATAL_ERROR "Не найдено ни одного content/**/code.hpp")
endif()

add_executable(algo_tests ${TEST_SOURCES})
target_include_directories(algo_tests PRIVATE "${CMAKE_SOURCE_DIR}")
target_link_libraries(algo_tests PRIVATE GTest::gtest_main)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(algo_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(GoogleTest)
gtest_discover_tests(algo_tests
  DISCOVERY_MODE PRE_TEST
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
