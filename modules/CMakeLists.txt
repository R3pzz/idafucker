# Discover every immediate child directory containing a CMakeLists.txt
file(GLOB MODULE_DIRS CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")

foreach(dir ${MODULE_DIRS})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${dir}")
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/CMakeLists.txt")
            add_subdirectory("${dir}")
        endif()
    endif()
endforeach()