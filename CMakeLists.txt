cmake_minimum_required(VERSION 3.14)
project(cpp-orm 
    VERSION 1.0.0
    DESCRIPTION "C++ ORM header-only library"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(cpp-orm INTERFACE)
add_library(cpp-orm::cpp-orm ALIAS cpp-orm)

target_include_directories(cpp-orm
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp"
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cpp-orm-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cpp-orm-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp-orm
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/cpp-orm-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cpp-orm-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cpp-orm-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp-orm
)

install(EXPORT cpp-orm-targets
    FILE cpp-orm-targets.cmake
    NAMESPACE cpp-orm::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp-orm
)