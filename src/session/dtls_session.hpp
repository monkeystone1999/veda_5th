#pragma once
#include "./tls_context.hpp"
#include "crypto_state.hpp"
#include <cstring>
#include <memory>
#include <vector>

namespace v5 {
/// Protocol 은 Singleton 으로 구성
/// CipherSpec 은 모든 암호화 알고리즘에 CipherSpec 을 상속받아
/// static 구조체로 만들자
/// 모든 Key 와 IV, Tag 는 Session
template <typename T>
concept CipherRequire =
    requires(T t, SSL *ssl, BIO *bio, std::vector<uint8_t> &body) {
      { t.encrypt(ssl, body) } -> std::same_as<std::vector<uint8_t>>;
      { t.decrypt(ssl, body) } -> std::same_as<std::vector<uint8_t>>;
    };
class DTLSSession {
public:
  template <CipherRequire C>
  DTLSSession(SSL_CTX *ssl, C c)
      : self(std::make_unique<Model<C>>(std::move(c))), tlsContext(ssl) {}

  template <typename Header> int Read(Header *header, std::string &data) {
    std::vector<uint8_t> packet = tlsContext.read();
    if (packet.empty())
      return -1;
    std::vector<uint8_t> body = self->decrypt(tlsContext.getSSL(), packet);
    if (body.empty() || body.size() < sizeof(Header))
      return -1;
    std::memcpy(header, body.data(), sizeof(Header));
    data.assign(body.begin() + sizeof(Header), body.end());
    return 0;
  }

  template <typename Header> int Write(Header *header, std::string &data) {
    std::vector<uint8_t> packet;
    packet.reserve(sizeof(Header) + data.size());
    const uint8_t *header_ptr = reinterpret_cast<const uint8_t *>(header);
    packet.insert(packet.end(), header_ptr, header_ptr + sizeof(Header));
    packet.insert(packet.end(), data.begin(), data.end());
    std::vector<uint8_t> encrypted = self->encrypt(tlsContext.getSSL(), packet);
    if (encrypted.empty()) {
      return -1;
    }
    tlsContext.write(encrypted);
    return 0;
  }

private:
  struct Concept {
    virtual std::vector<uint8_t> encrypt(SSL *ssl,
                                         std::vector<uint8_t> body) = 0;
    virtual std::vector<uint8_t> decrypt(SSL *ssl,
                                         std::vector<uint8_t> body) = 0;
    virtual ~Concept() = default;
  };

  template <CipherRequire C> struct Model final : public Concept {
    C impl;
    Model(C c) : impl(std::move(c)) {}
    virtual std::vector<uint8_t> encrypt(SSL *ssl,
                                         std::vector<uint8_t> body) override {
      return impl.encrypt(ssl, body);
    };
    virtual std::vector<uint8_t> decrypt(SSL *ssl,
                                         std::vector<uint8_t> body) override {
      return impl.decrypt(ssl, body);
    };
  };
  std::unique_ptr<Concept> self;
  TLSContext tlsContext;
  std::vector<uint8_t> body;
};
} // namespace v5
