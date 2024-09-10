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
pkg_check_modules(LIBHARU REQUIRED libharu)

# Include headers
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${MARIADB_INCLUDE_DIRS}
    ${LIBHARU_INCLUDE_DIRS}
)

# Link directories
link_directories(
    ${MARIADB_LIBRARY_DIRS}
    ${LIBHARU_LIBRARY_DIRS}
)

# Set source files
file(GLOB SOURCES "src/*.c")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link executable with MariaDB and LibHaru libraries
target_link_libraries(${PROJECT_NAME}
    ${MARIADB_LIBRARIES}
    ${LIBHARU_LIBRARIES}
)

# Add compilation flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
