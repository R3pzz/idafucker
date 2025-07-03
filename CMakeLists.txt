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
project(Introspect
        VERSION 0.1.0
        DESCRIPTION "The new-generation engine for game-hacking and RE with AI support"
        LANGUAGES CXX C)

# -------------------------------------------------
#  Global build options
# -------------------------------------------------
set(CMAKE_CXX_STANDARD               23)
set(CMAKE_CXX_STANDARD_REQUIRED      ON)
set(CMAKE_CXX_EXTENSIONS             OFF)

option(INTROSPECT_WARN_AS_ERROR "Treat warnings as errors"     OFF)
option(INTROSPECT_BUILD_TESTS   "Build unit/integration tests" OFF)
option(INTROSPECT_BUILD_DOCS    "Build Doxygen documentation"  OFF)
option(BUILD_SHARED_LIBS        "Build libraries as shared"    OFF)

# Generate compile_commands.json for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -------------------------------------------------
#  Helper modules & toolchain-wide rules
# -------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Common)

# -------------------------------------------------
#  Sub-projects
# -------------------------------------------------
add_subdirectory(bridge)
add_subdirectory(fuse)
add_subdirectory(unveil)
add_subdirectory(hyperui)
add_subdirectory(introspect)
add_subdirectory(modules)

# -------------------------------------------------
#  Testing & docs (optional)
# -------------------------------------------------
if(INTROSPECT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(INTROSPECT_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    add_subdirectory(docs)
endif()

# Print a small summary at the end of configuration.
include(CMakePrintHelpers)
cmake_print_variables(CMAKE_BUILD_TYPE INTROSPECT_WARN_AS_ERROR BUILD_SHARED_LIBS)
