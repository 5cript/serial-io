#pragma once

#include "basic_serial.hpp"

namespace serial
{
    class asynchronous_serial;

    using custom_callback = std::function <void(boost::system::error_code /* ec */)>;
    using read_callback = custom_callback;
    using write_callback = custom_callback;
    using parse_callback = custom_callback;
    using error_callback = std::function <void(asynchronous_serial* /*connection*/, boost::system::error_code /*ec*/)>;

    class asynchronous_serial : public basic_serial
    {
    public:
        asynchronous_serial(asio::io_service& service, std::string const& deviceName);

        /**
         *  Read more bytes into the buffer. This will overwrite the buffer.
         */
        void read();

        /**
         *  This function writes all the bytes from begin to end onto the stream.
         *  The handler function is called when the write operation completes.
         *  Do not (!) call write while another write operation is in progress!
         */
        template <typename IteratorT>
        void write(IteratorT&& begin, IteratorT&& end, write_callback handler)
        {
            write_buffer_.resize(end - begin);
            std::copy(begin, end, write_buffer_.begin());

            boost::asio::async_write(con_, boost::asio::buffer(write_buffer_),
                [this, handler](boost::system::error_code ec, std::size_t)
                {
                    handler(ec);
                }
            );
        }

        /**
         *  This function writes the string onto the tcp stream.
         *  The handler function is called when the write operation completes.
         *  Do not (!) call write while another write operation is in progress!
         */
        void write(std::string const& string, write_callback handler)
        {
            write(std::begin(string), std::end(string), handler);
        }

        void write(std::basic_string <unsigned char> const& string, write_callback handler)
        {
            write(std::begin(string), std::end(string), handler);
        }

        std::basic_string <unsigned char> extractRead();

        /**
         *  Sets the read callback, which is called when a read operation finishes.
         */
        void set_read_callback(read_callback const& new_read_callback);

    private:
        void do_read();

        /**
        *  Checks the deadline_timer and possibly terminates the connection if the timeout was reached.
        *  Any IO will reset the deadline
        */
        void check_deadline(boost::asio::deadline_timer* timer, boost::system::error_code const& ec);

    private:
        read_callback read_callback_inst_;
        boost::asio::deadline_timer read_timeout_timer_;
        std::size_t bytes_ready_;
    };
}
