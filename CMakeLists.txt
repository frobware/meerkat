cmake_minimum_required(VERSION 2.8)

project(dsio)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

enable_language(C)
enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(prevent-in-source-builds)
include(use-ccache)
include(use-gold-linker)
include(CheckCCompilerFlag)
include(FindLibWebSockets)

option(ENABLE_COVERAGE "Build with code coverage" OFF)

if(ENABLE_COVERAGE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(ENABLE_ASAN "Enable Address Sanitizer on debug builds" ON)
if(ENABLE_ASAN)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src/core)

if(LIBWEBSOCKETS_FOUND)
  add_subdirectory(src/libwebsockets)
endif()

if(LIBWEBSOCKETS_FOUND)
  add_subdirectory(examples)
endif()

message("C Compiler is '${CMAKE_C_COMPILER}'")
message("CFLAGS for all types: ${CMAKE_C_FLAGS}")
message("CFLAGS for Debug: ${CMAKE_C_FLAGS_DEBUG}")
message("CFLAGS for Release: ${CMAKE_C_FLAGS_RELEASE}")
 