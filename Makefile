all: release

release: gcc-r
	cmake --build build --config release

debug: gcc-d
	cmake --build build --config debug

msvc-r: CMakeLists.txt
	cmake -B build -G "Visual Studio 17 2022" -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl -DCMAKE_BUILD_TYPE=Release

msvc-d: CMakeLists.txt
	cmake -B build -G "Visual Studio 17 2022" -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl -DCMAKE_BUILD_TYPE=Debug

clang-r: CMakeLists.txt
	cmake -B build -G "Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang -DCMAKE_BUILD_TYPE=Release

clang-d: CMakeLists.txt
	cmake -B build -G "Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug

gcc-r: CMakeLists.txt
	cmake -B build -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release

gcc-d: CMakeLists.txt
	cmake -B build -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug
