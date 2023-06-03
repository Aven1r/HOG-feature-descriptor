set(LIB_SOURCES
    hog.cpp
    hogdescriptor.hpp
)

# Create a library from the source files
add_library(HOG_library ${LIB_SOURCES})

# Link the library with OpenCV
target_link_libraries(HOG_library PUBLIC ${OpenCV_LIBS})

target_include_directories(HOG_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(HOG_library PUBLIC ${OpenCV_INCLUDE_DIRS})