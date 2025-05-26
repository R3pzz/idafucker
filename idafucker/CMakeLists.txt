# Gather sources
file(GLOB_RECURSE IDAFUCKER_CORE_SOURCES CONFIGURE_DEPENDS
     "src/*.cpp" "src/*.cxx"
)
file(GLOB_RECURSE IDAFUCKER_HEADERS CONFIGURE_DEPENDS
     "include/**/*.hpp" "include/**/*.h"
)

find_package(spdlog CONFIG REQUIRED)

# Remove main.cpp from core sources
list(FILTER IDAFUCKER_CORE_SOURCES EXCLUDE REGEX "main\\.cpp$")

# 1. Core library
add_library(idafucker_core ${IDAFUCKER_CORE_SOURCES} ${IDAFUCKER_HEADERS})
target_include_directories(idafucker_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(idafucker_core
    PUBLIC
        spdlog::spdlog_header_only
)
target_link_libraries(idafucker_core
    PUBLIC
        fuse
        unveil
        hyperui
)
target_enable_warnings(idafucker_core)

# 2. Executable
add_executable(idafucker src/idafucker.cpp)
target_link_libraries(idafucker PRIVATE idafucker_core)
target_enable_warnings(idafucker)

# Add Unicode for Windows
if(WIN32)
    target_compile_definitions(idafucker_core PUBLIC UNICODE _UNICODE)
    target_compile_definitions(idafucker PRIVATE UNICODE _UNICODE)
endif()