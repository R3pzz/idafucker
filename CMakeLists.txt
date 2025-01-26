# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("idafucker")
# CMAKE_PROJECT_NAME - global project name
# PROJECT_NAME - last project() call

# Listing all source files from ./src
file(GLOB_RECURSE SOURCES "./src/*")

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# todo : add third_party here

# Linking modules
file(GLOB_RECURSE MODULES "./modules/*")
foreach(MODULE IN LISTS MODULES)
    add_subdirectory(${MODULE})
endforeach()

target_include_directories(${PROJECT_NAME} PRIVATE "./include/*")

target_link_libraries(
  ${PROJECT_NAME} # PUBLIC deps here
)