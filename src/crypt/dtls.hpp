#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <string>

namespace v5 {

namespace DTLS { /// 템플릿화 생각해두기
enum class KeyExch { ECDH, RSA };
enum class MsgSend { ChaCha20, AES_GCM };
} // namespace DTLS

namespace Client {}

namespace Protocol {
#pragma pack(push, 1)
struct Header {
  uint32_t Salt;
  uint64_t SessionID;
};
#pragma pack(pop)
}; // namespace Protocol
namespace Server {

std::string certFile, keyFile, CAcertFile;

class Base {
protected:
  static SSL_CTX *ctx;

public:
  std::once_flag sslLib;
  Base() {
    std::call_once(sslLib, [] {
      OpenSSL_add_all_algorithms();
      SSL_load_error_strings();
    });
  }
  virtual ~Base();
};

class Crypt : protected Base {
private:
  Crypt() {}
  ~Crypt() override;

  static std::map<long long, SSL *> m;
  long long SessionID;

public:
  static Crypt *GetInstance() {
    static Crypt net;
    return &net;
  }

  void newSession() {
    /// new Session 받기
  }

  void initFiles() {
    ctx = SSL_CTX_new(DTLS_server_method());
    SSL_CTX_use_certificate_file(ctx, certFile.c_str(), SSL_FILETYPE_PEM);
    SSL_CTX_use_PrivateKey_file(ctx, keyFile.c_str(), SSL_FILETYPE_PEM);
    SSL_CTX_load_verify_locations(ctx, CAcertFile.c_str(), NULL);
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT,
                       NULL);
  }
  static std::string crypt(long long SessionId, const std::string message) {
    auto f = m.find(SessionId);
    if (f == m.end()) {
      return "";
    }
  }
  static std::string decrypt(long long SessionId, const std::string message) {
    auto f = m.find(SessionId);
    if (f == m.end()) {
      return "";
    }
  }
};
} // namespace Server
} // namespace v5
