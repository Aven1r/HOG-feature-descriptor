set (HOG_LIBRARY hoglibrary)

file (GLOB SRC 
        hogdescriptor/hogdescriptor.cpp
        texvisualization/texvisualization.cpp)


add_library(${HOG_LIBRARY} ${SRC})

install(TARGETS ${HOG_LIBRARY}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)

install(DIRECTORY hogdescriptor/include/hogdescriptor
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")

install(DIRECTORY texvisualization/include/texvisualization
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")

configure_file(../../cmake/hoglibraryConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/hoglibraryConfig.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/hoglibraryConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hoglibrary)

target_link_libraries(${HOG_LIBRARY} PUBLIC ${OpenCV_LIBS})
