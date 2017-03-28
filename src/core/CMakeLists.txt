set(LIBDSIO_SRCS
  actions.c
  allocator.c
  client.c
  connection.c
  log.c
  message.c
  mprintf.c
  topics.c)

add_library(libdsio_core SHARED ${LIBDSIO_SRCS})
set_target_properties(libdsio_core PROPERTIES OUTPUT_NAME dsio-core)

add_subdirectory(test)
