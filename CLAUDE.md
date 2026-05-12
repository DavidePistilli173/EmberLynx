# CLAUDE.md — EmberLynx Engine Coding & Project Specification

## Project Identity

Project name: EmberLynx  
Primary purpose: A modular, modern C++23 game engine.
AI assistant: This document defines how Claude Code must behave when generating, modifying, or reasoning about code in this repository.

## Repository Structure

Claude must assume the following directory layout:

/EmberLynx
│
├── bin/                # All output binaries, runtime configs, data files
├── build/              # All intermediate CMake build files
├── src/
│   ├── ember_lynx/     # Core engine (static library)
│   │   └── deps/       # Third‑party dependencies as git submodules
│   └── examples/       # Example applications using the engine
│
└── CMakeLists.txt      # Root build configuration (CMake 4+)

Rules for Claude regarding structure:

    - All engine code must live under src/ember_lynx/.
    - All example applications must live under src/examples/.
    - All third‑party libraries must be included as git submodules inside src/ember_lynx/deps/.
    - The engine builds as a static library and must statically link everything possible.

## Language & Coding Standards

Claude must strictly follow these rules for all generated code:

### Language Standard

    - C++23 is mandatory.

### Commenting

    - Use Doxygen style for all comments.

### Naming Conventions

    - Variables: snake_case
    - Functions: snake_case
    - Classes / Structs: PascalCase
    
Namespaces:

    - Engine root namespace: elx
    - All engine namespaces must be nested under elx
    - Each application must have its own distinct namespace, never elx

Private members: must end with an underscore (_)

### Const‑Correctness & Mutability

    - All variables must be declared const unless mutation is required.
    - All member functions must be const unless mutation is required.

### Constructors & Object Semantics

    - All constructors must be explicit.
    - Respect the Rule of 5.
    - Prefer composition over inheritance.

### Compile‑Time & Safety

    - Use constexpr or consteval wherever possible.
    - Use noexcept wherever possible.

### Error Handling

    - Prefer return values (e.g., std::optional, std::expected) instead of exceptions.

## CMake Rules

Claude must generate CMake code that:

    - Targets CMake 4.0+
    - Enforces C++23
    - Builds ember_lynx as a static library
    - Places all output binaries in bin/
    - Places all intermediate files in build/
    - Automatically links dependencies from src/ember_lynx/deps/
    - CMake files must be placed appropriately and must not assume global paths

## Engine Architecture Expectations

Claude must assume:

    - The engine is modular, with subsystems separated into clear components.
    - Subsystems should avoid inheritance unless absolutely necessary.
    - Public APIs must be minimal, stable, and well‑documented.

## Claude Code Behavior Requirements

Claude must:

    - Follow all rules in this document without exception.
    - Never introduce external dependencies unless explicitly instructed.
    - Prefer minimal, clean, idiomatic C++23.
    - Generate code that compiles without warnings on:
        
        - Clang
        - GCC
        - MSVC (latest)

    - Avoid unnecessary abstractions.
    - Avoid inheritance unless explicitly required.
    - Use final when a class is not intended for inheritance.    

## Formatting & Style

Claude must format code as if using:

    - One class per file
    - Header guards using #ifndef/#define
    - Use clang-format with the included .clang-format file in the repository root.

## Prohibited Behaviors

Claude must never:

    - Use exceptions for control flow
    - Use RTTI unless explicitly required
    - Use macros for anything except platform/compiler detection
    - Generate global mutable state
    - Introduce inheritance hierarchies without justification

## Allowed Libraries

Claude may use:

    - Standard C++23 library
    -Dependencies explicitly added as submodules under src/ember_lynx/deps/

No other libraries may be assumed.

## Claude Compliance

Claude must treat this file as the single source of truth for:

    - Code style
    - Architecture
    - Naming
    - Project layout
    - CMake structure
    - Error handling
    - Namespace rules

If user instructions conflict with this file, Claude must ask for clarification.