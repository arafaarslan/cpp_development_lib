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

set(THIRD_PARTIES "${CMAKE_CURRENT_SOURCE_DIR}/../cpp_third_parties/")
message (STATUS "THIRD_PARTIES: ${THIRD_PARTIES}")

file(GLOB_RECURSE ARAFLIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${THIRD_PARTIES}/date/linux/include/date/tz.cpp
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

        ${THIRD_PARTIES}//asio/asio/include
        ${THIRD_PARTIES}//curl/linux/include
        ${THIRD_PARTIES}//date/linux/include/date
        ${THIRD_PARTIES}//json/linux/include/nlohmann
        ${THIRD_PARTIES}//libarchive/linux/include
        ${THIRD_PARTIES}//libsodium/linux/include
        ${THIRD_PARTIES}//libsodium/linux/include/sodium
        ${THIRD_PARTIES}//openssl/linux/include/openssl
)

set(LIBCURL_LIB ${THIRD_PARTIES}//curl/linux/lib/libcurl.so)
set(LIBARCHIVE_LIB ${THIRD_PARTIES}//libarchive/linux/lib/libarchive.a )
set(LIBSODIUM_LIB ${THIRD_PARTIES}//libsodium/linux/lib/libsodium.so.a )
set(LIBDATE_TZ ${THIRD_PARTIES}//date/linux/lib/libdate-tz.a )
set(LIBOPENSSL ${THIRD_PARTIES}//openssl/linux/lib64/libssl.a )

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