if (SAMPLE)
    add_subdirectory(example)
endif()

add_subdirectory(lib)

if (TESTING)
    add_subdirectory(tests)
endif()