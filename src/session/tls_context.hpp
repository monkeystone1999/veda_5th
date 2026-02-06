#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <string>
#include <type_traits>

namespace v5 {
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
  int read(std::string &buffer) {}
  int write(const std::string &buffer) {}

private:
  SSL *ssl;
  BIO *readBio, *writeBio;
};
} // namespace v5
