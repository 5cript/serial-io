#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

#include "io_context/thread_pooler.hpp"
#include "io_context/managed_io_context.hpp"

#include "async_serial.hpp"

int main()
{
    using namespace serial;

    managed_io_context <thread_pooler> context;

    asynchronous_serial con{*context.get_io_service(), "COM5"};
    con.configure(57600);

    con.set_read_callback([&](auto err) {
        std::cout << "errc: " << err << "\n";
        std::cout << "read something\n";

        auto read = con.extractRead();
        for (auto const& i : read)
        {
            std::cout << std::hex << std::setw(2) << (int)i << ' ';
        }
        std::cout << "\n";
        con.read();
    });

    /*
    con.write("\xAA\x10\x04ECHO", [](auto ec){
        std::cout << "written\n";
    });
    */

    /*
    std::string on = "\xAA\x08";
    std::string off = "\xAA\x09";

    on.resize(3);
    off.resize(3);

    con.write(on, [](auto ec){
        std::cout << "written\n";
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    con.write(off, [](auto ec){
        std::cout << "written\n";
    });
    */

    std::basic_string <unsigned char> echoDevice;
    echoDevice.resize(7);
    echoDevice[0] = 0xAA;
    echoDevice[1] = 0x10;
    echoDevice[2] = 0x04;
    echoDevice[3] = 'E';
    echoDevice[4] = 'C';
    echoDevice[5] = 'H';
    echoDevice[6] = 'O';

    con.write(echoDevice, [&](auto ec){
        std::cout << "written\n";
    });
    con.read();


    std::basic_string <unsigned char> adValue = {0xAA, 0x04, 0x00};
    for (bool toggle = true;; toggle = !toggle)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        con.write(adValue, [](auto ec){
            std::cout << "written\n";
        });
    }


    std::cin.get();

    return 0;
}
