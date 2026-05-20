cmake_minimum_required(VERSION 3.16)

project(EFramework LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------------------------------------------------------
# OpenCV
# -----------------------------------------------------------------------------
# Ubuntu/WSL package:
#   sudo apt install libopencv-dev pkg-config
#
# Primary method: OpenCVConfig.cmake from libopencv-dev.
# Fallback method: pkg-config opencv4.
find_package(OpenCV QUIET)

if(OpenCV_FOUND)
    message(STATUS "OpenCV found by CMake: ${OpenCV_VERSION}")
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(OPENCV4 REQUIRED opencv4)
    message(STATUS "OpenCV found by pkg-config: ${OPENCV4_VERSION}")
endif()

# -----------------------------------------------------------------------------
# Sources
# -----------------------------------------------------------------------------
file(GLOB_RECURSE EFRAMEWORK_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(eframework ${EFRAMEWORK_SOURCES})

# The code uses project-rooted includes such as <02Platform/Component/...>.
target_include_directories(eframework PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

if(OpenCV_FOUND)
    target_include_directories(eframework PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(eframework PRIVATE ${OpenCV_LIBS})
else()
    target_include_directories(eframework PRIVATE ${OPENCV4_INCLUDE_DIRS})
    target_link_directories(eframework PRIVATE ${OPENCV4_LIBRARY_DIRS})
    target_link_libraries(eframework PRIVATE ${OPENCV4_LIBRARIES})
    target_compile_options(eframework PRIVATE ${OPENCV4_CFLAGS_OTHER})
endif()

target_link_libraries(eframework PRIVATE pthread)

target_compile_options(eframework PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# Compatibility with code using _DEBUG-style conditional macros.
target_compile_definitions(eframework PRIVATE
    $<$<CONFIG:Debug>:_DEBUG=1>
)
