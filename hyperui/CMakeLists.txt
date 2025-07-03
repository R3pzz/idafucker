# -------- hyperui : HTML/CSS/JS UI library ------------
file(GLOB_RECURSE HYPERUI_SOURCES CONFIGURE_DEPENDS
     "src/*.cpp" "src/*.cxx"
)
file(GLOB_RECURSE HYPERUI_HEADERS CONFIGURE_DEPENDS
     "include/**/*.hpp" "include/**/*.h"
)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# Platform-specific UI backend
if(WIN32)
    include(NuGetHelper)
    nuget_install_package(Microsoft.Web.WebView2 ${CMAKE_SOURCE_DIR}/packages)
    nuget_install_package(Microsoft.Windows.ImplementationLibrary ${CMAKE_SOURCE_DIR}/packages)
endif()

add_library(hyperui ${HYPERUI_SOURCES} ${HYPERUI_HEADERS} "include/hyperui.hpp")
target_include_directories(hyperui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
# Packages
target_link_libraries(hyperui
    PUBLIC
        nlohmann_json::nlohmann_json
        spdlog::spdlog_header_only
)

# WebView2 for Windows build
if(WIN32)
    target_link_libraries(hyperui 
        PRIVATE
            ${CMAKE_SOURCE_DIR}/packages/Microsoft.Web.WebView2/build/native/Microsoft.Web.WebView2.targets
            ${CMAKE_SOURCE_DIR}/packages/Microsoft.Windows.ImplementationLibrary/build/native/Microsoft.Windows.ImplementationLibrary.targets
    )
endif()

# Our own libraries
target_link_libraries(hyperui
    PUBLIC
        fuse
        introspect
)

target_enable_warnings(hyperui)
set_target_properties(hyperui PROPERTIES
    CXX_STANDARD 23
    PUBLIC_HEADER "${HYPERUI_HEADERS}"
    VS_GLOBAL_WebView2LoaderPreference "Static"
)

# Add Unicode for Windows
if(WIN32)
    target_compile_definitions(hyperui PUBLIC UNICODE _UNICODE)
endif()