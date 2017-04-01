find_program("RAGEL" ragel)

function(ragel_gen fsm deps)
  add_custom_command(OUTPUT ${fsm}.c
    COMMAND ${RAGEL} -p -o ${fsm}.c ${fsm}.rl -I ${CMAKE_CURRENT_SOURCE_DIR} ${ARGN}
    DEPENDS ${fsm}.rl ${deps})
  add_custom_command(OUTPUT ${fsm}.dot
    COMMAND ${RAGEL} -p -V -o ${fsm}.dot ${fsm}.rl -I ${CMAKE_CURRENT_SOURCE_DIR} ${ARGN}
    DEPENDS ${fsm}.rl ${deps})
  add_custom_command(OUTPUT ${fsm}.xml
    COMMAND ${RAGEL} -x -o ${fsm}.xml ${fsm}.rl -I ${CMAKE_CURRENT_SOURCE_DIR} ${ARGN}
    DEPENDS ${fsm}.rl ${deps})
endfunction(ragel_gen)

if(RAGEL)
  ragel_gen(${CMAKE_CURRENT_SOURCE_DIR}/connection-state
    ${CMAKE_CURRENT_SOURCE_DIR}/connection-state-actions.rl)
endif()

set(LIBDSIO_SRCS
  actions.c
  allocator.c
  client.c
  connection.c
  connection-state.c
  connection-state.dot
  connection-state.xml
  error.c
  log.c
  message.c
  mprintf.c
  topics.c
)

add_library(libdsio_core SHARED ${LIBDSIO_SRCS})
set_target_properties(libdsio_core PROPERTIES OUTPUT_NAME dsio-core)

add_subdirectory(test)
