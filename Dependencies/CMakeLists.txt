add_subdirectory(subnite_extras)

if (${SHOULD_BUILD_LIBRARY})
    add_subdirectory(dyn_lib)
endif()
