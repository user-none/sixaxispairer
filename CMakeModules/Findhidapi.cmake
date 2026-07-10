# Try to find hidapi library
# Once done will define:
#
# hidapi_FOUND
# hidapi::hidapi
# hidapi_INCLUDE_DIRS

find_path(hidapi_INCLUDE_DIR
    NAMES hidapi.h
    PATH_SUFFIXES hidapi
)
mark_as_advanced(FORCE hidapi_INCLUDE_DIR)

find_library(hidapi_LIBRARY
    NAMES hidapi
)
mark_as_advanced(FORCE hidapi_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(hidapi DEFAULT_MSG
    hidapi_INCLUDE_DIR
    hidapi_LIBRARY
)

if(hidapi_FOUND)
    set(hidapi_INCLUDE_DIRS ${hidapi_INCLUDE_DIR})
    if (NOT TARGET hidapi::hidapi AND EXISTS "${hidapi_LIBRARY}" AND EXISTS "${hidapi_INCLUDE_DIR}")
        add_library(hidapi::hidapi UNKNOWN IMPORTED)
        set_target_properties(hidapi::hidapi PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES ${hidapi_INCLUDE_DIR}
            IMPORTED_LOCATION ${hidapi_LIBRARY}
        )
    endif()
endif()
