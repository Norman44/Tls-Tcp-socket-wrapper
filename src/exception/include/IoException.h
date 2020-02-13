#ifndef IOEXCEPTION_H
#define IOEXCEPTION_H

#include <stdexcept>
#include <string>

namespace stream {

    /**
     * IoException is thrown when input/output error occurs.
     */
    class IoException : public std::runtime_error {
    public:

        /**
         * Constructor. Initializes the exception.
         *@param msg a message describing the cause.
         */
        explicit IoException(const std::string& msg, const std::string& type="IoException") noexcept(true);

        IoException(const IoException& ex) noexcept(true) = default;
    };

} // namespace

#endif
