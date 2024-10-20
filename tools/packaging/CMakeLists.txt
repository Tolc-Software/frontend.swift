include_guard()

include(GNUInstallDirs)

# Export the Frontend.swift library
install(
  TARGETS Frontend.swift
  EXPORT Frontend.swiftTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
  EXPORT Frontend.swiftTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Frontend.swift
  NAMESPACE Tolc::)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/Frontend
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Documentation
install(DIRECTORY ${CMAKE_SOURCE_DIR}/docs/public
        DESTINATION ${CMAKE_INSTALL_DOCDIR})

# Need to be able to download conan packages
install(
  FILES ./cmake/ConanHelpers.cmake
        ${CMAKE_SOURCE_DIR}/tools/conan_profiles/${CMAKE_SYSTEM_NAME}/clang.in
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Frontend.swift)

# Add License
install(FILES ${CMAKE_SOURCE_DIR}/COPYRIGHT.md ${CMAKE_SOURCE_DIR}/LICENSE.md
        DESTINATION .)

configure_file(${CMAKE_SOURCE_DIR}/cmake/packaging/Frontend.swiftConfig.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/Frontend.swiftConfig.cmake @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Frontend.swiftConfig.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Frontend.swift)

# Export IR
install(
  TARGETS IR
  EXPORT IRTargets
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/IR)
install(
  EXPORT IRTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/IR
  NAMESPACE IR::)
install(FILES ${CMAKE_SOURCE_DIR}/cmake/packaging/IRConfig.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/IR)

set(CPACK_VERBATIM_VARIABLES YES)
# Allow packaging
include(CPack)
