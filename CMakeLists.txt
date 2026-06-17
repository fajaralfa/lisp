cmake_minimum_required(VERSION 3.10)
project(LispInterpreter C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Collect source files
set(SOURCES
    src/main.c
    src/file_reader.c
    src/tokenizer.c
    src/parser.c
    src/vm.c
)

# Create executable
add_executable(lisp ${SOURCES})

# Optional: enable warnings (GCC/Clang)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(lisp PRIVATE -Wall -Wextra -Wpedantic)
endif()