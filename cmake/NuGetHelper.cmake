if(DEFINED __IDAFUCKER_NUGET_HELPER__)
    return()
endif()
set(__IDAFUCKER_NUGET_HELPER__ TRUE)

# -----------------------------------------------
#  Finding a package
# -----------------------------------------------
function(nuget_install_package _pkg_name _install_dir)
    # A Windows-only function
    if(NOT MSVC)
        return()
    endif()

    # Try to find 
    find_program(NUGET_EXE NAMES nuget)
    if(NOT NUGET_EXE)
	    message("No instance of NuGet executable was found.")
	    message(FATAL_ERROR "Please, install NuGet, and run CMake again.")
    endif()

    execute_process(COMMAND ${NUGET_EXE} install ${_pkg_name} -ExcludeVersion -OutputDirectory ${_install_dir})
endfunction()