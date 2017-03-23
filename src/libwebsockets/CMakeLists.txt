set(libdsio_libwebsockets_srcs dsio-libwebsockets-impl.c)
add_library(libdsio_libwebsockets SHARED ${libdsio_libwebsockets_srcs})
target_include_directories(libdsio_libwebsockets PUBLIC ${LIBWEBSOCKETS_INCLUDE_DIR})
set_target_properties(libdsio_libwebsockets PROPERTIES OUTPUT_NAME dsio-libwebsockets)
target_link_libraries(libdsio_libwebsockets PUBLIC ${LIBWEBSOCKETS_LIBRARIES} libdsio_core)

