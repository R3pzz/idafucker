cmake_minimum_required(VERSION 3.25)

# -------------------------------------------------
#  Quick vcpkg toolchain reminder
# -------------------------------------------------
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VCPKG_ROOT})
        message(STATUS "Tip: configure with  -DCMAKE_TOOLCHAIN_FILE=$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    else()
        message(STATUS "Tip: Install vcpkg and configure with  -DCMAKE_TOOLCHAIN_FILE=<vcpkg_root>/scripts/buildsystems/vcpkg.cmake")
    endif()
endif()

# -------------------------------------------------
#  Project meta
# -------------------------------------------------
project(IDAFucker
        VERSION 0.1.0
        DESCRIPTION "Cross-platform C++23 advanced debugger"
        LANGUAGES CXX C)

# -------------------------------------------------
#  Global build options
# -------------------------------------------------
set(CMAKE_CXX_STANDARD               23)
set(CMAKE_CXX_STANDARD_REQUIRED      ON)
set(CMAKE_CXX_EXTENSIONS             OFF)

option(HYPER_WARN_AS_ERROR   "Treat warnings as errors"     OFF)
option(HYPER_BUILD_TESTS     "Build unit/integration tests" OFF)
option(HYPER_BUILD_DOCS      "Build Doxygen documentation"  OFF)
option(BUILD_SHARED_LIBS     "Build libraries as shared"    OFF)

# Generate compile_commands.json for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -------------------------------------------------
#  Helper modules & toolchain-wide rules
# -------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Common)                      #  From cmake/Common.cmake

# -------------------------------------------------
#  Sub-projects
# -------------------------------------------------
add_subdirectory(bridge)             #  Platform abstraction library
add_subdirectory(fuse)               #  Extended STL helpers
add_subdirectory(unveil)             #  Disassembly engine
add_subdirectory(hyperui)            #  WebView2 / WebKitGTK UI kit
add_subdirectory(idafucker)          #  Main executable & runtime
add_subdirectory(modules)            #  Dynamically-loaded debugger modules

# -------------------------------------------------
#  Testing & docs (optional)
# -------------------------------------------------
if(HYPER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(HYPER_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    add_subdirectory(docs)
endif()

# Print a small summary at the end of configuration.
include(CMakePrintHelpers)
cmake_print_variables(CMAKE_BUILD_TYPE HYPER_WARN_AS_ERROR BUILD_SHARED_LIBS)
