#include "ember_lynx/eng/Engine.hpp"

#include <cassert>

namespace elx::eng {

    std::unique_ptr<Engine> Engine::s_instance_{ nullptr };

    Engine::Engine(elx::ui::Window window) noexcept : window_{ std::move(window) } {}

    std::expected<void, std::string> Engine::init(Config const& config) noexcept {
        if (s_instance_ != nullptr) {
            return std::unexpected{ "Engine is already initialised" };
        }

        auto window = elx::ui::Window::create(config.window);
        if (!window) {
            return std::unexpected{ std::move(window.error()) };
        }

        s_instance_ = std::unique_ptr<Engine>{ new Engine{ std::move(*window) } };
        return {};
    }

    Engine& Engine::get() noexcept {
        assert(s_instance_ != nullptr && "Engine::get() called before Engine::init()");
        return *s_instance_;
    }

    void Engine::shutdown() noexcept {
        s_instance_.reset();
    }

    bool Engine::is_initialised() noexcept {
        return s_instance_ != nullptr;
    }

    elx::ui::Window& Engine::window() noexcept {
        return window_;
    }

    elx::ui::Window const& Engine::window() const noexcept {
        return window_;
    }

} // namespace elx::eng
