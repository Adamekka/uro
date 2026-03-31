cmake_minimum_required(VERSION 3.10)
project(cv5)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CXX_FLAGS
    -Wall
    -Wcast-align
    -Wextra
    -Wformat
    -Wformat-security
    -Wno-shadow
    -Wno-unused-result
    -Wnull-dereference
    -Wredundant-decls
    -Wreturn-type
    -Wstrict-overflow
    -Wuninitialized
    -Wvla
    -fstack-protector-strong
    -fsanitize=address
    -fsanitize=undefined
)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

file(GLOB SOURCES src/*.cpp)
qt_add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(cv5 PRIVATE ${CXX_FLAGS})
target_link_options(cv5 PRIVATE -fsanitize=address -fsanitize=undefined)
target_link_libraries(cv5 PRIVATE Qt6::Widgets)
