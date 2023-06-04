# add_executable(main main.cpp)

# # Set include directories for the executable
# target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
# target_include_directories(main PRIVATE ${OpenCV_INCLUDE_DIRS})

# # Link the executable with YourLibraryName and OpenCV
# target_link_libraries(main PRIVATE HOG_LIBRARY ${OpenCV_LIBS})
add_executable(example
    main.cpp
)

# Link libraries
target_link_libraries(example PRIVATE hogdescriptor texvisualization)
target_link_libraries(example PRIVATE ${OpenCV_LIBS})