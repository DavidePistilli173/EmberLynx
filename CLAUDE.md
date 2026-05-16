# EmberLynx Engine Project Overview

A modular, modern C++23 game engine using glfw and bgfx.

## Key Commands

- cmake -S . -B build -DCMAKE_BUILD_TYPE=<Debug|Release> => Configure CMake project.
- cmake --build build => Build the project.
- rm -rf build => Clean the build files.

## Key folders

- bin => All output binaries, runtime configs and data files
- build => All intermediate CMake build files
- src/ember_lynx => Core engine (static library)
- src/ember_lynx/deps => Third-party dependencies as git submodules
- src/examples => Example applications that use the engine.

## Language & Coding Standards

    - Use clean, idiomatic C++23.
    - Use Doxygen style for all comments.
    - Engine root namespace: elx
    - All engine namespaces must be nested under elx
    - Each application must have its own distinct namespace, never elx
    - Private members: must end with an underscore (_)
    - Prefer composition over inheritance.
    - Use constexpr or consteval wherever possible.
    - Prefer return values (e.g., std::optional, std::expected) instead of exceptions.
    - Header guards use #ifndef/#define
    - Avoid macros as much as possible.

### Naming Conventions

    - Variables: snake_case
    - Functions: snake_case
    - Classes / Structs: PascalCase
    