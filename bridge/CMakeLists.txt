# -------- bridge : native API exposer -----------------
file(GLOB_RECURSE BRIDGE_SOURCES CONFIGURE_DEPENDS
     "src/*.cpp" "src/*.cxx" "src/*.s"
)
file(GLOB_RECURSE BRIDGE_HEADERS CONFIGURE_DEPENDS
     "include/**/*.hpp" "include/**/*.h"
)

find_package(spdlog REQUIRED)

add_library(bridge ${BRIDGE_SOURCES} ${BRIDGE_HEADERS} "include/bridge.hpp")
target_include_directories(bridge PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(bridge PUBLIC spdlog::spdlog_header_only)

# Our own libraries
target_link_libraries(bridge
    PUBLIC
        fuse
        unveil # Used for hook checking
)

target_enable_warnings(bridge)
set_target_properties(bridge PROPERTIES
    CXX_STANDARD 23
    PUBLIC_HEADER "${BRIDGE_HEADERS}"
)