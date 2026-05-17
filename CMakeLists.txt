cmake_minimum_required(VERSION 3.15)
project(EFramework LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Source root and include path
set(SOURCE_ROOT ${CMAKE_SOURCE_DIR}/source)
include_directories(${SOURCE_ROOT})

# Collect all .cpp files recursively
file(GLOB_RECURSE EFW_SOURCES
    "${SOURCE_ROOT}/*.cpp"
    "${SOURCE_ROOT}/**/*.cpp"
)

# Exclude files with OpenCV dependencies
list(FILTER EFW_SOURCES EXCLUDE REGEX "PTC22\\.cpp$")
list(FILTER EFW_SOURCES EXCLUDE REGEX "VideoManager\\.cpp$")
list(FILTER EFW_SOURCES EXCLUDE REGEX "VideoProviderManager\\.cpp$")
list(FILTER EFW_SOURCES EXCLUDE REGEX "VideoRequesterManager\\.cpp$")

# Create executable
add_executable(EFramework ${EFW_SOURCES})

# Compiler-specific options
if(MSVC)
    target_compile_options(EFramework PRIVATE /W4)
else()
    target_compile_options(EFramework PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Source files: ${EFW_SOURCES}")
