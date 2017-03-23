set(SRC_CORE_TESTS
  test-allocator
  test-message
  test-mprintf
)

foreach(test ${SRC_CORE_TESTS})
  add_executable(${test} ${test}.c)
  target_link_libraries(${test} libdsio_core)
  add_test(${test} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${test})
endforeach()
