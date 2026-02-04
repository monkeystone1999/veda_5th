#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>

namespace v5 {

enum class SSLMode { SERVER, CLIENT };

class CryptSSL {
  static SSL_CTX *sslCTX;
  static std::once_flag initFlag;

private:
  SSL *ssl;
  BIO *bioChain;
  SSLMode mode;
  CryptSSL(SSLMode m) : ssl(nullptr), bioChain(nullptr), mode(m) {
    std::call_once(initFlag, []() {
      SSL_load_error_strings();
      OpenSSL_add_ssl_algorithms();
      const SSL_METHOD *method = TLS_method();
      sslCTX = SSL_CTX_new(method);
      if (!sslCTX) {
        std::cerr << "SSL_CTX_new() fail!" << std::endl;
      }
    });
    ssl = SSL_new(sslCTX);
    if (mode == SSLMode::SERVER) {
      SSL_set_accept_state(ssl);
    } else {
      SSL_set_connect_state(ssl);
    }
    bioChain = BIO_new(BIO_f_ssl());
    /// 이 부분에서 인증서를 받은 ssl 과 bioChain 이 연결이 된다.
    BIO_set_ssl(bioChain, ssl, BIO_CLOSE);
  };
  ~CryptSSL() {
    if (bioChain) {
      BIO_free_all(bioChain);
    }
  };
  const SSL_METHOD *method;

public:
  static std::unique_ptr<CryptSSL> CreateCrypt(SSLMode m = SSLMode::SERVER) {
    return std::unique_ptr<CryptSSL>(new CryptSSL(m));
  }
  friend std::default_delete<CryptSSL>;
  int coupling(int client_socket) {
    if (!ssl || !bioChain)
      return -1;
    BIO *socketBio = BIO_new_socket(client_socket, BIO_NOCLOSE);
    BIO_push(bioChain, socketBio);
  }
  int handshake() {
    int ret = BIO_do_handshake(bioChain);
    if (ret <= 0) {
      return -1;
    }
    return 1;
  }
  int loadCRT(const char *file_locate) {
    if (SSL_CTX_use_certificate_file(sslCTX, file_locate, SSL_FILETYPE_PEM) <=
        0) {
      std::cerr << "Certificate File Load Fail\n" << std::endl;
    }
  }
  int write(const std::string &msg) {
    if (!bioChain)
      return -1;
    int TotalWritten = 0;
    int len = static_cast<int>(msg.length());
    while (TotalWritten < len) {
      int ret = BIO_write(bioChain, msg.data() + TotalWritten, len);
      if (ret <= 0) {
        if (BIO_should_retry(bioChain))
          continue;
        return 01;
      }
      TotalWritten += ret;
    }
    BIO_flush(bioChain);
    return TotalWritten;
  }
  int read(std::string &buffer) {
    if (!bioChain)
      return -1;
    char tmp[4096];
    int ret = BIO_read(bioChain, tmp, sizeof(tmp));
    if (ret <= 0) {
      if (BIO_should_retry(bioChain))
        return 0;
      return -1;
    }
    buffer.assign(tmp, ret);
    return ret;
  }
  int loadKEY(const char *file_locate) {
    if (SSL_CTX_use_PrivateKey_file(sslCTX, file_locate, SSL_FILETYPE_PEM) <=
        0) {
      std::cerr << "Certificate File Load Fail\n" << std::endl;
    }
  }
  int loadCA(const char *file_locate) {
    if (SSL_CTX_load_verify_locations(sslCTX, file_locate, nullptr) <= 0) {
      std::cerr << "CA File Load Fail\n" << std::endl;
      return -1;
    }
    return 0;
  }
};

SSL_CTX *CryptSSL::sslCTX = nullptr;
std::once_flag CryptSSL::initFlag;
} // namespace v5
