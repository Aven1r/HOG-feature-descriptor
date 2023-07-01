set (HOG_LIBRARY hoglibrary)

file (GLOB SRC 
        hogdescriptor/hogdescriptor.cpp
        texvisualization/texvisualization.cpp)

add_library(${HOG_LIBRARY} ${SRC})

# Install library into the ./lib folder
install(TARGETS ${HOG_LIBRARY}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)

# Install library header files
install(DIRECTORY hogdescriptor/include/hogdescriptor
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")

install(DIRECTORY texvisualization/include/texvisualization
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")

target_link_libraries(${HOG_LIBRARY} PUBLIC ${OpenCV_LIBS})
