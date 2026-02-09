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
  std::unique_ptr<CryptoState> CryptoState_;
  TLSContext TLSContext_;

public:
  void Read();
  void Write();
  Session(SSL_CTX *ctx, CipherSpec spec);
  ~Session() {}

private:
  struct ReplayWindow {
    uint64_t max_seq_seen;
    std::array<uint64_t, 64> past{};
    uint8_t indexing = 0;
    bool checkSequence(uint64_t seq) {
      uint8_t before = indexing;
      indexing++;
      indexing /= 64;
      if (past[before] > seq || past[before] + 64 < seq) {
        return false;
      }
      past[indexing] = seq;
      return true;
    }
  };
};
} // namespace v5
