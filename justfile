# Project configuration
build_dir_linux := "build-linux"
executable := "MergeTheHexes"
generator := "Ninja"
build_type := "Release"                 # Debug || Release
threads := "$(nproc)"                   # use all cores

# Main build (clean + configure + parallel build)
build:
    rm -rf {{build_dir_linux}}
    cmake -S . -B {{build_dir_linux}} -G {{generator}} -DCMAKE_BUILD_TYPE={{build_type}} -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
    cmake --build {{build_dir_linux}} --parallel -- -j {{threads}}

# Fast incremental build (no reconfigure)
compile:
    @if [ ! -d "{{build_dir_linux}}" ]; then \
        echo "Build directory not found. Running full build..."; \
        just build; \
    else \
        cmake --build {{build_dir_linux}} --parallel -- -j {{threads}}; \
    fi

# Run the executable
run:
    ./{{build_dir_linux}}/{{executable}}/{{executable}}

# Build & Run
build-run:
    just compile && just run

# Debug build (with debug symbols)
debug:
    rm -rf debug_build
    cmake -S . -B debug_build -G {{generator}} -DCMAKE_BUILD_TYPE=Debug
    cmake --build debug_build --parallel -- -j {{threads}}

# Run debug version
debug-run:
    ./debug_build/{{executable}}

# Configure only (useful for changing CMake options)
configure:
    cmake -S . -B {{build_dir_linux}} -G {{generator}} -DCMAKE_BUILD_TYPE={{build_type}}

# Clean build directory
clean:
    rm -rf {{build_dir_linux}}

# Show build configuration
info:
    @echo "Build Directory: {{build_dir_linux}}"
    @echo "Executable: {{executable}}"
    @echo "Generator: {{generator}}"
    @echo "Build Type: {{build_type}}"
    @echo "Threads: {{threads}}"
    @if [ -d "{{build_dir_linux}}" ]; then \
        echo "\nCMake Cache:"; \
        grep CMAKE_BUILD_TYPE {{build_dir_linux}}/CMakeCache.txt 2>/dev/null || echo "Not configured"; \
    fi

# Build and Build-Run with ccache enabled (im not gonna use these. i just wrote them for Debug!)

build-ccache:
    rm -rf {{build_dir_linux}}
    cmake -S . -B {{build_dir_linux}} -G {{generator}} -DCMAKE_BUILD_TYPE={{build_type}} -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
    cmake --build {{build_dir_linux}} --parallel -- -j {{threads}}

build-run-ccache:
    rm -rf {{build_dir_linux}}
    cmake -S . -B {{build_dir_linux}} -G {{generator}} -DCMAKE_BUILD_TYPE={{build_type}} -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
    cmake --build {{build_dir_linux}} --parallel -- -j {{threads}}
    just run