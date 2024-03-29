include_guard()

include(${modules}/GetParser.cmake)
get_parser(VERSION v0.6.0)

# Set the include path for the system library in the variable
# We are using the standard library shipped
# with the downloaded llvm for include paths in the parsing
get_parser_system_include(
  VARIABLE
  SYSTEM_INCLUDE_PATHS
  PARSER_SOURCE_DIR
  ${parser_SOURCE_DIR}
  LIBCPP_ROOT_DIR
  ${parser_SOURCE_DIR}/lib/Parser
  LLVM_VERSION
  ${PARSER_LLVM_VERSION})

# If Parser was built from source, the file should only be expanded with empty strings
# and therefore use the default config from Parser.
configure_file(
  TestUtil/include/TestUtil/parserConfig.hpp.in
  ${CMAKE_CURRENT_LIST_DIR}/TestUtil/include/TestUtil/parserConfig.hpp @ONLY)

# Defines create_catch2_test and get_catch2
include(${modules}/Catch2Helpers.cmake)

get_catch2(VERSION v2.13.8)

include(FetchContent)
FetchContent_Declare(
  stage_content
  GIT_REPOSITORY https://github.com/Tolc-Software/Stage.git
  GIT_TAG main)
FetchContent_MakeAvailable(stage_content)

# Create a platform independent way of getting the path to the test stage
set(test_stage ${CMAKE_CURRENT_LIST_DIR}/testStage)
set(examples_dir ${PROJECT_SOURCE_DIR}/docs/examples)
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
  # This is to normalize strings relative to how they are used in std::filesystem
  string(REPLACE "/" "\\\\" test_stage ${test_stage})
  string(REPLACE "/" "\\\\" examples_dir ${examples_dir})
endif()
configure_file(
  TestUtil/include/TestStage/paths.cpp.in
  ${CMAKE_CURRENT_LIST_DIR}/TestUtil/include/TestStage/paths.cpp @ONLY)

# Everything put in TestUtil/include is available
add_library(TestUtil
  TestUtil/include/TestUtil/objcSwiftStage.cpp
  TestUtil/include/TestUtil/parse.cpp
  TestUtil/include/TestStage/paths.cpp
  )
target_include_directories(TestUtil PUBLIC TestUtil/include)
target_link_libraries(TestUtil PRIVATE Tolc::Parser Tolc::Frontend.swift Catch2::Catch2 Tolc::Stage)
target_link_libraries(TestUtil PUBLIC IR::IR)
find_package(fmt REQUIRED)

add_warnings(TARGET TestUtil)
add_options(TARGET TestUtil)

# Defines create_catch2_test
include(${modules}/Catch2Helpers.cmake)

if(ENABLE_COVERAGE)
  include(${modules}/CodeCoverage.cmake)

  # Use coverage of frontend
  target_code_coverage(Frontend.swift)

  add_code_coverage_all_targets()
endif()

# Create a test fixture for downloading dependencies in the test stage
add_test(
  NAME ConfigureTestStage
  WORKING_DIRECTORY ${test_stage}
  COMMAND ${CMAKE_COMMAND} -S. -Bbuild -GNinja)
set_tests_properties(ConfigureTestStage PROPERTIES FIXTURES_SETUP TestStage)

foreach(
  test

  # Objc/Builders/enumBuilder.cpp
  # ObjcSwift/Builders/attributeBuilder.cpp
  # ObjcSwift/Builders/classBuilder.cpp
  # ObjcSwift/Builders/functionBuilder.cpp
  # ObjcSwift/Builders/moduleFileBuilder.cpp
  # ObjcSwift/Builders/typeToStringBuilder.cpp
  # ObjcSwift/Helpers/combine.cpp
  # ObjcSwift/Proxy/attribute.cpp
  # ObjcSwift/Proxy/class.cpp
  # ObjcSwift/Proxy/enum.cpp
  # ObjcSwift/Proxy/function.cpp
  # ObjcSwift/Proxy/module.cpp
  # ObjcSwift/Proxy/moduleFile.cpp
  ObjcSwift/Helpers/typeToStringBuilder.cpp

  # complex.cpp
  # functional.cpp
  # inheritence.cpp
  # operators.cpp
  # variants.cpp
  # virtualInheritence.cpp
  arrays.cpp
  builtins.cpp
  classes.cpp
  defaultArgNames.cpp
  deques.cpp
  documentationStyles.cpp
  enums.cpp
  filesystem_paths.cpp
  functions.cpp
  globalVariables.cpp
  libraryNSClash.cpp
  lists.cpp
  maps.cpp
  memberVariables.cpp
  namespaces.cpp
  optionals.cpp
  order.cpp
  overloadedFunctions.cpp
  pairs.cpp
  sets.cpp
  smartPointers.cpp
  templates.cpp
  tuples.cpp
  unordered_maps.cpp
  unordered_sets.cpp
  userDefined.cpp
  valarrays.cpp
  vectors.cpp

  )
  # Ex: ObjcSwift/Proxy/moduleFile.cpp -> moduleFile
  get_filename_component(testName ${test} NAME_WLE)

  create_catch2_test(
    TARGET
    ${testName}
    SOURCES
    ${test}
    INCLUDE
    ${PROJECT_SOURCE_DIR}/src
    LINK_LIBRARIES
    Frontend.swift
    TestUtil
    Tolc::Stage
    fmt::fmt)

  add_warnings(TARGET ${testName})
  add_options(TARGET ${testName})

  set(ENABLE_SANITIZER_ADDRESS TRUE)
  set(ENABLE_SANITIZER_MEMORY TRUE)
  enable_sanitizers(${testName})

  string(FIND "${test}" "/" subdirectory)
  if(subdirectory EQUAL -1)
    # The test is in the root => should be an integration test

    # Require that the dependencies are downloaded so they can be copied to a
    # temporary test stage
    set_tests_properties(${testName} PROPERTIES FIXTURES_REQUIRED TestStage)
  endif()
endforeach()
