#ifndef TLSSERVER_H
#define TLSSERVER_H

#include "TcpServer.h"

#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <string>

namespace stream {

    class TlsServer : public TcpServer {
    public:
        /**
         * Tls constructor, initialize socket on port ...
         * @param port
         */
        explicit TlsServer(const int &port);

        /**
         * Tls destructor, free SSL context
         */
        ~TlsServer();

        /**
         * Configure TLS context
         * @param  certificate - path to certificate
         * @param  privateKey  - path to private key
         * @return 0 on success, -1 otherwise
         */
        int configureContext(const std::string &certificate, const std::string &privateKey);

        /**
         * Get secure socket layer
         * @return SSL pointer
         */
        SSL *getSSL();

        /**
         * Accept TLS client connection
         * @return new socket, -1 on error
         */
        int tlsAccept();

        /**
         * TLS receive over SSL
         * @param  buffer  - receiving buffer
         * @param  bufSize - size of a receiving buffer
         * @return bytes received, -1 on error
         */
        int tlsReceive(char* buffer, size_t bufSize);

        static int tlsReceive(SSL *ssl, char* buffer, size_t bufSize); /* static TLS receive over SSL */

        /**
         * TLS receive over SSL bytes of size
         * @param  buffer  - receiving buffer
         * @param  len     - number of bytes we wish to receive
         * @param  bufSize - size of a receiving buffer
         * @return bytes received
         */
        int tlsReceive(char* buffer, size_t bufSize, size_t size);

        static int tlsReceive(SSL *ssl, char* buffer, size_t bufSize, size_t size); /* static TLS receive over SSL bytes of size */

        /**
         * TLS send over SSL
         * Function sends packet only once - it may not send everything. Return number of bytes sent.
         * @param buffer - buffer to send
         * @param size - number of bytes to send
         * @return bytes sent
         */
        int tlsSend(const char* buffer, size_t size);

        /**
         * TLS send over SSL
         * Function loops until all bytes are sent. Return 0 on success or -1 on error.
         * @param  buf - to send
         * @param   - to send
         * @return -1 on fail, 0 on success
         */
        int tlsSendAll(const char *buffer, size_t size);

        static int tlsSendAll(SSL *ssl, const char *buffer, int size);

        /**
         * TLS send whole string content over SSL
         * @param  str - string container we wish to send
         * @return -1 on fail, 0 on success
         */
        int tlsSendAll(const std::string& str);

        static int tlsSendAll(SSL *ssl, const std::string& str);

        void shutdownSsl();

    private:
        /**
         * Create TLS context
         * @return 0 on succes, -1 otherwise
         */
        SSL_CTX* createContext();

        SSL_CTX* ctx_ = nullptr;
        SSL* ssl_ = nullptr;
    };

} // namespace

#endif
