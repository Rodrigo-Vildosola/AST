cmake_minimum_required(VERSION 3.10)
project(ExpressionTree CXX)

# Specify C++ standard (modern C++ style)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories relative to the build directory.
# Executables will go to .build/bin and libraries to .build/lib.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(NLOHMANN_JSON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/nlohmann/include)

# Add include directory (for public headers)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${NLOHMANN_JSON_DIR})

# Collect all source files for the static library.
file(GLOB LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/expression/*.cpp")

# Create a static library target.
add_library(expr_static STATIC ${LIB_SOURCES})
target_include_directories(expr_static PUBLIC ${CMAKE_SOURCE_DIR}/include/expression)

# Create the executable from main.cpp.
add_executable(expr_exe main.cpp)
target_link_libraries(expr_exe expr_static)
