#pragma once
#include <concepts>
#include <memory>
#include <string>
namespace v5 {

class Session;

template <typename T>
concept Protocol = requires(T t, std::string &body, std::string &head,
                            std::shared_ptr<Session> session) {
  { t.Switching(session, head) } -> std::same_as<bool>;
  { t.read(head, body) } -> std::same_as<bool>;
  { t.write(head, body) } -> std::same_as<bool>;
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
    virtual bool Switching(std::shared_ptr<Session> session,
                           std ::string &head) = 0;
    virtual bool read(std::string &body, std::string &head) = 0;
    virtual bool write(std::string &body, std::string &head) = 0;
    virtual ~Concept() = default;
  };
  template <Protocol P> struct Model : public Concept {
    P implement;
    virtual bool Switching(std::shared_ptr<Session> session,
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
