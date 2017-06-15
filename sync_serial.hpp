#pragma once

#include "basic_serial.hpp"

namespace serial
{
    class synchronous_serial : public basic_serial
    {
    public:
        synchronous_serial(asio::io_service& service, std::string const& deviceName);

        void write(std::string const& str);
        void write(std::basic_string <unsigned char> const& str);

        std::string read();

    private:
    };
}
