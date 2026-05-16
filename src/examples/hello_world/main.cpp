#include "ember_lynx/eng/entry_point.hpp"

namespace hello_world {

    class App : public elx::eng::App {
     public:
        [[nodiscard]] elx::eng::Engine::Config config() const noexcept override {
            return { .window = { .title = "Hello World" }, .logger = { .log_file_path = "hello_world.log" } };
        }

        void on_update(std::chrono::nanoseconds delta_time) noexcept override {
            elx::trace("Hello World running! Delta time: '{}'", delta_time);
        }
    };

} // namespace hello_world

std::unique_ptr<elx::eng::App> elx::eng::create_app() noexcept {
    return std::make_unique<hello_world::App>();
}
