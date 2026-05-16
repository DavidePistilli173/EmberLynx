#include "ember_lynx/engine/App.hpp"

#include <iostream>
#include <print>

int main() {
    std::unique_ptr<elx::engine::App> app = elx::engine::create_app();

    auto result = elx::engine::Engine::init(app->config());
    if (!result) {
        std::cerr << "[EmberLynx] Engine initialisation failed: " << result.error() << '\n';
        return 1;
    }

    elx::engine::Engine& engine = elx::engine::Engine::get();
    uint64_t             counter{ 0U };
    while (!engine.window().should_close()) {
        engine.window().poll_events();
        app->on_update(engine);
        engine.window().present();
        std::println("Running...{}", counter++);
    }

    elx::engine::Engine::shutdown();
    return 0;
}
