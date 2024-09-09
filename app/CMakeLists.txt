# Set the cmake minimum version
cmake_minimum_required(VERSION 3.5)

# Define the project name
project(app)

# Generate symbols for IDE indexer
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to C11
set(CMAKE_C_STANDARD 11)

# Add the libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(MARIADB REQUIRED mariadb)

# Include MariaDB headers
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${MARIADB_INCLUDE_DIRS}
)
link_directories(${MARIADB_LIBRARY_DIRS})

# Set source files
file(GLOB SOURCES "src/*.c")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link executable with MariaDB library
target_link_libraries(${PROJECT_NAME} ${MARIADB_LIBRARIES})

# Add compilation flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

