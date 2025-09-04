
if (SHOULD_BUILD_LIBRARY)
set(JUCE_MODULES_ONLY ON CACHE BOOL "" FORCE)
endif()

collect_cpp_files(PLUGIN_SOURCE_FILES)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(PLATFORM_STRING "x86_64-win")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PLATFORM_STRING "x86_64-linux")
else()
    message(FATAL_ERROR "- [SBNT ERROR] Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

if (${SHOULD_BUILD_LIBRARY})
add_library(${PLUGIN_STATIC_LIB_NAME} STATIC
    ${JUCE_MODULES_PATH}/juce_audio_basics/juce_audio_basics.cpp
    ${JUCE_MODULES_PATH}/juce_audio_devices/juce_audio_devices.cpp
    ${JUCE_MODULES_PATH}/juce_audio_formats/juce_audio_formats.cpp
    ${JUCE_MODULES_PATH}/juce_audio_processors/juce_audio_processors.cpp
    ${JUCE_MODULES_PATH}/juce_audio_utils/juce_audio_utils.cpp
    ${JUCE_MODULES_PATH}/juce_core/juce_core.cpp
    ${JUCE_MODULES_PATH}/juce_data_structures/juce_data_structures.cpp
    ${JUCE_MODULES_PATH}/juce_dsp/juce_dsp.cpp
    ${JUCE_MODULES_PATH}/juce_events/juce_events.cpp
    ${JUCE_MODULES_PATH}/juce_graphics/juce_graphics.cpp
    ${JUCE_MODULES_PATH}/juce_gui_basics/juce_gui_basics.cpp

    ${PLUGIN_SOURCE_FILES}
)

add_dependencies(${PLUGIN_STATIC_LIB_NAME} SubniteExtras)

target_include_directories(${PLUGIN_STATIC_LIB_NAME} PRIVATE
    ${JUCE_MODULES_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Dependencies
)

target_compile_options(${PLUGIN_STATIC_LIB_NAME}  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/Gy # MSVC Enable function-level linking
    $<$<CXX_COMPILER_ID:MSVC>:/bigobj> # enables big objects
    # $<$<CXX_COMPILER_ID:MSVC>:/fsanitize=address # to find memory leaks
)

target_compile_definitions(${PLUGIN_STATIC_LIB_NAME}
    PRIVATE
        JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
        JUCE_GUI_BASICS_INCLUDE_CORE_HEADERS=1
        JUCE_MODAL_LOOPS_PERMITTED=1
)

# if (NOT ${SHOULD_BUILD_LIBRARY})
else()

juce_add_plugin(${PLUGIN_PROJECT_NAME}
    COMPANY_NAME "Subnite Plugins"       # Specify the name of the plugin's author
    IS_SYNTH FALSE                       # Is this a synth or an effect?
    NEEDS_MIDI_INPUT FALSE               # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE              # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                 # Is this plugin a MIDI effect?
    PLUGIN_MANUFACTURER_CODE SBNT        # A four-character manufacturer id with at least one upper-case character
	PLUGIN_CODE Tmpl                     # A unique four-character plugin id with exactly one upper-case character
    FORMATS VST3                         # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME ${PLUGIN_VST3_NAME}     # The name of the final executable, which can differ from the target name
	DESCRIPTION "My plugin description"
    VST3_CATEGORIES "Fx" "Filter" "Pitch Shift" "Stereo"
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_COPY_DIR ${POST_BUILD_PLUGIN_COPY_DIR}
)

add_dependencies(${PLUGIN_PROJECT_NAME} SubniteExtras)
message("- [SBNT INFO] Added juce plugin")


target_sources(${PLUGIN_PROJECT_NAME}
    PRIVATE
        ${PLUGIN_SOURCE_FILES}
)
message("- [SBNT INFO] Added target sources")

target_compile_definitions(${PLUGIN_PROJECT_NAME}
    PUBLIC
        # JUCE_DISPLAY_SPLASH_SCREEN=0 # for either older versions (8.0.8) or non linux (not sure)

        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
)

message("- [SBNT INFO] Added target compile definitions")

target_link_libraries(${PLUGIN_PROJECT_NAME}
    PRIVATE
        juce::juce_audio_plugin_client
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra

        SubniteExtras # some extra's like a better value tree (with saving set up)
    PUBLIC
        juce::juce_recommended_config_flags
        $<$<PLATFORM_ID:Windows>:juce::juce_recommended_lto_flags> # problems with gcc on linux...
        juce::juce_recommended_warning_flags
)
message("- [SBNT INFO] Linked target libraries")

message("- [SBNT INFO] Modules includes at: ${JUCE_MODULES_PATH}")

target_include_directories(${PLUGIN_PROJECT_NAME}
    PUBLIC
        "${JUCE_MODULES_PATH}"
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_SOURCE_DIR}/Dependencies"
)

target_compile_options (${PLUGIN_PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-fdiagnostics-color=always>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Werror> # treat warnings as errors.
    $<$<CXX_COMPILER_ID:MSVC>:/WX> # warning = error
)
set(CMAKE_COLOR_DIAGNOSTICS ON)


# add command that copies the output to another directory
get_target_property(VST3_PATH ${PROJECT_NAME}_VST3 JUCE_PLUGIN_ARTEFACT_FILE)


if (${COPY_VST3_AFTER_BUILD})
    foreach(item ${LIST_OF_POST_BUILD_VST3_COPY_PATHS})
        message("- [SBNT INFO] Added Copy Command To: ${item}")
        add_custom_command(
            TARGET ${PROJECT_NAME}_VST3 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${VST3_PATH} ${item}/${PLUGIN_VST3_NAME}.vst3
        )
    endforeach()

endif()

if (DEFINED JUCEHEADER_COPY_DIR)
    juce_generate_juce_header(${PLUGIN_PROJECT_NAME})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_PROJECT_NAME}_artefacts/JuceLibraryCode/JuceHeader.h ${JUCEHEADER_COPY_DIR}
        COMMENT "- [SBNT INFO] Copying JuceHeader.h to: ${JUCEHEADER_COPY_DIR}"
    )
endif()


endif() # build lib check
