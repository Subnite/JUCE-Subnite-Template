# should include the dependencies directory instead so the "Subnite" folder is included in your #include's

collect_cpp_files(SUBNITE_SOURCE_FILES)

add_library(SubniteExtras STATIC
    ${SUBNITE_SOURCE_FILES}
)

target_link_libraries(SubniteExtras
    PRIVATE
        # juce::juce_audio_plugin_client
        juce::juce_audio_basics
        # juce::juce_audio_devices
        # juce::juce_audio_formats
        # juce::juce_audio_processors
        # juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        # juce::juce_dsp
        # juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        # juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        $<$<PLATFORM_ID:Windows>:juce::juce_recommended_lto_flags> # problems with gcc on linux...
        juce::juce_recommended_warning_flags
)

target_compile_options(SubniteExtras PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Werror> # treat warnings as errors.
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
)

target_include_directories(SubniteExtras PRIVATE
    "${JUCE_MODULES_PATH}"
)

