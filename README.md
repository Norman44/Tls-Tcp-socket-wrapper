# Tls-Tcp-socket-wrapper
C++ Server tls tcp socket wrapper

## Requirements
Install cmake and openssl dev lib ...
```
sudo apt-get install libssl-dev
```

Create certificate and key ...

## Compile 
```
cmake CMakeLists.txt
```
```
make all
```
```
./TlsServer
```

## Example
```cpp
#include <iostream>
#include <string>

#include "TlsServer.h"
#include "NetworkException.h"

using namespace stream;

int main() {
    char buff[1024];
    std::string str = "Hello there!";

    std::cout << "TLS server begin ..." << std::endl;

    TlsServer tls(8080);

    try {
        if (tls.configureContext("/path/to/certificate.pem", "/path/to/key.pem") == -1) {
            throw NetworkException("configure context error, check certificate and key ...");
        }

        if (tls.tlsAccept() == -1) {
            throw NetworkException("accept error");
        }

        while (1) {
            int bytesReceived = tls.tlsReceive(buff, sizeof buff);

            if (bytesReceived == 0) { /* Client closed connection */
                std::cout << "client closed connection" << std::endl; // Hello
                tls.shutdownSsl();
                break;

            } else if (bytesReceived == -1) {
                tls.shutdownSsl();
                throw NetworkException("receive error");

            } else {
                std::cout << buff << std::endl;
                if (tls.tlsSendAll(str) == -1) {
                    tls.shutdownSsl();
                    throw NetworkException("send error");
                }
            }
        }

    } catch (NetworkException& ex) {
        std::cerr << ex.what() << std::endl;
    }

    std::cout << "TLS server finish." << std::endl;
    return 0;
}

```

## Client 
Client code available https://github.com/Norman44/Tls-Tcp-socket-wrapper-client
