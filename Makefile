all: release

# pick whichever compiler you want for release
release: gcc-r
	cmake --build bin --config release

# pick whichever compiler you want for debug
debug: gcc-d
	cmake --build bin --config debug

msvc-r: CMakeLists.txt
	cmake -B bin -G "Visual Studio 17 2022" -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl -DCMAKE_BUILD_TYPE=Release

msvc-d: CMakeLists.txt
	cmake -B bin -G "Visual Studio 17 2022" -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl -DCMAKE_BUILD_TYPE=Debug

clang-r: CMakeLists.txt
	cmake -B bin -G "Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang -DCMAKE_BUILD_TYPE=Release

clang-d: CMakeLists.txt
	cmake -B bin -G "Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug

gcc-r: CMakeLists.txt
	cmake -B bin -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release

gcc-d: CMakeLists.txt
	cmake -B bin -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug
