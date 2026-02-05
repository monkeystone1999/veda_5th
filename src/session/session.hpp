#pragma once
#include "./anycipher.hpp"
#include <map>
#include <memory>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <type_traits>
namespace v5 {

/// ECDHE 는 확정으로 하고 대칭키 알고리즘만 선택하게 하자
class Session : public std::enable_shared_from_this<Session> {
private:
  SSL *ssl;
  BIO *readBio, *writeBio;
  AnyCipher AnyCipher_;

public:
  template <Cipher C> Session(SSL_CTX *ctx, C c) : AnyCipher_(c) {
    ssl = SSL_new(ctx);
    readBio = BIO_new(BIO_s_mem());
    writeBio = BIO_new(BIO_s_mem());
    SSL_set_bio(ssl, readBio, writeBio);
    SSL_set_accept_state(ssl);
  };
  int read(std::string &buffer) { return AnyCipher_.decrypt(ssl, buffer); }
  int write(const std::string &buffer) {
    return AnyCipher_.encrypt(ssl, buffer);
  }
  ~Session() {
    SSL_free_buffers(ssl);
    SSL_free(ssl);
    BIO_free(readBio);
    BIO_free(writeBio);
  }
};
} // namespace v5
