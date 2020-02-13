#ifndef NETWORKEXCEPTION_H
#define NETWORKEXCEPTION_H

#include "IoException.h"

namespace stream {

    /**
     * NetworkException is thrown when operation on socket fails.
     */
    class NetworkException : public IoException {
    public:
        /**
         * Constructor. Initializes the exception.
         *@param msg a message describing the cause.
         */
        explicit NetworkException(const std::string& msg) noexcept;

        NetworkException(const NetworkException& ex) noexcept = default;

    };

} // namespace

#endif
