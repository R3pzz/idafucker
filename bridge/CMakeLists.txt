# -------- bridge : native API exposer -----------------
file(GLOB_RECURSE BRIDGE_SOURCES_WIN CONFIGURE_DEPENDS
     "src/bridge-win/*.cpp" "src/bridge-win/*.cxx" "src/bridge-win/*.s"
)
file(GLOB_RECURSE BRIDGE_HEADERS_WIN CONFIGURE_DEPENDS
     "include/bridge-win/**/*.hpp" "include/bridge-win/**/*.h"
)

find_package(spdlog REQUIRED)

add_library(bridge ${BRIDGE_SOURCES_WIN} ${BRIDGE_HEADERS_WIN} "include/bridge.hpp")
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
    PUBLIC_HEADER "${BRIDGE_HEADERS_WIN}"
)