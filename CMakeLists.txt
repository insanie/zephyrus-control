cmake_minimum_required(VERSION 3.27)
project(zephyrus_control C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(${PROJECT_NAME} main.c yaml_parsers.c atkacpi_ops.c ryzenadj_ops.c)
include_directories(${CMAKE_SOURCE_DIR})

find_package(yaml CONFIG REQUIRED)
link_directories(${CMAKE_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE yaml)
target_link_libraries(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}\\libryzenadj.a")
