
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
///
#include <functional>
#include <map>
#include <string>

namespace v5 {
class TCP {
  int port;

public:
  TCP(int port = -1);
  ~TCP();
};
} // namespace v5
