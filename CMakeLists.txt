cmake_minimum_required(VERSION 3.10)

project(oni)

set(ONI_SOURCES "" CACHE INTERNAL "")
set(ONI_INCLUDE_DIRS "" CACHE INTERNAL "")

set(CMAKE_CXX_STANDARD 11)

macro(oni_add_dependency)
file(
    GLOB SRCS

    "./*.c"
    "./*.cpp"

    "./*.h"
    "./*.hpp"
)
set(ONI_SOURCES

    ${ONI_SOURCES}
    ${SRCS}

    CACHE INTERNAL ""
)
set(ONI_INCLUDE_DIRS

    ${ONI_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    " "

    CACHE INTERNAL ""
)
endmacro()

add_subdirectory(dependencies)
add_subdirectory(source)

add_executable(${PROJECT_NAME}

    ${ONI_SOURCES}
)

message(${ONI_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME}

    PRIVATE ${ONI_INCLUDE_DIRS}
)
