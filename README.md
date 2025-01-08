# About

A tool for viewing and setting the bluetooth address
a sixaxis controller is currently paired with.


# Dependencies

HID API (https://github.com/libusb/hidapi)


# Supported Platforms

* Windows
* Mac
* Linux


# Building

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ ./bin/sixaxispairer
    $ ./bin/sixaxispairer xx:xx:xx:xx:xx:xx
