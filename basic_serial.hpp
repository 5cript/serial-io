#pragma once

#include "net_core.hpp"

namespace serial
{
    struct serial_config
    {
        asio::serial_port_base::parity parity {asio::serial_port_base::parity::none};
        asio::serial_port_base::character_size character_size {8};
        asio::serial_port_base::flow_control flow_control {asio::serial_port_base::flow_control::none};
        asio::serial_port_base::stop_bits stop_bits {asio::serial_port_base::stop_bits::one};
    };

    class basic_serial
    {
    public:
        /**
         *  Construct a serial connection object.
         */
        basic_serial(asio::io_service& service, std::string const& deviceName);

        /**
         *  Destructor
         */
        virtual ~basic_serial();

        /**
         *  Close the serial connection.
         */
        void close();

        /**
         *  Copying is disabled
         */
        basic_serial& operator=(basic_serial const&) = delete;
        basic_serial(basic_serial const&) = delete;

        /**
         *  Configure the serial connection.
         *
         *  @param baud The baud rate.
         *  @param config Other configuration options. @see serial_config
         */
        void configure(unsigned int baud, serial_config config = {});

    protected:
        asio::serial_port con_;
        std::vector <unsigned char> read_buffer_;
        std::vector <unsigned char> write_buffer_;
    };
}
