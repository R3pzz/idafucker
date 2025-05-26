if(DEFINED __IDAFUCKER_COMMON__)
    return()
endif()
set(__IDAFUCKER_COMMON__ TRUE)

# -----------------------------------------------
#  Universal warning flags
# -----------------------------------------------
function(target_enable_warnings _tgt)
    if(MSVC)
        target_compile_options(${_tgt} PRIVATE /W4)
    else()
        target_compile_options(${_tgt} PRIVATE
            -Wall -Wextra -Wpedantic
            -Wconversion -Wsign-conversion
        )
    endif()

    if(HYPER_WARN_AS_ERROR)
        if(MSVC)
            target_compile_options(${_tgt} PRIVATE /WX)
        else()
            target_compile_options(${_tgt} PRIVATE -Werror)
        endif()
    endif()
endfunction()

# -----------------------------------------------
#  Pre-compiled headers (optional)
# -----------------------------------------------
function(target_setup_pch _tgt _header)
    if(CMAKE_GENERATOR MATCHES "Visual Studio")
        target_precompile_headers(${_tgt} PRIVATE "${_header}")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_precompile_headers(${_tgt} PRIVATE "${_header}")
    endif()
endfunction()