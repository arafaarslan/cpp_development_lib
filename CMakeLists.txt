cmake_minimum_required(VERSION 3.10)
project(araflib VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PLATFORM "?" CACHE STRING "Which Platform")
message(STATUS "Selected platform: ${PLATFORM}")
message(STATUS "Selected Built Type : ${CMAKE_BUILD_TYPE}")
message(STATUS "Selected CMAKE_SYSTEM_NAME : ${CMAKE_SYSTEM_NAME}")
message(STATUS "Selected CMAKE_GENERATOR : ${CMAKE_GENERATOR}")

# ----------------------------------------------------------
# Collect all source files
# ----------------------------------------------------------
file(GLOB_RECURSE ARAFLIB_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

message(STATUS "ARAF_LIB SOURCES: ${ARAFLIB_SOURCES}")
message (STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

# ----------------------------------------------------------
# Create static library
# ----------------------------------------------------------
add_library(${PROJECT_NAME} STATIC ${ARAFLIB_SOURCES})

# Public include paths (headers visible to users)
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/3rd_parties/asio/asio/include>
        $<INSTALL_INTERFACE:include>
)

# C++11 required
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

# Debug flags
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:-O0 -g>
)

# -----------------------------
# Libraries
# -----------------------------

find_package(OpenSSL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
)

# ----------------------------------------------------------
# Add tests
# ----------------------------------------------------------
# add_subdirectory(test)