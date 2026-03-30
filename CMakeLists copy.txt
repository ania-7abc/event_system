cmake_minimum_required(VERSION 3.14)
project(base64 VERSION 1.0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

add_library(base64 src/base64.cpp)
set_target_properties(base64 PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)
target_include_directories(base64 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libsodium REQUIRED IMPORTED_TARGET libsodium)
target_link_libraries(base64 PRIVATE PkgConfig::libsodium)

install(TARGETS base64 EXPORT base64Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT base64Targets
    FILE base64Targets.cmake
    NAMESPACE ania-7abc::
    DESTINATION lib/cmake/base64
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/base64Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/base64Config.cmake
    INSTALL_DESTINATION lib/cmake/base64
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/base64ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/base64Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/base64ConfigVersion.cmake
    DESTINATION lib/cmake/base64
)