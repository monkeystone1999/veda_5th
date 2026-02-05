#pragma once
#include <openssl/ssl.h>
#include <string>
namespace v5 {
class ChaCha20_Poly1305 {
public:
  ChaCha20_Poly1305();
  bool encrypt(SSL *ssl, const std::string &data) {}
  bool decrypt(SSL *ssl, std::string &data) {}
};

} // namespace v5
