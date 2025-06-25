# -------- fuse : extended STL helpers -----------------
file(GLOB_RECURSE FUSE_SOURCES CONFIGURE_DEPENDS
     "src/*.cpp" "src/*.cxx"
)
file(GLOB_RECURSE FUSE_HEADERS CONFIGURE_DEPENDS
     "include/**/*.hpp" "include/**/*.h"
)

find_package(spdlog REQUIRED)

add_library(fuse ${FUSE_SOURCES} ${FUSE_HEADERS} "include/fuse.hpp")
target_include_directories(fuse PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(fuse PUBLIC spdlog::spdlog_header_only)

target_enable_warnings(fuse)
set_target_properties(fuse PROPERTIES
    CXX_STANDARD 23
    PUBLIC_HEADER "${FUSE_HEADERS}"
)