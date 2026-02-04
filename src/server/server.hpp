#include <concepts>

namespace v5 {

template <typename T>
concept ProtocolRequire = requires(T t) {

};

class Server {
public:
  Server();
  int add();
};
} // namespace v5
