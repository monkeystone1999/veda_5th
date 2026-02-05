#pragma once
#include <concepts>
#include <memory>
#include <openssl/ssl.h>
#include <string>
namespace v5 {

template <typename T>
concept Cipher =
    requires(T t, std::string &data, SSL *ssl, std::string &c_data) {
      { t.encrypt(ssl, c_data) } -> std::same_as<bool>;
      { t.decrypt(ssl, data) } -> std::same_as<bool>;
    };
class AnyCipher {
public:
  template <Cipher C>
  AnyCipher(C c) : self(std::make_unique<Model<C>>(std::move(c))) {}
  bool encrypt(SSL *ssl, const std::string &data) {
    return self->encrypt(ssl, data);
  }
  bool decrypt(SSL *ssl, std::string &data) { return self->decrypt(ssl, data); }

private:
  struct Concept {
    virtual ~Concept() = default;
    virtual bool encrypt(SSL *ssl, const std::string &) = 0;
    virtual bool decrypt(SSL *ssl, std::string &) = 0;
  };

  template <Cipher C> struct Model final : Concept {
    C impl;
    Model(C c) : impl(std::move(c)) {}
    bool encrypt(SSL *ssl, const std::string &d) override {
      return impl.encrypt(ssl, d);
    }
    bool decrypt(SSL *ssl, std::string &d) override {
      return impl.decrypt(ssl, d);
    }
  };

  std::unique_ptr<Concept> self;
};
} // namespace v5
