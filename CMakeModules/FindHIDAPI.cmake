# Try to find HIDAPI library
# Once done will define:
#
# HIDAPI_FOUND
# HIDAPI_INCLUDE_DIRS
# HIDAPI_LIBRARIES
#
# Can specify root search dir:
#
# HIDAPI_ROOT_DIR

if (HIDAPI_INCLUDE_DIRS)
    set (HIDAPI_FIND_QUIETLY TRUE)
endif ()

if (NOT HDIAPI_INCLUDE_DIRS)
    find_path (HIDAPI_INCLUDE_DIRS NAMES hidapi.h PATH SUFFIXES hidapi HINTS ${HIDAPI_ROOT_DIR})
    set (HIDAPI_INCLUDE_DIRS ${HIDAPI_INCLUDE_DIRS}/hidapi CACHE PATH "HIDAPI include directory")
endif ()

find_library (HIDAPI_LIBRARIES NAMES hidapi HINTS ${HIDAPI_ROOT_DIR})

if (HIDAPI_INCLUDE_DIRS AND HIDAPI_LIBRARIES)
    set (HIDAPI_FOUND TRUE)
endif ()

if (HIDAPI_FOUND)
    if (NOT HIDAPI_FIND_QUIETLY)
        message (STATUS "Found HIDAPI: ${HIDAPI_LIBRARIES}")
    endif ()
else ()
    if (HIDAPI_FIND_REQUIRED)
        message (FATAL_ERROR "Could NOT find HIDAPI")
    else ()
        message (STATUS "Could NOT find HIDAPI")
    endif ()
endif ()

mark_as_advanced (HIDAPI_INCLUDE_DIRS HIDAPI_LIBRARIES)
