all: release

# pick whichever compiler you want for release
release: gcc-r
	cmake --build bin/linux --config release

# pick whichever compiler you want for debug
debug: gcc-d
	cmake --build bin/linux --config debug

# different compilers and optimization levels
# JUCE_MODULES_PATH should be in the environment (to dir juce-version/modules), especially if you aren't overriding the JUCE_SOURCE_OVERRIDE_DIR (to specify where juce is located)
gcc-r: CMakeLists.txt
	cmake -B bin/linux -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release -DJUCE_MODULES_PATH=$(JUCE_MODULES_PATH)

gcc-d: CMakeLists.txt
	cmake -B bin/linux -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug -DJUCE_MODULES_PATH=$(JUCE_MODULES_PATH)
