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
