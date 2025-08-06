cmake_minimum_required(VERSION 3.15)
project(Llangardaix LANGUAGES CXX)

# set the CPP standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# find the Sparta framework
if(NOT DEFINED ENV{SPARTA_ROOT})
    message(WARNING "SPARTA_ROOT environment variable is not set, trying to find Sparta in conda environment")
    # Try to guess SPARTA_ROOT based on conda environment (example)
    if(DEFINED ENV{CONDA_PREFIX})
        set(SPARTA_ROOT "$ENV{CONDA_PREFIX}")
        message(STATUS "Using SPARTA_ROOT from CONDA_PREFIX: ${SPARTA_ROOT}")
    else()
        message(FATAL_ERROR "SPARTA_ROOT environment variable not set and CONDA_PREFIX not found. Please set SPARTA_ROOT manually.")
    endif()
else()
    set(SPARTA_ROOT "$ENV{SPARTA_ROOT}")
    message(STATUS "Using SPARTA_ROOT from environment: ${SPARTA_ROOT}")
endif()

include_directories(${SPARTA_ROOT}/include)
link_directories(${SPARTA_ROOT}/lib)

set(SOURCES
    src/main.cpp
    src/CPUFactory.cpp
)

add_executable(llangardaix ${SOURCES})

target_link_libraries(llangardaix
    sparta_core
    sparta_utils
    sparta_simulation
    sparta_app
)

target_compile_options(llangardaix PRIVATE -Wall -Wextra -Wpedantic)

