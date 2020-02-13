#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

namespace stream {

    /**
     * Tcp server socket wrapper class
     */
    class TcpServer {
    public:
        static const int ERROR;

        /**
         * Tcp server socket constructor on port
         * @param port
         */
        explicit TcpServer(int port);

        /**
         * Initialize socket - bind to address and listen
         * @return server socket or -1 on fail
         */
        int tcpInitialize();

        /**
         * Accept conencting node
         * @return new socket or -1 on fail
         */
        int tcpAccept();

        /**
         * Tcp receive
         * @param  buffer  - buffer to receive to
         * @param  bufSize - buffer size
         * @return bytes received or -1 on fail
         */
        int tcpReceive(char* buffer, size_t bufSize);

        static int tcpReceive(int sock, char* buffer, size_t bufSize); /* Static tcp receive on socket "sock" */

        /**
         * Tcp receive bytes of size. "size" must not exceed bufSize value.
         * @param  buffer  - buffer to receive to
         * @param  bufSize - size of a receiving buffer
         * @param  size     - bytes to receive
         * @return bytes received
         */
        int tcpReceive(char* buffer, size_t bufSize, size_t size);

        static int tcpReceive(int sock, char* buffer, size_t bufSize, size_t size); /* Static tcp receive bytes of size len on socket "sock" */

        /**
         * TCP send bytes
         * @param  buffer
         * @param  size - number of bytes to send
         * @return bytes sent on success -1 on error
         */
        int tcpSend(const char *buffer, size_t size);

        static int tcpSend(int sock, const char *buffer, int size); /* Static tcp send bytes of size "size" from socket "sock" */

        /**
         * TCP send string container
         * @param  str - string container to send
         * @return bytes sent on success -1 on error
         */
        int tcpSend(const std::string& str);

        static int tcpSend(int sock, const std::string& str); /* Static tcp send from socket "sock" */

        /**
         * Set timeout on a new socket
         * @param time_s - time in seconds
         */
        void setSocketTimeout(int time_s);

        static void setSocketTimeout(int sock, int time_s); /* Set timeout on a socket "sock" */

        /**
         * Check if new socket is active
         * @return -1 if socket not active, else 0
         */
        int isSocketActive();

        static int isSocketActive(int sock);

        /**
         * Close new sock node
         * @return 0 on succes, -1 otherwise
         */
        int closeNewSocket();

        /**
         * Close server socket
         * @return 0 on succes, -1 otherwise
         */
        int closeSocket();

        static int closeSocket(int so);

        int sock, newSock;
    private:
        struct sockaddr_in address_{}; // Socket address struct
        int port_;

        struct timeval tv_{};

    };

} // namespace

#endif
