#pragma once
#include <openssl/ssl.h>
#include <string>
namespace v5 {
class ChaCha20_Poly1305 {
public:
  ChaCha20_Poly1305();
  bool encrypt(const std::string &data) {}
  bool decrypt(std::string &data) {}
};

} // namespace v5
