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
include(ragel-gen)

option(ENABLE_COVERAGE "Build with code coverage" ON)

if(ENABLE_COVERAGE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wno-padded -Wno-unused-parameter")
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wno-unused-parameter")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(ENABLE_ASAN "Enable Address Sanitizer on debug builds" OFF)

if(ENABLE_ASAN)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

find_program("RAGEL" ragel)

if(RAGEL)
  ragel_gen(${CMAKE_SOURCE_DIR}/src/core/state
    ${CMAKE_SOURCE_DIR}/src/core/state-actions.rl)
endif()

set(LIBDSIO_SRCS
  src/core/actions.c
  src/core/allocator.c
  src/core/bb.c
  src/core/client.c
  src/core/connection.c
  src/core/error.c
  src/core/log.c
  src/core/message.c
  src/core/mprintf.c
  src/core/state.c
  src/core/state.dot
  src/core/state.xml
  src/core/topics.c
)

add_library(libdsio_core SHARED ${LIBDSIO_SRCS})
set_target_properties(libdsio_core PROPERTIES OUTPUT_NAME dsio-core)

set(SRC_CORE_TESTS
  test-allocator
  test-message
  test-mprintf
  test-actions
)

foreach(test ${SRC_CORE_TESTS})
  add_executable(${test} src/core/test/${test}.c)
  target_link_libraries(${test} libdsio_core)
  add_test(${test} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${test})
endforeach()

if(LIBWEBSOCKETS_FOUND)
  set(libdsio_libwebsockets_srcs src/libwebsockets/dsio-libwebsockets-impl.c)
  add_library(libdsio_libwebsockets SHARED ${libdsio_libwebsockets_srcs})
  target_include_directories(libdsio_libwebsockets PUBLIC ${LIBWEBSOCKETS_INCLUDE_DIR})
  set_target_properties(libdsio_libwebsockets PROPERTIES OUTPUT_NAME dsio-libwebsockets)
  target_link_libraries(libdsio_libwebsockets PUBLIC ${LIBWEBSOCKETS_LIBRARIES} libdsio_core)

  add_executable(login examples/login.c)
  target_link_libraries(login PUBLIC libdsio_libwebsockets)
endif()

message("C Compiler is '${CMAKE_C_COMPILER}'")
message("CFLAGS for all types: ${CMAKE_C_FLAGS}")
message("CFLAGS for Debug: ${CMAKE_C_FLAGS_DEBUG}")
message("CFLAGS for Release: ${CMAKE_C_FLAGS_RELEASE}")
