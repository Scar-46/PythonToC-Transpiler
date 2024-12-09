cmake_minimum_required(VERSION 3.15)

project(Transpiler VERSION 1.0 LANGUAGES C CXX)

# Enable C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define directories
set(SRC_DIR src)
set(OBJ_DIR build)
set(BIN_DIR bin)

# Collect source files
file(GLOB_RECURSE SOURCES_C "${SRC_DIR}/*.c")
file(GLOB_RECURSE SOURCES_CPP "${SRC_DIR}/*.cpp")

# Create output directories
file(MAKE_DIRECTORY ${OBJ_DIR})
file(MAKE_DIRECTORY ${BIN_DIR})

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES_C} ${SOURCES_CPP})

# Include directories
include_directories(${SRC_DIR})

# Compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
