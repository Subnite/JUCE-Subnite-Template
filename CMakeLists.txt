# General info
set(JUCE_SOURCE_DIR "/home/subnite/dev/.packages/JUCE/8.0.8") # set juce source dir (parent of "modules" folder)
set(PLUGIN_PROJECT_NAME "MyPlugin") # preferably no spaces
set(PLUGIN_VST3_NAME "MyPlugin") # name of the actual vst3 file

set(PLUGIN_STATIC_LIB_NAME ${PLUGIN_PROJECT_NAME}StaticLib) # name of the static library (must differ from plugin project name)
set(SHOULD_BUILD_LIBRARY FALSE) # IMPORTANT: if true, it will generate a static library instead of the plugin

# Post Build Params
set(COPY_VST3_AFTER_BUILD TRUE) # will copy to the paths in the list below
set(LIST_OF_POST_BUILD_VST3_COPY_PATHS
    # "C:/Program Files/Common Files/VST3"
    # "~/.vst3"
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(COPY_JUCEHEADER_AFTER_BUILD FALSE) # enables, and copies the generated juce header into the selected directory
set (JUCEHEADER_COPY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)


# ===========================================================================================
cmake_minimum_required(VERSION 3.15)
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON) # will create compile_commands.json for include paths etc
project(${PLUGIN_PROJECT_NAME} VERSION 0.1.0)

# INFO: this function finds all cpp files *RECURSIVELY* from the directory it's called from
function(collect_cpp_files out_var)
    file(GLOB_RECURSE cpp_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
    set(${out_var} ${cpp_files} PARENT_SCOPE)
endfunction()

add_subdirectory(Dependencies) # for the deps
add_subdirectory(Source) # creates the plugin, set other params in there
