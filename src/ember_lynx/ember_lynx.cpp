#include "ember_lynx/ember_lynx.hpp"

namespace elx {

    eng::Engine& engine() {
        return eng::Engine::get();
    }

} // namespace elx
