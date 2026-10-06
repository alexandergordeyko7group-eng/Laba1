cmake_minimum_required(VERSION 3.16)
project(Lab1_OS LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "This project requires Windows and WinAPI.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER "${CONFIG}" CONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} "${CMAKE_BINARY_DIR}/bin")
endforeach()

add_executable(Creator Creator.cpp employee.h)
add_executable(Reporter Reporter.cpp employee.h)
add_executable(Main Main.cpp employee.h)
target_link_libraries(Main PRIVATE kernel32)
add_dependencies(Main Creator Reporter)

if(MSVC)
    target_compile_options(Creator PRIVATE /utf-8)
    target_compile_options(Reporter PRIVATE /utf-8)
    target_compile_options(Main PRIVATE /utf-8)
endif()