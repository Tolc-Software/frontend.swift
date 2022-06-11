cmake_minimum_required(VERSION 3.15)

# Temporary workaround for clang 14
# https://github.com/llvm/llvm-project/issues/53950
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
  set(frontend_languages CXX)
else()
  set(frontend_languages C CXX)
endif()

project(
  TestStage
  VERSION 1.0
  LANGUAGES ${frontend_languages})

# Export compile flags to compile_commands.json database. Useful for linters and
# autocompletion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_language(OBJC)
enable_language(OBJCXX)
enable_language(Swift)

# Get the variable 'target_name'
# NOTE: This will be generated for each test
include(cmake/TargetName.cmake)

# The users C++ library
add_library(${target_name} INTERFACE)
target_include_directories(${target_name} INTERFACE include)

# Use C++17 since the tests are using std::optional etc.
set_target_properties(
  ${target_name}
  PROPERTIES CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  CXX_STANDARD 17)

# The Objective-C++ middle layer
add_library(${target_name}_objc src/${target_name}_objc.mm)
target_include_directories(${target_name}_objc PUBLIC src)
target_link_libraries(${target_name}_objc PRIVATE ${target_name})

# NOTE: Take the CXX version of the incoming library
#       and use it as the standard
set_property(TARGET ${target_name}_objc PROPERTY OBJCXX_STANDARD 17)

include(CTest)

# # The Objective-C exposed layer
add_executable(objecty tests/main.mm)
target_link_libraries(objecty PRIVATE ${target_name}_objc)
target_link_libraries(objecty PRIVATE "-framework Foundation")
add_test(NAME objecty
  COMMAND objecty)

# TODO: When Swift is stable, start testing that aswell
set(use_swift OFF)
if(use_swift)
  # The ObjcSwift middle layer
  add_library(${target_name}_swift SHARED src/${target_name}_swift.swift)
  target_link_libraries(${target_name}_swift PRIVATE ${target_name}_objc)
  set_target_properties(${target_name}_swift PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_BINARY_DIR}
    )
  set_property(TARGET ${target_name}_swift
    APPEND_STRING
    PROPERTY
    COMPILE_FLAGS
    "-import-objc-header ${CMAKE_CURRENT_LIST_DIR}/src/${target_name}-Bridging-Header.h")
  # The ObjcSwift exposed layer
  add_executable(swifty
    tests/main.swift)
  target_link_libraries(swifty PRIVATE
    ${target_name}_swift)
  add_test(NAME swifty
    COMMAND swifty)
endif()

