#ifndef ELX_UI_WINDOW_HPP
#define ELX_UI_WINDOW_HPP

#include <cstdint>
#include <expected>
#include <string>
#include <string_view>

struct GLFWwindow;

namespace elx::ui {

    /**
     * @brief Cross-platform window backed by GLFW.
     *
     * GLFW is initialised on first Window creation and terminated when the last
     * Window is destroyed.  Windows are move-only.
     */
    class Window {
     public:
        static constexpr int32_t k_default_width{ 800 };
        static constexpr int32_t k_default_height{ 600 };

        /** @brief Window creation parameters. */
        struct Config {
            int32_t          width{ k_default_width };
            int32_t          height{ k_default_height };
            std::string_view title{ "EmberLynx" };
        };

        Window()                         = delete;
        Window(Window const&)            = delete;
        Window& operator=(Window const&) = delete;

        Window(Window&& other) noexcept;
        Window& operator=(Window&& other) noexcept;

        ~Window();

        /**
         * @brief Create a Window.
         * @return The new Window, or an error string describing the failure.
         */
        [[nodiscard]] static std::expected<Window, std::string> create(Config const& config) noexcept;

        /** @brief Returns true when the OS or user has requested the window to close. */
        [[nodiscard]] bool should_close() const noexcept;

        /** @brief Process all pending OS events. */
        void poll_events() const noexcept;

        /**
         * @brief Swap front and back buffers, committing the current frame to the compositor.
         *
         * Must be called once per frame until a dedicated renderer (e.g. bgfx) takes over
         * surface commits.  Without this, Wayland compositors never map the surface.
         */
        void present() const noexcept;

        /** @brief Window width in screen coordinates. */
        [[nodiscard]] int32_t width() const noexcept;

        /** @brief Window height in screen coordinates. */
        [[nodiscard]] int32_t height() const noexcept;

        /** @brief Raw GLFW handle, useful for platform-specific integrations (e.g. bgfx). */
        [[nodiscard]] GLFWwindow* native_handle() const noexcept;

     private:
        explicit Window(GLFWwindow* handle) noexcept;

        GLFWwindow* handle_{ nullptr };
    };

} // namespace elx::ui

#endif // ELX_UI_WINDOW_HPP
