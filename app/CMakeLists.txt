# Set the cmake minimum version
cmake_minimum_required(VERSION 3.5)

# Define the project name
project(app)

# Generate symbols for IDE indexer
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to C11
set(CMAKE_C_STANDARD 11)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(MARIADB REQUIRED mariadb)

# CMake instructions to make the static lib
add_library(pdfgen STATIC
    ${CMAKE_SOURCE_DIR}/libs/pdfgen.c
)

# Include headers
target_include_directories(pdfgen PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/libs
    ${MARIADB_INCLUDE_DIRS}
)

# Link directories (not always necessary in modern CMake)
link_directories(${MARIADB_LIBRARY_DIRS})

# Set source files
file(GLOB SOURCES "src/*.c")

# Create executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
)

# Link executable with MariaDB, pdfgen, and math library (m)
target_link_libraries(${PROJECT_NAME}
    ${MARIADB_LIBRARIES}
    pdfgen
    m  # math.h
)

# Add compilation flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
