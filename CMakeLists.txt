cmake_minimum_required (VERSION 3.0)

project (sixaxispairing C)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set (PACKAGE_DIRECTORY ${PROJECT_BINARY_DIR}/package)

find_package (HIDAPI REQUIRED)

set (SOURCES
    main.c
)

# Mac USB linking.
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_library(IOKIT NAMES IOKit)
    set (PLATFORM_LIBS
        ${IOKIT}
    )
endif ()

add_executable (${PROJECT_NAME} ${SOURCES})
target_link_libraries (${PROJECT_NAME} ${HIDAPI_LIBRARIES} ${PLATFORM_LIBS})
