if (SAMPLES)
    add_subdirectory(example)
endif()

add_subdirectory(lib)

if (TESTING)
    add_subdirectory(tests)
endif()