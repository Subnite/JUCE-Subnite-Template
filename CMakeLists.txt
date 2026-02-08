# General info
# set(JUCE_SOURCE_OVERRIDE_DIR "/home/subnite/dev/.packages/JUCE/8.0.8") # set juce source dir (parent of "modules" folder), only use this if you can't find the package otherwise
set(PLUGIN_PROJECT_NAME "MyPlugin") # preferably no spaces
set(PLUGIN_VST3_NAME "MyPlugin") # name of the actual vst3 file

set(PLUGIN_STATIC_LIB_NAME ${PLUGIN_PROJECT_NAME}StaticLib) # name of the static library (must differ from plugin project name)
set(PLUGIN_DYNAMIC_LIB_NAME ${PLUGIN_PROJECT_NAME}Wrapped) # name of the dynamic library (must differ from plugin project name)
set(SHOULD_BUILD_LIBRARY FALSE) # IMPORTANT: if true, it will generate a static library instead of the plugin, and a dynamic wrapper library

# Post Build Params
set(COPY_VST3_AFTER_BUILD TRUE) # will copy to the paths in the list below
set(LIST_OF_POST_BUILD_VST3_COPY_PATHS
    # "C:/Program Files/Common Files/VST3"
    # "~/.vst3"
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# set (JUCEHEADER_COPY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source) # if defined, will generate a juce header and copy it to the provided location


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

if (DEFINED JUCE_SOURCE_OVERRIDE_DIR)
    add_subdirectory(${JUCE_SOURCE_OVERRIDE_DIR} JUCE)
    message("- [SBNT INFO] Included JUCE at " ${JUCE_SOURCE_OVERRIDE_DIR})
    set(JUCE_MODULES_PATH ${JUCE_SOURCE_OVERRIDE_DIR}/modules)
else()
    find_package(JUCE CONFIG REQUIRED)
endif()

add_subdirectory(Dependencies) # for the deps
add_subdirectory(Source) # creates the plugin, set other params in there
