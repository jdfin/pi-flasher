cmake_minimum_required(VERSION 3.12.0)
project(flash VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_PARALLEL_LEVEL 1)

add_compile_options(-Wall)

add_library(flash_lib
    spi_dev.cpp
    mcp23x17_spi.cpp
    mcp23x17.cpp
    control.cpp
    abus.cpp
    dbus.cpp
    ihx.cpp
    flash.cpp
    hardware.cpp
)

link_libraries(flash_lib)

add_executable(mcp23x17_test mcp23x17_test.cpp)

add_executable(mcp23x17_show mcp23x17_show.cpp)

add_executable(flash flash_main.cpp)
