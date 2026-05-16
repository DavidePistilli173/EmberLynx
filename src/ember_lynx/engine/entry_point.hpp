#ifndef ELX_ENGINE_ENTRY_POINT_HPP
#define ELX_ENGINE_ENTRY_POINT_HPP

#include "ember_lynx/engine/Engine.hpp"

namespace elx::engine {

    /// @brief User-provided: supply the Engine configuration before startup.
    [[nodiscard]] Engine::Config app_config() noexcept;

    /// @brief User-provided: called once per frame after engine events are processed.
    void app_update(Engine& engine) noexcept;

} // namespace elx::engine

#endif // ELX_ENGINE_ENTRY_POINT_HPP
