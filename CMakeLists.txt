cmake_minimum_required(VERSION 3.12)

project(CHPatterns
    VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

configure_file(VersionInfo.h.in VersionInfo.h)

add_executable(CHPatterns chpatterns.cxx utils.cxx)

target_include_directories(CHPatterns PUBLIC
    "${PROJECT_BINARY_DIR}"
    )
