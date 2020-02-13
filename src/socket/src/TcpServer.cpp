#include "TcpServer.h"

#include <unistd.h>
#include <cstring>

namespace stream {

    const int TcpServer::ERROR = -1;

    TcpServer::
    TcpServer(int port)
            : sock(-1),
              newSock(-1),
              port_(port) {}

    int TcpServer::
    tcpInitialize() {
        int yes = 1;

        memset(&address_, 0, sizeof(address_));
        address_.sin_family = AF_INET;
        address_.sin_addr.s_addr = htonl(INADDR_ANY);
        address_.sin_port = htons(port_);

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) { // init
            return ERROR;
        }

        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)); //Avoid bind error

        if (bind(sock, (struct sockaddr *)&address_, sizeof(address_)) == -1) { // Bind
            return ERROR;
        }

        if (listen(sock, 128) == -1) { // Listen
            return ERROR;
        }

        return sock;
    }

    int TcpServer::
    tcpAccept() {
        socklen_t sosize = sizeof(address_); // Get the size of a sock
        if ((newSock = accept(sock, (struct sockaddr *)&address_, &sosize)) == -1) {
            return ERROR;
        }
        return newSock; // return new sock Fd on success
    }

    int TcpServer::
    tcpReceive(char* buffer, size_t bufSize) {
        memset(buffer, 0, bufSize);
        return recv(newSock, buffer, bufSize, 0);
    }

    int TcpServer::
    tcpReceive(int sock, char* buffer, size_t bufSize) {
        memset(buffer, 0, bufSize);
        return recv(sock, buffer, bufSize, 0);
    }

    int TcpServer::
    tcpReceive(char* buffer, size_t bufSize, size_t size) {
        if (size > bufSize) { return ERROR; }

        int bytesReceived = 0, bytes = 0;
        memset(buffer, 0, bufSize);

        while (bytesReceived < (int)size) {
            bytes = recv(newSock, &buffer[bytesReceived], size - bytesReceived, 0);
            bytesReceived += bytes;
            if (bytes == -1) { return bytes; }
            if (bytes == 0) { return bytes; }
        }

        return bytesReceived; /* Return bytes received */
    }

    int TcpServer::
    tcpReceive(int sock, char* buffer, size_t bufSize, size_t size) {
        if (size > bufSize) { return ERROR; }

        int bytesReceived = 0, bytes = 0;
        memset(buffer, 0, bufSize);

        while (bytesReceived < (int)size) {
            bytes = recv(sock, &buffer[bytesReceived], size - bytesReceived, 0);
            bytesReceived += bytes;
            if (bytes == -1) { return bytes; }
            if (bytes == 0) { return bytes; }
        }

        return bytesReceived; /* Return bytes received */
    }

    int TcpServer::
    tcpSend(const char *buffer, size_t size) {
        size_t *len = &size;
        int total = 0;  /* How many bytes sent */
        int bytesLeft = *len;   /* How many left to send */
        int n;

        while(total < (int)*len) {
            n = send(newSock, buffer+total, bytesLeft, MSG_NOSIGNAL);
            if (n == -1) {
                return ERROR;
            }
            total += n;
            bytesLeft -= n;
        }
        *len = total;

        return total; /* Return bytes sent */
    }

    int TcpServer::
    tcpSend(const std::string& str) {
        const char *buff = str.c_str();
        return tcpSend(buff, strlen(buff));
    }

    int TcpServer::
    tcpSend(int sock, const char *buffer, int size) {
        int *len = &size;
        int total = 0;  //How many bytes we've sent
        int bytesleft = *len;   //How many we have left to send
        int n;

        while(total < *len) {
            n = send(sock, buffer+total, bytesleft, MSG_NOSIGNAL);
            if (n == -1) {
                return ERROR;
            }
            total += n;
            bytesleft -= n;
        }
        *len = total; // Return number actually sent here

        return total; // return bytes sent
    }

    int TcpServer::
    tcpSend(int sock, const std::string& str) {
        const char *buff = str.c_str();
        return TcpServer::tcpSend(sock, buff, strlen(buff));
    }

    void TcpServer::
    setSocketTimeout(int time_s) {
        tv_.tv_sec = time_s;
        tv_.tv_usec = 0;
        setsockopt(newSock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_, sizeof tv_);
    }

    void TcpServer::
    setSocketTimeout(int sock, int time_s) {
        struct timeval tv{};
        tv.tv_sec = time_s;
        tv.tv_usec = 0;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    }

    int TcpServer::
    isSocketActive() {
        int error = 0;
        socklen_t len = sizeof (error);
        int ret = getsockopt(newSock, SOL_SOCKET, SO_ERROR, &error, &len);

        if (ret != 0) {
            return ERROR;
        }
        if (error != 0) {
            return ERROR;
        }

        return 0;
    }

    int TcpServer::
    isSocketActive(int sock) {
        int error = 0;
        socklen_t len = sizeof (error);
        int ret = getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &len);

        if (ret != 0) {
            return ERROR;
        }
        if (error != 0) {
            return ERROR;
        }

        return 0;
    }

    int TcpServer::
    closeNewSocket() {
        int error_code;
        socklen_t error_code_size = sizeof(error_code);
        if (getsockopt(newSock, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size) == -1) {
            return ERROR;
        } else {
            return close(newSock);
        }
    }

    int TcpServer::
    closeSocket() {
        int error_code;
        socklen_t error_code_size = sizeof(error_code);
        if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size) == -1) {
            return ERROR;
        } else {
            return close(sock);
        }
    }

    int TcpServer::
    closeSocket(int so) {
        int error_code;
        socklen_t error_code_size = sizeof(error_code);
        if (getsockopt(so, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size) == -1) {
            return ERROR;
        } else {
            return close(so);
        }
    }

} // namespace
