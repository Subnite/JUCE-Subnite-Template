# JUCE Subnite Template

## User Documentation
### Installation
to install you can just put the VST3 file in either your own custom directory, or the default directory at
`Program Files/Common Files/VST3`

## For Developers
### Building
First of all, set the JUCE path correctly in the top layer CMakeLists.txt and change some of the other flags while you're at it.

#### command line:
run "make" to build the makefile (you can edit to use a difference compiler)

### Things that might go wrong
Make sure to check out all the options in the top layer of the CMakeLists.txt and also the one in Source, since that one is responsible for the plugin creation
1. Set the path to JUCE correctly
2. Try turning off the post build copying if it fails
3. Delete the Builds directory since cmake doesn't like it :/

