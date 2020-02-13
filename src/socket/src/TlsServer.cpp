#include "TlsServer.h"

#include <iostream>
#include <cstring>

namespace stream {

    TlsServer::
    TlsServer(const int &port)
    : TcpServer(port) {
        SSL_load_error_strings();
        OpenSSL_add_ssl_algorithms();
        createContext();
    }

    TlsServer::
    ~TlsServer() {
        shutdownSsl(); /* If not shutdown from another thread */

        TcpServer::closeSocket();

        if(ctx_ != nullptr) {
            SSL_CTX_free(ctx_);
        }

        EVP_cleanup();
    }

    int TlsServer::
    configureContext(const std::string &certificate, const std::string &privateKey) {
        int success = 0;

        if (!ctx_) { return ERROR; }

        SSL_CTX_set_ecdh_auto(ctx_, 1);

        if (SSL_CTX_use_certificate_file(ctx_, certificate.c_str(), SSL_FILETYPE_PEM) <= 0) {
            return ERROR;
        }

        if (SSL_CTX_use_PrivateKey_file(ctx_, privateKey.c_str(), SSL_FILETYPE_PEM) <= 0 ) {
            return ERROR;
        }

        if (TcpServer::tcpInitialize() == -1) { return ERROR; } /* Initialize server socket */

        return success;
    }

    SSL* TlsServer::
    getSSL() { return ssl_; }

    int TlsServer::
    tlsAccept() {
        if (ctx_ == nullptr) { return ERROR; } //return -1;

        if (TcpServer::tcpAccept() == -1) { return ERROR; };

        ssl_ = SSL_new(ctx_);
        SSL_set_fd(ssl_, TcpServer::newSock);
        if (SSL_accept(ssl_) <= 0) { // r = SSLaccept():
//            int ret = SSL_get_error(ssl_, r);
            return ERROR;
        }

        return TcpServer::newSock;
    }

    int TlsServer::
    tlsReceive(char* buffer, size_t bufSize) {
        if (ssl_ == nullptr || sock < 0) {
            return ERROR;
        }
        memset(buffer, 0, bufSize);
        int bytes = SSL_read(ssl_, buffer, bufSize);

        return bytes;
    }

    int TlsServer::
    tlsReceive(SSL *ssl, char* buffer, size_t bufSize) {
        if (ssl == nullptr) {
            return ERROR;
        }

        memset(buffer, 0, bufSize);
        int bytes = SSL_read(ssl, &buffer[0], bufSize);

        return bytes;
    }

    int TlsServer::
    tlsReceive(char* buffer, size_t bufSize, size_t size) {
        if (size > bufSize) { return ERROR; }

        if (ssl_ == nullptr || sock < 0) {
            return ERROR;
        }

        int bytesReceived = 0, bytes = 0;
        memset(buffer, 0, bufSize);

        while (bytesReceived < (int)size) {
            bytes = SSL_read(ssl_, &buffer[bytesReceived], (int)size-bytesReceived);
            bytesReceived += bytes;
            if (bytes == -1) { return bytes; }
            if (bytes == 0) { return bytes; }
        }

        return bytesReceived;
    }

    int TlsServer::
    tlsReceive(SSL *ssl, char* buffer, size_t bufSize, size_t size) {
        if (size > bufSize) { return ERROR; }

        if (ssl == nullptr) {
            return ERROR;
        }

        int bytesReceived = 0, bytes = 0;
        memset(buffer, 0, bufSize);

        while (bytesReceived < (int)size) {
            bytes = SSL_read(ssl, &buffer[bytesReceived], (int)size-bytesReceived);
            bytesReceived += bytes;
            if (bytes == -1) { return bytes; }
            if (bytes == 0) { return bytes; }
        }

        return bytesReceived;
    }

    int TlsServer::
    tlsSend(const char* buffer, size_t size) {
        if (ssl_ == nullptr || sock < 0) {
            return ERROR;
        }
        return SSL_write(ssl_, buffer, size);
    }


    int TlsServer::
    tlsSendAll(const char *buffer, size_t size) {
        if (ssl_ == nullptr || sock < 0) {
            return ERROR;
        }

        size_t *len = &size;
        int total = 0;  //How many bytes we've sent
        int bytesLeft = *len;   //How many we have left to send
        int n;

        while(total < (int)*len) {
            n = SSL_write(ssl_, buffer+total, bytesLeft);
            if (n == -1) { return ERROR; }
            total += n;
            bytesLeft -= n;
        }
        *len = total; // Return number actually sent here

        return n == -1 ? -1 : 0; // return -1 on fail, 0 on success
    }

    int TlsServer::
    tlsSendAll(const std::string& str) {
        const char *buff = str.c_str();
        return tlsSendAll(buff, strlen(buff));
    }

    int TlsServer::
    tlsSendAll(SSL *ssl, const char *buffer, int size) {
        if (ssl == nullptr) {
            return ERROR;
        }

        int *len = &size;
        int total = 0;  //How many bytes we've sent
        int bytesLeft = *len;   //How many we have left to send
        int n;

        while(total < *len) {
            n = SSL_write(ssl, buffer+total, bytesLeft);
            if (n == -1) { return ERROR; }
            total += n;
            bytesLeft -= n;
        }
        *len = total; // Return number actually sent here

        return n == -1 ? -1 : 0; // return -1 on fail, 0 on success
    }

    int TlsServer::
    tlsSendAll(SSL *ssl, const std::string& str) {
        const char *buff = str.c_str();
        return TlsServer::tlsSendAll(ssl, buff, strlen(buff));
    }

    void TlsServer::
    shutdownSsl() {
        if (ssl_ != nullptr) {
            SSL_shutdown(ssl_);
            SSL_free(ssl_);
            ssl_ = nullptr;

            TcpServer::closeNewSocket();
        }
    }

    SSL_CTX* TlsServer::
    createContext() {
        const SSL_METHOD *method;

        method = SSLv23_server_method();

        ctx_ = SSL_CTX_new(method);

        return ctx_;
    }

} // namespace
