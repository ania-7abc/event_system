cmake_minimum_required(VERSION 3.30)
project(event_system VERSION 2.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

add_library(event_system src/event_bus.cpp)
set_target_properties(event_system PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)
target_include_directories(event_system PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

find_package(Boost CONFIG REQUIRED)
target_link_libraries(event_system PUBLIC Boost::headers)

install(TARGETS event_system EXPORT event_systemTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT event_systemTargets
    FILE event_systemTargets.cmake
    NAMESPACE ania-7abc::
    DESTINATION lib/cmake/event_system
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/event_systemConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/event_systemConfig.cmake
    INSTALL_DESTINATION lib/cmake/event_system
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/event_systemConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/event_systemConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/event_systemConfigVersion.cmake
    DESTINATION lib/cmake/event_system
)
