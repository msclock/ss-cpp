#include "distribution.hpp"

namespace uiqt {
namespace distribution {
bool is_debug() noexcept {
#ifdef _DEBUG
    return true;
#else
    return false;
#endif
}
} // namespace distribution
} // namespace uiqt
