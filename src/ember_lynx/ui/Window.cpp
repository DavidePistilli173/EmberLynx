#include "ember_lynx/ui/Window.hpp"

#include <GLFW/glfw3.h>
#include <atomic>
#include <cstdlib>
#include <format>

namespace {

    // Read once before main() — environment is stable and no threads exist yet.
    bool const s_is_wayland{ std::getenv("WAYLAND_DISPLAY") != nullptr }; // NOLINT(concurrency-mt-unsafe)

    std::atomic<int> s_glfw_ref_count{ 0 };

    /// @brief Initialise GLFW on first call; no-op on subsequent calls.
    /// @return false if glfwInit() fails.
    [[nodiscard]] bool glfw_acquire() noexcept {
        if (s_glfw_ref_count.fetch_add(1, std::memory_order_acq_rel) == 0) {
            // Prevent GLFW from auto-selecting X11 on a Wayland session where
            // Xwayland may not be running (e.g. Niri).
            if (s_is_wayland) {
                glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
            }
            return glfwInit() == GLFW_TRUE;
        }
        return true;
    }

    /// @brief Terminate GLFW when the last reference is released.
    void glfw_release() noexcept {
        if (s_glfw_ref_count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            glfwTerminate();
        }
    }

} // namespace

namespace elx::ui {

    Window::Window(GLFWwindow* handle) noexcept : handle_{ handle } {}

    Window::Window(Window&& other) noexcept : handle_{ other.handle_ } {
        other.handle_ = nullptr;
    }

    Window& Window::operator=(Window&& other) noexcept {
        if (this != &other) {
            if (handle_ != nullptr) {
                glfwDestroyWindow(handle_);
                glfw_release();
            }
            handle_       = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

    Window::~Window() {
        if (handle_ != nullptr) {
            glfwDestroyWindow(handle_);
            glfw_release();
        }
    }

    std::expected<Window, std::string> Window::create(Config const& config) noexcept {
        if (!glfw_acquire()) {
            s_glfw_ref_count.fetch_sub(1, std::memory_order_acq_rel);
            return std::unexpected{ "Failed to initialise GLFW" };
        }

        GLFWwindow* handle = glfwCreateWindow(config.width, config.height, std::string{ config.title }.c_str(), nullptr, nullptr);

        if (handle == nullptr) {
            glfw_release();
            return std::unexpected{ std::format("Failed to create window '{}'", config.title) };
        }

        return Window{ handle };
    }

    bool Window::should_close() const noexcept {
        return glfwWindowShouldClose(handle_) != 0;
    }

    void Window::poll_events() const noexcept {
        glfwPollEvents();
    }

    void Window::present() const noexcept {
        glfwMakeContextCurrent(handle_);
        glfwSwapBuffers(handle_);
    }

    int32_t Window::width() const noexcept {
        int w{ 0 };
        glfwGetWindowSize(handle_, &w, nullptr);
        return static_cast<int32_t>(w);
    }

    int32_t Window::height() const noexcept {
        int h{ 0 };
        glfwGetWindowSize(handle_, nullptr, &h);
        return static_cast<int32_t>(h);
    }

    GLFWwindow* Window::native_handle() const noexcept {
        return handle_;
    }

} // namespace elx::ui
