cmake_minimum_required(VERSION 3.15)

project(
  Frontend.swift
  VERSION 0.1.0
  LANGUAGES CXX)

configure_file(docs/ReleaseNotes/version.in
               ${CMAKE_CURRENT_LIST_DIR}/docs/ReleaseNotes/version @ONLY)

include(FetchContent)
FetchContent_Declare(
  cmake_modules
  GIT_REPOSITORY https://github.com/Tolc-Software/cmake-modules.git
  GIT_TAG main)
FetchContent_MakeAvailable(cmake_modules)
# Basis of all shared CMake modules
set(modules ${cmake_modules_SOURCE_DIR}/cmake)

# Set ccache, ipo, etc.
include(${modules}/ProjectSettings.cmake)

include(${modules}/WarningsAndOptions.cmake)

include(${modules}/Sanitizers.cmake)

# Define run_conan
include(${modules}/ConanHelpers.cmake)
# Create the profile from variables known during configure
setup_conan_profile(VARIABLE frontend_conan_profile)
# This helps with packaging
set(FRONTEND.PY_CONAN_REQUIRES_PACKAGES spdlog/1.9.2 fmt/8.1.1)
find_conan_packages(REQUIRES ${FRONTEND.PY_CONAN_REQUIRES_PACKAGES} PROFILE
                    ${frontend_conan_profile})

include(FetchContent)
FetchContent_Declare(
  IRSpecification
  GIT_REPOSITORY https://github.com/Tolc-Software/IntermediateRepresentation.git
  GIT_TAG v0.15.1)

FetchContent_MakeAvailable(IRSpecification)

add_library(
  Frontend.swift

  src/Frontend/ObjcSwift/frontend.cpp
  src/ObjcSwift/Helpers/getDocumentationParameter.cpp
  src/ObjcSwift/Helpers/operatorNames.cpp
  src/ObjcSwift/Helpers/split.cpp
  src/ObjcSwift/Helpers/string.cpp
  src/ObjcSwift/Helpers/typeToStringBuilder.cpp
  src/ObjcSwift/Helpers/types.cpp
  src/ObjcSwift/Helpers/wrapInFunction.cpp
  src/ObjcSwift/getOverloadedFunctions.cpp

  src/Swift/Builders/attributeBuilder.cpp
  src/Swift/Builders/classBuilder.cpp
  src/Swift/Builders/enumBuilder.cpp
  src/Swift/Builders/functionBuilder.cpp
  src/Swift/Builders/moduleBuilder.cpp
  src/Swift/Builders/moduleFileBuilder.cpp
  src/Swift/Proxy/attribute.cpp
  src/Swift/Proxy/class.cpp
  src/Swift/Proxy/enum.cpp
  src/Swift/Proxy/function.cpp
  src/Swift/Proxy/function.cpp
  src/Swift/Proxy/module.cpp
  src/Swift/Proxy/moduleFile.cpp
  src/Swift/getName.cpp
  src/Swift/types.cpp

  src/Objc/utility.cpp
  src/Objc/Builders/attributeBuilder.cpp
  src/Objc/Builders/classBuilder.cpp
  src/Objc/Builders/enumBuilder.cpp
  src/Objc/Builders/functionBuilder.cpp
  src/Objc/Builders/moduleBuilder.cpp
  src/Objc/Builders/moduleFileBuilder.cpp
  src/Objc/Builders/typeBuilder.cpp
  src/Objc/Conversions/Container/container.cpp
  src/Objc/Conversions/Container/helpers.cpp
  src/Objc/Conversions/Container/value.cpp
  src/Objc/Conversions/base.cpp
  src/Objc/Conversions/container.cpp
  src/Objc/Conversions/enum.cpp
  src/Objc/Conversions/getConversionName.cpp
  src/Objc/Conversions/userDefined.cpp
  src/Objc/Conversions/utility.cpp
  src/Objc/Proxy/attribute.cpp
  src/Objc/Proxy/class.cpp
  src/Objc/Proxy/enum.cpp
  src/Objc/Proxy/function.cpp
  src/Objc/Proxy/function.cpp
  src/Objc/Proxy/moduleFile.cpp
  src/Objc/getName.cpp

)

add_warnings(TARGET Frontend.swift)
add_options(TARGET Frontend.swift)

# Set the sanitizer options on project_options
enable_sanitizers(Frontend.swift)

target_link_libraries(
  Frontend.swift
  PUBLIC IR::IR
  PRIVATE fmt::fmt)

target_include_directories(Frontend.swift PRIVATE src)
target_include_directories(
  Frontend.swift PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                     $<INSTALL_INTERFACE:include>)

set_target_properties(
  Frontend.swift
  PROPERTIES CXX_STANDARD_REQUIRED ON
             CXX_EXTENSIONS OFF)

# Add library to link against
add_library(Tolc::Frontend.swift ALIAS Frontend.swift)

# Only build tests/benchmarks/packaging if building this project directly
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(ENABLE_TESTING "Build the testing tree" OFF)
  if(ENABLE_TESTING)
    option(ENABLE_COVERAGE "Use code coverage tools on the tests" OFF)
    enable_testing()
    add_subdirectory(tests)
  endif()

  option(ENABLE_BENCHMARKS "Build the benchmarks" OFF)
  if(ENABLE_BENCHMARKS)
    # TODO: Add benchmarks
  endif()

  option(ENABLE_PACKAGING "Enables install and packaging via CPack" OFF)
  if(ENABLE_PACKAGING)
    # add_subdirectory(tools/packaging)
  endif()
endif()
