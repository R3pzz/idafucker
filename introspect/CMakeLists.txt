# Gather sources
file(GLOB_RECURSE INTROSPECT_SOURCES CONFIGURE_DEPENDS
     "src/*.cpp" "src/*.cxx"
)
file(GLOB_RECURSE INTROSPECT_HEADERS CONFIGURE_DEPENDS
     "include/**/*.hpp" "include/**/*.h"
)

find_package(spdlog CONFIG REQUIRED)

# 1. Core library
add_executable(introspect ${INTROSPECT_SOURCES} ${INTROSPECT_HEADERS})
target_include_directories(introspect PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(introspect
    PUBLIC
        bridge
        spdlog::spdlog_header_only
)
target_link_libraries(introspect
    PUBLIC
        fuse
        unveil
        hyperui
)
target_enable_warnings(introspect)

# Add Unicode for Windows
if(WIN32)
    target_compile_definitions(introspect PRIVATE UNICODE _UNICODE)
endif()