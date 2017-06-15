#include "basic_serial.hpp"

#include <stdexcept>

namespace serial
{
//#####################################################################################################################
    basic_serial::basic_serial(asio::io_service& service, std::string const& deviceName)
        : con_{service, deviceName}
        , read_buffer_(4096)
        , write_buffer_{}
    {
        if (!con_.is_open())
            throw std::runtime_error("Failed to open serial port");

    }
//---------------------------------------------------------------------------------------------------------------------
    void basic_serial::configure(unsigned int baud, serial_config config)
    {
        con_.set_option(boost::asio::serial_port_base::baud_rate{baud});
        con_.set_option(config.parity);
        con_.set_option(config.character_size);
        con_.set_option(config.flow_control);
        con_.set_option(config.stop_bits);
    }
//---------------------------------------------------------------------------------------------------------------------
    void basic_serial::close()
    {
        con_.cancel();
        con_.close();
    }
//---------------------------------------------------------------------------------------------------------------------
    basic_serial::~basic_serial()
    {
        close();
    }
//#####################################################################################################################
}
