# set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-export-all-symbols")
set(CMAKE_SHARED_LINKER_FLAGS "-W1")

add_library(${PLUGIN_DYNAMIC_LIB_NAME} SHARED # SHARED or STATIC
    src/private/juce-init.cpp
    src/private/TParameter.cpp
    src/private/aura-params.cpp
    src/private/plugin.cpp
)

add_dependencies(${PLUGIN_DYNAMIC_LIB_NAME} ${PLUGIN_STATIC_LIB_NAME})

target_include_directories(${PLUGIN_DYNAMIC_LIB_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private
    ${CMAKE_CURRENT_SOURCE_DIR}/src/public
    # for the plugin
    ${CMAKE_SOURCE_DIR}/Source
    ${CMAKE_SOURCE_DIR}/Dependencies/
    # JUCE itself
    ${JUCE_MODULES_PATH}
)

# target_link_directories(${PLUGIN_DYNAMIC_LIB_NAME} PRIVATE
#     ${AURA_LIB_DIR}/$<CONFIG>/
# )

target_link_libraries(${PLUGIN_DYNAMIC_LIB_NAME} PRIVATE
    "$<LINK_LIBRARY:WHOLE_ARCHIVE,${PLUGIN_STATIC_LIB_NAME}>"
)

target_compile_definitions(${PLUGIN_DYNAMIC_LIB_NAME} PRIVATE
    SOURCE_EXPORT_SHARED_LIB=1
    JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
    JUCE_MODAL_LOOPS_PERMITTED=1
)

# target_compile_options(${PLUGIN_DYNAMIC_LIB_NAME} PRIVATE
#     "/fsanitize=address"
# )

# add_custom_command(
#     TARGET ${PLUGIN_DYNAMIC_LIB_NAME}  # Replace with your target name
#     POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy
#             $<TARGET_FILE:${PLUGIN_DYNAMIC_LIB_NAME}>
#             ${EXPORT_ALL_PATH}
#     COMMENT "Copying source plugin output to ${EXPORT_ALL_PATH}"
# )
