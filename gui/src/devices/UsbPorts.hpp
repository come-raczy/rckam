/**
 ** rckam: a Qt remote control for digital cameras
 **
 ** Copyright (C) <year>  <name of author>
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Affero General Public License as
 ** published by the Free Software Foundation, either version 3 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Affero General Public License for more details.
 **
 ** You should have received a copy of the GNU Affero General Public License
 ** along with this program.  If not, see <https://www.gnu.org/licenses/>.
 **/

#ifndef DEVICES_USB_PORTS_HPP
#define DEVICES_USB_PORTS_HPP

#include <memory>
#include <vector>
#include <string>

// silence zero-sized arrays
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wpedantic"
//#include <libusb.h>
//#pragma GCC diagnostic pop

//#include <libudev.h>

namespace rckam
{
namespace devices 
{

#if 0
struct DeviceInfo
{
    uint8_t busNumber;
    uint8_t portNumber;
    uint8_t address;
    libusb_device_descriptor descriptor;
    std::string vendor;
    std::string product;
};

class UsbPorts
{
    UsbPorts(const UsbPorts&) = delete;
    UsbPorts& operator =(const UsbPorts&) = delete;
public:
    UsbPorts();
    std::vector<DeviceInfo> listDevices();
private:
    std::unique_ptr<libusb_context, decltype(libusb_exit) *> context;
    std::unique_ptr<struct udev, decltype(udev_unref) *> udev;
    std::unique_ptr<struct udev_hwdb, decltype(udev_hwdb_unref) *> hwdb;
    const char *hwdbGet(const char *modalias, const char *key);
    std::string getVendorString(uint16_t vid);
    std::string getProductString(uint16_t vid, uint16_t pid);
};
#endif

} // namespace devices
} // namespace rckam

#endif // #ifndef DEVICES_USB_PORTS_HPP

