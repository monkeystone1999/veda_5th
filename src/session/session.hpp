#pragma once
#include "./anyprotocol.hpp"
#include "./crypto_state.hpp"
#include "./tls_context.hpp"
#include <map>
#include <memory>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <type_traits>
namespace v5 {
/// std::map<fd, Session> mapping;
/// ECDHE 는 확정으로 하고 대칭키 알고리즘만 선택하게 하자

template <Protocol auto &ProtocolInstance> class Session : public SessionBase {
private:
  CryptoState CryptoState_;
  TLSContext TLSContext_;

public:
  void Read();
  void Write();
  Session(SSL_CTX *ctx, CipherSpec spec);
  ~Session() {}
};
} // namespace v5
