#include "async_serial.hpp"

namespace serial
{
//#####################################################################################################################
    asynchronous_serial::asynchronous_serial(asio::io_service& service, std::string const& deviceName)
        : basic_serial{service, deviceName}
        , read_callback_inst_{}
        , read_timeout_timer_{service}
        , bytes_ready_{0}
    {
        // this will ensure, that the timer does not fire right away when it it started
        // the timer is started on read, but the timeout will be set later in every do_read cycle.
        read_timeout_timer_.expires_at(boost::posix_time::pos_infin);
    }
//---------------------------------------------------------------------------------------------------------------------
    void asynchronous_serial::read()
    {
        read_timeout_timer_.async_wait(
            [this](boost::system::error_code const& ec)
            {
                check_deadline(&read_timeout_timer_, ec);
            }
        );

        do_read();
    }
//---------------------------------------------------------------------------------------------------------------------
    std::basic_string <unsigned char> asynchronous_serial::extractRead()
    {
        auto bytes_ready_cpy = bytes_ready_;
        bytes_ready_ = 0;
        return {std::begin(read_buffer_), std::begin(read_buffer_) + bytes_ready_cpy};
    }
//---------------------------------------------------------------------------------------------------------------------
    void asynchronous_serial::set_read_callback(read_callback const& new_read_callback)
    {
        read_callback_inst_ = new_read_callback;
    }
//---------------------------------------------------------------------------------------------------------------------
    void asynchronous_serial::clear_read_buffer()
    {
        bytes_ready_ = 0;
    }
//---------------------------------------------------------------------------------------------------------------------
    void asynchronous_serial::do_read()
    {
        // sets / resets the timer.
        //read_timeout_timer_.expires_from_now(boost::posix_time::seconds(config::read_timeout));

        con_.async_read_some(boost::asio::buffer(read_buffer_),
            [this](boost::system::error_code ec, std::size_t bytes_transferred)
            {
                if (ec == boost::asio::error::operation_aborted)
                    return;

                bytes_ready_ = bytes_transferred;
                read_timeout_timer_.expires_from_now(boost::posix_time::pos_infin);

                // must get all bytes from the read_buffer_
                read_callback_inst_(ec);
            }
        );
    }
//---------------------------------------------------------------------------------------------------------------------
    std::size_t asynchronous_serial::get_read_count() const
    {
        return bytes_ready_;
    }
//---------------------------------------------------------------------------------------------------------------------
    void asynchronous_serial::check_deadline(boost::asio::deadline_timer* timer, boost::system::error_code const& ec)
    {
        // The operation was aborted. This most likely means, that the connection was terminated.
        // Accessing this from here is unsafe.
        if (ec == boost::asio::error::operation_aborted)
            return;

        //if (ec == boost::system::errc::operation_canceled)
        //    return;

        if (ec && ec != boost::system::errc::operation_canceled)
            return;

        // Check whether the deadline has passed. We compare the deadline against
        // the current time since a new asynchronous operation may have moved the
        // deadline before this actor had a chance to run.
        if (timer->expires_at() <= boost::asio::deadline_timer::traits_type::now())
        {
            // ...
        }
        else
        {
            timer->async_wait(
                [this, timer](boost::system::error_code const& ec)
                {
                    check_deadline(timer, ec);
                }
            );
        }
}
//#####################################################################################################################
}
