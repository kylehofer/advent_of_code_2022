cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C%2B%2B standards
project(advent_of_code_2022 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)

