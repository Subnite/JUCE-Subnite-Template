if (${SHOULD_BUILD_LIBRARY})
set(JUCE_MODULES_ONLY ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(${JUCE_SOURCE_DIR} JUCE)
message("****Included JUCE at " ${JUCE_SOURCE_DIR})

if (${SHOULD_BUILD_LIBRARY})
	add_library(${PLUGIN_STATIC_LIB_NAME} STATIC
    ${JUCE_SOURCE_DIR}/modules/juce_audio_basics/juce_audio_basics.cpp
    ${JUCE_SOURCE_DIR}/modules/juce_audio_devices/juce_audio_devices.cpp
    ${JUCE_SOURCE_DIR}/modules/juce_audio_formats/juce_audio_formats.cpp
    ${JUCE_SOURCE_DIR}/modules/juce_audio_processors/juce_audio_processors.cpp
    ${JUCE_SOURCE_DIR}/modules/juce_audio_utils/juce_audio_utils.cpp
    ${JUCE_SOURCE_DIR}/modules/juce_core/juce_core.cpp
    ${JUCE_SOURCE_DIR}/modules/juce_data_structures/juce_data_structures.cpp
    ${JUCE_SOURCE_DIR}/modules/juce_dsp/juce_dsp.cpp
    ${JUCE_SOURCE_DIR}/modules/juce_events/juce_events.cpp
    ${JUCE_SOURCE_DIR}/modules/juce_graphics/juce_graphics.cpp
    ${JUCE_SOURCE_DIR}/modules/juce_gui_basics/juce_gui_basics.cpp

	# add your source files here
	PluginProcessor.cpp
	PluginEditor.cpp
)

target_include_directories(${PLUGIN_STATIC_LIB_NAME} PRIVATE
    ${JUCE_SOURCE_DIR}/modules
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
    COMPANY_NAME "Subnite Plugins"                          # Specify the name of the plugin's author
    IS_SYNTH FALSE                       # Is this a synth or an effect?
    NEEDS_MIDI_INPUT FALSE               # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE              # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                 # Is this plugin a MIDI effect?
    PLUGIN_MANUFACTURER_CODE SBNT               # A four-character manufacturer id with at least one upper-case character
	PLUGIN_CODE TMPL                            # A unique four-character plugin id with exactly one upper-case character
    FORMATS VST3                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME ${PLUGIN_VST3_NAME}        # The name of the final executable, which can differ from the target name
	DESCRIPTION "My plugin description"
    VST3_CATEGORIES "Fx" "Filter" "Pitch Shift" "Stereo"
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_COPY_DIR ${POST_BUILD_PLUGIN_COPY_DIR}
)

message("****Added juce plugin")


collect_cpp_files(PLUGIN_SOURCE_FILES)

target_sources(${PLUGIN_PROJECT_NAME}
    PRIVATE
	${PLUGIN_SOURCE_FILES}
)
message("****Added target sources")

target_compile_definitions(${PLUGIN_PROJECT_NAME}
    PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0

        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
)

message("****Added target compile definitions")

target_link_libraries(${PLUGIN_PROJECT_NAME}
    PRIVATE
        # AudioPluginData           # If we'd created a binary data target, we'd link to it here
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
message("****Linked target libraries")

message("****Modules includes at: ${JUCE_SOURCE_DIR}/modules")

target_include_directories(${PLUGIN_PROJECT_NAME}
    PUBLIC
        "${JUCE_SOURCE_DIR}/modules"
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

# if(CMAKE_CXX_COMPILER_ID NOT MATCHES "MSVC")
message("****No sign conversion etc")
target_compile_options(${PLUGIN_PROJECT_NAME}
    PRIVATE
    -w
)
# endif()

# juce_generate_juce_header(${PLUGIN_PROJECT_NAME})

# add command that copies the output to another directory
get_target_property(VST3_PATH ${PROJECT_NAME}_VST3 JUCE_PLUGIN_ARTEFACT_FILE)

if (${COPY_VST3_AFTER_BUILD})
    foreach(item ${LIST_OF_POST_BUILD_VST3_COPY_PATHS})
        message("****Added Copy Command To: ${item}")
        add_custom_command(
            TARGET ${PROJECT_NAME}_VST3 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${VST3_PATH}/Contents/x86_64-win ${item}/${PLUGIN_VST3_NAME}
        )
    endforeach()

endif()


endif() # build lib check
