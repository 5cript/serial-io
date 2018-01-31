#include "basic_serial.hpp"

#include <stdexcept>

#include <string>
using namespace std::string_literals;

namespace serial
{
//#####################################################################################################################
    basic_serial::basic_serial(asio::io_service& service, std::string const& deviceName)
        : con_{service}
        , read_buffer_(2048)
        , write_buffer_{}
    {
        boost::system::error_code ec;
        con_.open(deviceName, ec);
    }
//---------------------------------------------------------------------------------------------------------------------
    void basic_serial::configure(unsigned int baud, serial_config config)
    {
        if (con_.is_open())
        {
            con_.set_option(boost::asio::serial_port_base::baud_rate{baud});
            con_.set_option(config.parity);
            con_.set_option(config.character_size);
            con_.set_option(config.flow_control);
            con_.set_option(config.stop_bits);
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    bool basic_serial::is_open() const noexcept
    {
        return con_.is_open();
    }
//---------------------------------------------------------------------------------------------------------------------
    void basic_serial::close()
    {
        if (con_.is_open())
        {
            con_.cancel();
            con_.close();
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    basic_serial::~basic_serial()
    {
        close();
    }
//#####################################################################################################################
}
