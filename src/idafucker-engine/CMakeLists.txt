# CMakeList.txt : CMake project for idafucker-engine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (idafucker-engine "idafucker-engine.cpp" "idafucker-engine.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET idafucker-engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
