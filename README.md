# JUCE Subnite Template

## User Documentation
### Installation
to install you can just put the VST3 file in either your own custom directory, or the default directory at
`Program Files/Common Files/VST3/` on windows
`$HOME/.vst3/` on linux

## For Developers
### Building
First of all, check the top layer CMakeLists.txt and personalize it to your needs, then definitely check the Source folder CMakeLists.txt for plugin specific configuration.

#### Command line:
run `make` to build the makefile (you can edit to use a difference compiler)
alternatively on windows, run `build.bat msvc-r` for msvc release mode.
> if you're using nix, you can run `nix build` to get juce up and running, then `nix develop` to get inside a shell with all dependencies ready for you to build with `make`

### Things that might go wrong
Make sure to check out all the options in the top layer of the CMakeLists.txt and also the one in Source, since that one is responsible for the plugin creation
1. Set the path to JUCE correctly (if `find_package` fails)
2. Try turning off the post build copying if the final step fails
3. Delete the `bin` directory since cmake doesn't like it sometimes :/
4. If you're trying to use the juce header, then make sure it is defined in the top layer cmake file

### Build flow
If you want to change the whole configuration, here is the general build flow
1. build the rust files that the plugin might depend on
2. include / build JUCE itself
3. build the plugin
