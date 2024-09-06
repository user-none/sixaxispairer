cmake_minimum_required(VERSION 3.18)

project(sixaxispairer VERSION 1.0.0 LANGUAGES C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(PACKAGE_DIRECTORY ${PROJECT_BINARY_DIR}/package)

find_package(hidapi REQUIRED)

set(SOURCES
    main.c
)

# Mac USB linking.
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_library(IOKIT NAMES IOKit)
    set (PLATFORM_LIBS
        ${IOKIT}
    )
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${hidapi_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} hidapi::hidapi ${PLATFORM_LIBS})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
