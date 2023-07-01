find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(hoglib_DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/Doxyfile.in)
    set(hoglib_DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)
    configure_file(${hoglib_DOXYGEN_IN} ${hoglib_DOXYGEN_OUT})
    add_custom_target(hoglib_doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${hoglib_DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
    install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/hoglib_doc
        DESTINATION ./docs)
else ()
    message(STATUS "Couldn't find Doxygen")
endif ()
