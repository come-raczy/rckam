
set(GRESOURCE_BUNDLE "${CMAKE_CURRENT_SOURCE_DIR}/gresource.c" )
set(GRESOURCE_BUNDLE_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/gresource.c" PARENT_SCOPE)
set(GRESOURCE_BUNDLE_OBJECT  "${CMAKE_CURRENT_BINARY_DIR}/gresource.o" PARENT_SCOPE)

message(STATUS GRESOURCE_BUNDLE_OBJECT: ${GRESOURCE_BUNDLE_OBJECT})

set(GRESOURCE_BUNDLE_TARGET "RCKAM_CLIENT_RESOURCES_GRESOURCE_C")
set(GRESOURCE_XML "rckam.gresource.xml")

message(STATUS "=============== execute_process ")

execute_process(WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMAND "${GLIB_COMPILE_RESOURCES} --generate-dependencies ${GRESOURCE_XML} | tr '\n' ' '"
                OUTPUT_VARIABLE GRESOURCE_DEPENDENCIES)
message(STATUS "=============== add_custom_command OUTPUT ${GRESOURCE_BUNDLE}")

add_custom_command(OUTPUT ${GRESOURCE_BUNDLE}
    COMMAND ${GLIB_COMPILE_RESOURCES} --generate-source --target=${GRESOURCE_BUNDLE} ${GRESOURCE_XML}
    #COMMAND ${GLIB_COMPILE_RESOURCES} --generate-source --target=${RESOURCES_BUNDLE} --dependency-file=$(RESOURCES_DEPS) --generate-phony-targets ${RESOURCES_XML}
    #COMMAND echo "This is ALL target 'zoo', and it depends on ${TEST_FILE}"
    # If the file exists, then commands related to that file won't be executed
    # DONOT let other target depends on the same OUTPUT as current target,
    #   or it may be bad when doing parallel make
    DEPENDS ${GRESOURCE_XML} ${GRESOURCE_DEPENDENCIES}

    # to make quotes printable,for example
    #VERBATIM
)
message(STATUS "===============")

add_custom_target(${GRESOURCE_BUNDLE_TARGET} DEPENDS ${GRESOURCE_BUNDLE})

#set_source_files_properties(${GRESOURCE_BUNDLE} PROPERTIES COMPILE_FLAGS ${GRESOURCE_BUNDLE_COMPILE_FLAGS})
