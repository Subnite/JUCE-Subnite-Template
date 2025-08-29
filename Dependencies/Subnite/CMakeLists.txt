target_include_directories(${PLUGIN_PROJECT_NAME} PRIVATE
 	"${CMAKE_CURRENT_SOURCE_DIR}/../" # so it's clear that Subnite/DSP is a dependency for example
)

collect_cpp_files(SUBNITE_SOURCE_FILES)

target_sources(${PLUGIN_PROJECT_NAME} PRIVATE
    ${SUBNITE_SOURCE_FILES}
)
