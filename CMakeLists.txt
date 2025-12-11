cmake_minimum_required(VERSION 3.10)
project(araflib VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PLATFORM "?" CACHE STRING "Which Platform")
message(STATUS "Selected platform: ${PLATFORM}")
message(STATUS "Selected Built Type : ${CMAKE_BUILD_TYPE}")
message(STATUS "Selected CMAKE_SYSTEM_NAME : ${CMAKE_SYSTEM_NAME}")
message(STATUS "Selected CMAKE_GENERATOR : ${CMAKE_GENERATOR}")
message (STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

# ----------------------------------------------------------
# Collect all source files
# ----------------------------------------------------------
file(GLOB_RECURSE ARAFLIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd_parties/date/linux/include/date//tz.cpp
)
message(STATUS "ARAF_LIB SOURCES: ${ARAFLIB_SOURCES}")


# ----------------------------------------------------------
# Create static library
# ----------------------------------------------------------
add_library(
    ${PROJECT_NAME} STATIC 
    ${ARAFLIB_SOURCES}
    # ${ARAF_LIB_ROOT}/3rd_parties/date/src/tz.cpp
)


# ----------------------------------------------------------
# Include directories
# (Headers exported to users of araflib)
# ----------------------------------------------------------
find_package(OpenSSL REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>

        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_parties/asio/asio/include
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_parties/curl/linux/include/curl
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_parties/date/linux/include/date
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_parties/json/linux/include/nlohmann
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_parties/libarchive/linux/include
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_parties/libsodium/linux/include
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_parties/libsodium/linux/include/sodium
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_parties/openssl/linux/include/openssl
)

set(LIBCURL_LIB ${CMAKE_CURRENT_SOURCE_DIR}/3rd_parties/curl/linux/lib/libcurl.so)
set(LIBARCHIVE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/3rd_parties/libarchive/linux/lib/libarchive.a )
set(LIBSODIUM_LIB ${CMAKE_CURRENT_SOURCE_DIR}/3rd_parties/libsodium/linux/lib/libsodium.so.a )
set(LIBDATE_TZ ${CMAKE_CURRENT_SOURCE_DIR}/3rd_parties/date/linux/lib/libdate-tz.a )
set(LIBOPENSSL ${CMAKE_CURRENT_SOURCE_DIR}/3rd_parties/openssl/linux/lib64/libssl.a )

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${LIBCURL_LIB}
    ${LIBSODIUM_LIB}
    ${LIBARCHIVE_LIB}
    ${LIBDATE_TZ}
    ${LIBOPENSSL}
    pthread
)

# -----------------------------
# Libraries
# -----------------------------


# ----------------------------------------------------------
# Add tests
# ----------------------------------------------------------
# add_subdirectory(test)