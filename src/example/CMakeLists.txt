add_executable(hogexe main.cpp)

# Link libraries
target_link_libraries(hogexe PRIVATE hoglibrary)
target_link_libraries(hogexe PRIVATE ${OpenCV_LIBS})

# Set the include directories for hoglib headers
target_include_directories(hogexe PRIVATE
    ${CMAKE_SOURCE_DIR}/src/lib/hogdescriptor/include
    ${CMAKE_SOURCE_DIR}/src/lib/texvisualization/include
)

target_compile_definitions(hogexe PRIVATE INSTALL_PATH="${CMAKE_INSTALL_PREFIX}/bin")

install(TARGETS hogexe DESTINATION ./bin)
install(FILES hogconfig.txt DESTINATION ./bin)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/images DESTINATION ./bin)
