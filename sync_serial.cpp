#include "sync_serial.hpp"

namespace serial
{
//#####################################################################################################################
    synchronous_serial::synchronous_serial(asio::io_service& service, std::string const& deviceName)
        : basic_serial{service, deviceName}
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    void synchronous_serial::write(std::string const& str)
    {
        write_buffer_.resize(str.length());
        std::copy(std::begin(str), std::end(str), write_buffer_.begin());

        boost::asio::write(con_, boost::asio::buffer(write_buffer_));
    }
//---------------------------------------------------------------------------------------------------------------------
    void synchronous_serial::write(std::basic_string <unsigned char> const& str)
    {
        write_buffer_.resize(str.length());
        std::copy(std::begin(str), std::end(str), write_buffer_.begin());

        boost::asio::write(con_, boost::asio::buffer(write_buffer_));
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string synchronous_serial::read()
    {
        char c;
        boost::asio::read(con_, asio::buffer(&c, 1));
        return {std::begin(read_buffer_), std::end(read_buffer_)};
    }
//#####################################################################################################################
}
