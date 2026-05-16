#include "ember_lynx/engine/entry_point.hpp"

namespace hello_world {

    class App : public elx::engine::App {
     public:
        [[nodiscard]] elx::engine::Engine::Config config() const noexcept override {
            return { .window = { .title = "Hello World" } };
        }

        void on_update(elx::engine::Engine& /*engine*/) noexcept override {}
    };

} // namespace hello_world

std::unique_ptr<elx::engine::App> elx::engine::create_app() noexcept {
    return std::make_unique<hello_world::App>();
}
