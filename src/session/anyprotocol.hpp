
#pragma once
#include <array>
#include <bitset>
#include <concepts>
#include <memory>
#include <queue>
#include <string>
namespace v5 {

class SessionBase : public std::enable_shared_from_this<SessionBase> {
public:
  virtual ~SessionBase() = default;
};

template <typename T>
concept Protocol = requires(T t, std::string &body, std::string &head,
                            std::shared_ptr<SessionBase> session) {
  { t.Switching(session, head) } -> std::same_as<bool>;
  { t.read(head, body) } -> std::same_as<bool>;
  { t.write(head, body) } -> std::same_as<bool>;
};

enum class Type : uint8_t { HANDSHAKE = 0x01, MESSAGE = 0x02 };
enum class Flags : uint8_t { FIN = 0x01, ACK = 0x02 };

#pragma pack(push, 1)
struct Header {
  Type type;
  Flags flags;
  uint16_t length;
  uint64_t seq;
};
#pragma pack(pop)

class BaseProtocol {
  static constexpr uint8_t bit_size = 64;
};

namespace prev {
/// 쓰지 말 것! 추천이 되는 구조가 아님!!!
class AnyProtocol {
public:
  template <Protocol P> AnyProtocol(P &p) : self(&p) {}
  //  template<Protocol P> AnyProtocol(std::shared_ptr<P> p):
  //  self(std::make_shared<Model<P>>(p)){}
private:
  struct Concept {
    virtual bool Switching(std::shared_ptr<SessionBase> session,
                           std ::string &head) = 0;
    virtual bool read(std::string &body, std::string &head) = 0;
    virtual bool write(std::string &body, std::string &head) = 0;
    virtual ~Concept() = default;
  };
  template <Protocol P> struct Model : public Concept {
    P implement;
    virtual bool Switching(std::shared_ptr<SessionBase> session,
                           std ::string &head) {
      implement.Switching(session, head);
    }
    virtual bool read(std::string &body, std::string &head) {
      implement.read(body, head);
    }
    virtual bool write(std::string &body, std::string &head) {
      implement.write(body, head);
    }
  };

  void *self;
  /// std::shared_ptr<Concept> self;
};

} // namespace prev

} // namespace v5
