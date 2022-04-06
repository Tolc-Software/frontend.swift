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
  GIT_TAG v0.14.0)

FetchContent_MakeAvailable(IRSpecification)

add_library(
  Frontend.swift
  src/Frontend/ObjcSwift/frontend.cpp
  src/ObjcSwift/Builders/attributeBuilder.cpp
  src/ObjcSwift/Builders/classBuilder.cpp
  src/ObjcSwift/Builders/enumBuilder.cpp
  src/ObjcSwift/Builders/functionBuilder.cpp
  src/ObjcSwift/Builders/moduleBuilder.cpp
  src/ObjcSwift/Builders/moduleFileBuilder.cpp
  src/ObjcSwift/Builders/typeToStringBuilder.cpp
  src/ObjcSwift/Helpers/getDocumentationParameter.cpp
  src/ObjcSwift/Helpers/operatorNames.cpp
  src/ObjcSwift/Helpers/split.cpp
  src/ObjcSwift/Helpers/string.cpp
  src/ObjcSwift/Helpers/trampolineClass.cpp
  src/ObjcSwift/Helpers/types.cpp
  src/ObjcSwift/Proxy/attribute.cpp
  src/ObjcSwift/Proxy/class.cpp
  src/ObjcSwift/Proxy/enum.cpp
  src/ObjcSwift/Proxy/function.cpp
  src/ObjcSwift/Proxy/module.cpp
  src/ObjcSwift/Proxy/moduleFile.cpp
  src/ObjcSwift/checkType.cpp
  src/ObjcSwift/getOverloadedFunctions.cpp
  src/ObjcSwift/returnValuePolicy.cpp)

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
