add_library(ember_lynx STATIC)

add_subdirectory(deps)

target_sources(ember_lynx PRIVATE
    engine/Engine.cpp
    engine/entry_point.cpp
    ui/Window.cpp
)

target_include_directories(ember_lynx PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(ember_lynx PUBLIC
    glfw
)
