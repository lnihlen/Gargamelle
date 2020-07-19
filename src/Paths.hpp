#ifndef SRC_PATHS_HPP_
#define SRC_PATHS_HPP_

#include <cstddef>

namespace ggml { namespace handler {

enum Paths { kDump, kHello, kNotFound };

Paths getPath(const char* path, size_t length);

} // namespace handler
} // namespace ggml

#endif // SRC_PATHS_HPP_
