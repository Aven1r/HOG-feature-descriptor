# If sample option is on, cmake will make an executable.
if (SAMPLE)
    add_subdirectory(example)
endif()

add_subdirectory(lib)
