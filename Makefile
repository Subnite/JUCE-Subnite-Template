all: release

# pick whichever compiler you want for release
release: rust-r gcc-r
	cmake --build bin --config release

# pick whichever compiler you want for debug
debug: rust-d gcc-d
	cmake --build bin --config debug

# different compilers and optimization levels
gcc-r: CMakeLists.txt
	cmake -B bin -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release -DJUCE_MODULES_PATH=$(JUCE_MODULES_PATH)

gcc-d: CMakeLists.txt
	cmake -B bin -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug -DJUCE_MODULES_PATH=$(JUCE_MODULES_PATH)


# Rust

rust-r: Source/Rust
	cargo build --release --manifest-path ./Source/Rust/Cargo.toml

rust-d: Source/Rust
	cargo build --release --manifest-path ./Source/Rust/Cargo.toml
