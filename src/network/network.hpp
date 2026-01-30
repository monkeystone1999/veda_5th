#include <arpa/inet.h>
#include <memory>
#include <unistd.h>

namespace v5 {
template <typename T> /// TCP class or UDP class
class Network {
private:
  Network();
  ~Network();

public:
  static Network getInstance() {
    static Network net;
    return net;
  }
  friend std::default_delete<Network>;
};

template <typename T> /// Packet struct
class Session {};
} // namespace v5
