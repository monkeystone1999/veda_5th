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
  CryptoState(const CipherSpec &spec, std::span<const uint8_t> key,
              std::span<const uint8_t> iv);
  bool encrypt(std::string &data);
  bool decrypt(std::string &data);

private:
  EVP_CIPHER_CTX *ctx;
  uint64_t seq;
};
} // namespace v5
