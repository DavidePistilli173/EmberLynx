#include "ember_lynx/eng/App.hpp"

#include <chrono>
#include <iostream>
#include <print>

int main() {
    std::unique_ptr<elx::eng::App> app = elx::eng::create_app();

    auto result = elx::eng::Engine::init(app->config());
    if (!result) {
        std::cerr << "[EmberLynx] Engine initialisation failed: " << result.error() << '\n';
        return 1;
    }

    elx::eng::Engine& engine = elx::eng::Engine::get();

    using Clock    = std::chrono::steady_clock;
    auto prev_time = Clock::now();
    auto delta     = std::chrono::nanoseconds{ 0 };

    while (!engine.window().should_close()) {
        engine.window().poll_events();
        app->on_update(delta);
        engine.window().present();

        auto now  = Clock::now();
        delta     = now - prev_time;
        prev_time = now;
    }

    elx::eng::Engine::shutdown();
    return 0;
}
