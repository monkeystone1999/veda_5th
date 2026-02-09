#include <iostream>
#include <openssl/evp.h>
#include <span>
#include <string>

namespace v5 {
struct CipherSpec {
  const EVP_CIPHER *(*evp)();
  bool is_aead;
  size_t key_len;
  size_t iv_len;
};
class CryptoState {
public:
  CryptoState();
  bool encrypt(std::string &data);
  bool decrypt(std::string &data);

private:
  CipherSpec BasicSpec;
  EVP_CIPHER_CTX *ctx;
  uint64_t seq;
};

class ChaCha20_poly {
public:
  ChaCha20_poly(const CipherSpec &spec, std::span<const uint8_t> key,
                std::span<const uint8_t> iv);
  bool encrypt(std::string &data);
  bool decrypt(std::string &data);

private:
  EVP_CIPHER_CTX *ctx;
  uint64_t seq;
};

ChaCha20_poly::ChaCha20_poly(const CipherSpec &spec,
                             std::span<const uint8_t> key,
                             std::span<const uint8_t> iv) {
  ctx = EVP_CIPHER_CTX_new();
  int result =
      EVP_CipherInit_ex(ctx, spec.evp(), nullptr, key.data(), iv.data(), 1);
  if (result < 0) {
    std::cerr << "EVP_CipherInit Error" << std::endl;
  }
}

} // namespace v5
