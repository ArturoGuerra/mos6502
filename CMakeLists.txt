cmake_minimum_required(VERSION 3.12)

project(mos6502 CXX C ASM)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(
    -Wall
    -Wextra
    -Werror
    -Wpedantic
)

add_executable(${PROJECT_NAME})

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    m6502.cpp
    main.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
