#pragma once
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <string>
#include <type_traits>
#include <vector>

namespace v5 {

// template <typename T>
// concept TLSRequire =
//     requires(T t, std::string &read, const std::string &write) {
//       { t.read(read) } -> std::same_as<int>;
//       { t.write(write) } -> std::same_as<int>;
//    };

class TLSContext {
public:
  ~TLSContext();
  TLSContext(SSL_CTX *ctx) {
    ssl = SSL_new(ctx);
    readBio = BIO_new(BIO_s_mem());
    writeBio = BIO_new(BIO_s_mem());
    SSL_set_bio(ssl, readBio, writeBio);
    SSL_set_accept_state(ssl);
  }
  std::vector<uint8_t> read() {}
  void write(std::vector<uint8_t> buffer) {}
  SSL *getSSL() { return ssl; }

private:
  SSL *ssl;
  BIO *readBio, *writeBio;
};
} // namespace v5
