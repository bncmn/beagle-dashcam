# CMake Build Configuration for root of project
cmake_minimum_required(VERSION 3.18)
project(bbg_dashcam VERSION 1.0 DESCRIPTION "CMPT 433 Project" LANGUAGES CXX)

# Compiler options (inherited by sub-folders)
set(CMAKE_CXX_STANDARD 11)  # Use the desired C++ standard (e.g., 11, 14, 17, etc.)
#add_compile_options(-Wall -Werror -Wpedantic -Wextra)
add_compile_options(-fdiagnostics-color)
add_compile_options(-D _POSIX_C_SOURCE=200809L)

# Enable address sanitizer
# (Comment this out to make your code faster)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

# What folders to build
add_subdirectory(hal)
add_subdirectory(app)