#pragma once

#include <cstddef>
#include <boost/asio.hpp>

// these macros are meant to be overridden by -D flag
// do not define them anywhere else.

#ifdef _WIN32
#   define WINDOWS
#endif // _WIN32

#ifndef CONFIG_RECEIVE_BUFFER_SIZE
#   define CONFIG_RECEIVE_BUFFER_SIZE 4096
#endif // CONFIG_RECEIVE_BUFFER_SIZE

#ifndef CONFIG_READ_TIMEOUT
#   define CONFIG_READ_TIMEOUT 11
#endif // CONFIG_READ_TIMEOUT

namespace serial
{
    namespace asio = boost::asio;

    namespace config
    {
        constexpr static std::size_t buffer_size = CONFIG_RECEIVE_BUFFER_SIZE;
        constexpr static uint32_t read_timeout = CONFIG_READ_TIMEOUT;
    }
}
