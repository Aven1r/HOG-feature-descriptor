add_executable(hoglib
    main.cpp
)

# Link libraries
target_link_libraries(hoglib PRIVATE hogdescriptor texvisualization)
target_link_libraries(hoglib PRIVATE ${OpenCV_LIBS})

target_compile_definitions(hoglib PRIVATE INSTALL_PATH="${CMAKE_INSTALL_PREFIX}/bin")

install(TARGETS hoglib DESTINATION bin)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/images DESTINATION ./bin)


