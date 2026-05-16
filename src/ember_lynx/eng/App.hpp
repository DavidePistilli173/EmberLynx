#ifndef ELX_ENG_APP_HPP
#define ELX_ENG_APP_HPP

#include "ember_lynx/eng/Engine.hpp"

#include <chrono>
#include <memory>

namespace elx::eng {

    /**
     * @brief Base class for user applications.
     *
     * Derive from App, override on_update() and optionally config(), then
     * implement create_app() to return your derived instance.  The engine
     * owns the lifetime of the App object.
     */
    class App {
     public:
        App(App const&)            = delete;
        App& operator=(App const&) = delete;
        App(App&&)                 = delete;
        App& operator=(App&&)      = delete;

        virtual ~App() = default;

        /**
         * @brief Override to supply custom Engine configuration before startup.
         *
         * The default returns a zero-initialised Config (800x600, title "EmberLynx").
         */
        [[nodiscard]] virtual Engine::Config config() const noexcept;

        /** @brief Called once per frame inside the engine main loop. */
        virtual void on_update(std::chrono::nanoseconds delta_time) noexcept = 0;

     protected:
        App() = default;
    };

    /** @brief User-provided: construct and return the application instance. */
    [[nodiscard]] std::unique_ptr<App> create_app() noexcept;

} // namespace elx::eng

#endif // ELX_ENG_APP_HPP
