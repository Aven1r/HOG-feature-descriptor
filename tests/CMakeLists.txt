# Add your test source files in the test directory
add_executable(hogtest hogtest.cpp)

# Set include directories for the test executable
target_include_directories(hogtest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

# Link the test executable with YourLibraryName and OpenCV
target_link_libraries(hogtest PRIVATE HOG_library ${OpenCV_LIBS} doctest::doctest)
