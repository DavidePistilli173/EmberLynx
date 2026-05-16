#ifndef ELX_ENGINE_ENGINE_HPP
#define ELX_ENGINE_ENGINE_HPP

#include "ember_lynx/ui/Window.hpp"

#include <expected>
#include <memory>
#include <string>

namespace elx::engine {

    /// @brief Central engine singleton that owns all engine resources.
    ///
    /// Call Engine::init() once at startup, then access the instance from
    /// anywhere via Engine::get().  Call Engine::shutdown() to release all
    /// resources before the process exits.
    class Engine {
     public:
        /// @brief Top-level initialisation parameters.
        struct Config {
            elx::ui::Window::Config window{};
        };

        Engine(Engine const&)            = delete;
        Engine& operator=(Engine const&) = delete;
        Engine(Engine&&)                 = delete;
        Engine& operator=(Engine&&)      = delete;

        ~Engine() = default;

        /// @brief Initialise the Engine singleton.
        ///
        /// Fails if the Engine has already been initialised or if any
        /// sub-system fails to start.
        /// @return An error string on failure.
        [[nodiscard]] static std::expected<void, std::string> init(Config const& config) noexcept;

        /// @brief Retrieve the Engine singleton.
        /// @pre init() must have been called successfully.
        [[nodiscard]] static Engine& get() noexcept;

        /// @brief Destroy the Engine singleton and release all resources.
        static void shutdown() noexcept;

        /// @brief Returns true if the singleton has been successfully initialised.
        [[nodiscard]] static bool is_initialised() noexcept;

        [[nodiscard]] elx::ui::Window&       window() noexcept;
        [[nodiscard]] elx::ui::Window const& window() const noexcept;

     private:
        explicit Engine(elx::ui::Window window) noexcept;

        static std::unique_ptr<Engine> s_instance_;

        elx::ui::Window window_;
    };

} // namespace elx::engine

#endif // ELX_ENGINE_ENGINE_HPP
