#include "ember_lynx/engine/entry_point.hpp"

#include <iostream>

int main() {
    auto result = elx::engine::Engine::init(elx::engine::app_config());
    if (!result) {
        std::cerr << "[EmberLynx] Engine initialisation failed: " << result.error() << '\n';
        return 1;
    }

    elx::engine::Engine& engine = elx::engine::Engine::get();
    while (!engine.window().should_close()) {
        engine.window().poll_events();
        elx::engine::app_update(engine);
    }

    elx::engine::Engine::shutdown();
    return 0;
}
