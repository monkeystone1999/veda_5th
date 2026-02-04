
/// usage : Network net;
/// net(TCP(80));
/// net(UDP(60000));
/// net.run();
///
/// 해당 port 가 해야할 일들을 TCP 와 UDP 에 기입
/// Network 클래스는 TCP 의 어떠한 기능을 epoll 시 해당 TCP 클래스 객체에 전송을
/// 한다.
/// net.route("/auth/*")
/// packet 에서 url="" 이 부분을 넣자
/// 프로토콜은 어떤 것을 갖춰야할까? next() 를 했을 때 다음 문자열의 어떠한 값을
/// 가져왔으면 하는데 근데 reinterpreter 를 통해서 serialization 이 가능하다.
#include <concepts>
#include <functional>
namespace v5 {

template <typename T, typename R>
concept ProtocolRequire_t = requires(T t, R r) {
  { t.run(r) } -> std::same_as<void>;
};

template <ProtocolRequire_t ProtocolRequire> class TCP {
  int port;
  ProtocolRequire Header;

public:
  TCP(int port = -1);
  ~TCP();
  int getPort() const { return port; }
  int HeaderSwitch(std::function<ProtocolRequire> &&Pre) { Pre(Header); }
};
} // namespace v5
