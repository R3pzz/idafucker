# -------- unveil : extended STL helpers -----------------
file(GLOB_RECURSE UNVEIL_SOURCES CONFIGURE_DEPENDS
     "src/*.cpp" "src/*.cxx"
)
file(GLOB_RECURSE UNVEIL_HEADERS CONFIGURE_DEPENDS
     "include/**/*.hpp" "include/**/*.h"
)

find_package(spdlog CONFIG REQUIRED)
find_package(Zydis REQUIRED)

add_library(unveil ${UNVEIL_SOURCES} ${UNVEIL_HEADERS})
target_include_directories(unveil PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
# Packages
target_link_libraries(unveil
    PUBLIC
        spdlog::spdlog_header_only
    PRIVATE
        Zydis::Zydis
)

# Our own libraries
target_link_libraries(unveil
    PUBLIC
        fuse
)

target_enable_warnings(unveil)
set_target_properties(unveil PROPERTIES
    CXX_STANDARD 23
    PUBLIC_HEADER "${UNVEIL_HEADERS}"
    # Visual Studio 17 2022 can't find a linker language,
    # probably because of the C files from Zydis
    LINKER_LANGUAGE CXX
)