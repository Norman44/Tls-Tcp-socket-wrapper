#include "IoException.h"

namespace stream {

    IoException::
    IoException(const std::string& msg, const std::string& type) noexcept(true) :
            std::runtime_error(type + msg) {
    }

//    IoException::
//    IoException(const IoException& ex) noexcept(true) :
//            std::runtime_error(ex) {
//    }

}
