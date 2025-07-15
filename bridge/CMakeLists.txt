# -------- bridge : native API exposer -----------------

# Select the target platform folder
if (WIN32)
    set(BRIDGE_TARGET_SUBFOLDER "bridge-win")
else()
    set(BRIDGE_TARGET_SUBFOLDER "bridge-linux")
endif()

# Collect the sources
file(GLOB_RECURSE BRIDGE_SOURCES CONFIGURE_DEPENDS
    "src/${BRIDGE_TARGET_SUBFOLDER}/*.cpp"
    "src/${BRIDGE_TARGET_SUBFOLDER}/*.cxx"
    "src/${BRIDGE_TARGET_SUBFOLDER}/*.s"
)
file(GLOB_RECURSE BRIDGE_HEADERS CONFIGURE_DEPENDS
    "include/${BRIDGE_TARGET_SUBFOLDER}/**/*.hpp"
    "include/${BRIDGE_TARGET_SUBFOLDER}/**/*.h"
)

find_package(spdlog REQUIRED)

# Add the library
add_library(bridge ${BRIDGE_SOURCES} ${BRIDGE_HEADERS} "include/bridge.hpp")
target_include_directories(bridge PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(bridge PUBLIC spdlog::spdlog_header_only)
target_link_libraries(bridge PUBLIC fuse)

target_enable_warnings(bridge)
set_target_properties(bridge PROPERTIES
    CXX_STANDARD 23
    PUBLIC_HEADER "${BRIDGE_HEADERS}"
)

# Testing
if (INTROSPECT_BUILD_TESTS)
    # Collect the tests
    file(GLOB_RECURSE BRIDGE_TEST_SOURCES CONFIGURE_DEPENDS
        "tests/${BRIDGE_TARGET_SUBFOLDER}/*.cpp"
        "tests/${BRIDGE_TARGET_SUBFOLDER}/*.cxx"
        "tests/${BRIDGE_TARGET_SUBFOLDER}/*.s"
    )

    # Find gtest
    find_package(Catch2 CONFIG REQUIRED)

    # Add the executable and link up the libs
    add_executable(bridge_tests ${BRIDGE_TEST_SOURCES})
    target_link_libraries(bridge_tests PRIVATE bridge)
    target_link_libraries(bridge_tests PRIVATE Catch2::Catch2WithMain)

    # Finally, register our tests
    include(CTest)
    include(Catch)
    catch_discover_tests(bridge_tests)
endif()