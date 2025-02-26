cmake_minimum_required(VERSION 3.10)
project(ExpressionTree CXX)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(NLOHMANN_JSON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/nlohmann/include)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${NLOHMANN_JSON_DIR})

# Collect source files
file(GLOB LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/*/*.cpp")

# Create a static library
add_library(expr_static STATIC ${LIB_SOURCES})
target_include_directories(expr_static PUBLIC ${CMAKE_SOURCE_DIR}/include/expression)

# Create the executable
add_executable(expr_exe main.cpp)
target_link_libraries(expr_exe expr_static)

# Enable debugging flags when in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building with AddressSanitizer and UndefinedBehaviorSanitizer")
    target_compile_options(expr_exe PRIVATE -fsanitize=address,undefined -g -O1)
    target_link_options(expr_exe PRIVATE -fsanitize=address,undefined)
endif()
