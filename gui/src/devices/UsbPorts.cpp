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

#include "devices/UsbPorts.hpp"

#include <boost/format.hpp>

#include "common/Exceptions.hpp"

namespace rckam
{
namespace devices 
{

/**
 * \brief Exception thrown when there are problems with an USB operations
 */
class UsbException: public common::RckamException
{
public:
    UsbException(libusb_error errorNumber, const std::string &message);
};

UsbException::UsbException(libusb_error errorNumber, const std::string &message)
    : RckamException(errorNumber, message  + ": " + libusb_error_name(errorNumber) + ": " + libusb_strerror(errorNumber))
{
}

static struct udev *checkUdev(struct udev *udev)
{
    if (!udev) BOOST_THROW_EXCEPTION(common::RckamException("Failed to initialize udev"));
    return udev;
}

UsbPorts::UsbPorts()
    : context(nullptr, libusb_exit)
    , udev(udev_new(), udev_unref)
    , hwdb(udev_hwdb_new(checkUdev(udev.get())), udev_hwdb_unref)
{
    libusb_context *tmpContext;
    const auto ret = libusb_init(&tmpContext);
    if (0 != ret)
    {
        BOOST_THROW_EXCEPTION(UsbException(static_cast<libusb_error>(ret), "Failed to initialize libusb context"));
    }
    context.reset(tmpContext);
}

const char *UsbPorts::hwdbGet(const char *modalias, const char *key)
{
        struct udev_list_entry *entry;

        udev_list_entry_foreach(entry, udev_hwdb_get_properties_list_entry(hwdb.get(), modalias, 0))
                if (strcmp(udev_list_entry_get_name(entry), key) == 0)
                        return udev_list_entry_get_value(entry);

        return NULL;
}

std::string UsbPorts::getVendorString(uint16_t vid)
{
    auto modalias = boost::format("usb:v%04X*");
    const auto ret = hwdbGet((modalias % vid).str().c_str(), "ID_VENDOR_FROM_DATABASE");
    return NULL == ret ? std::string("Unknown vendor") : std::string(ret);
}

std::string UsbPorts::UsbPorts::getProductString(uint16_t vid, uint16_t pid)
{
    auto modalias = boost::format("usb:v%04Xp%04X*");
    const auto ret = hwdbGet((modalias % vid % pid).str().c_str(), "ID_MODEL_FROM_DATABASE");
    return NULL == ret ? std::string("Unknown product") : std::string(ret);
}

std::vector<DeviceInfo> UsbPorts::listDevices()
{
    libusb_device   **devices = NULL;
    const auto numberOfDevices = libusb_get_device_list(context.get(), &devices);
    if (0 > numberOfDevices)
    {
       BOOST_THROW_EXCEPTION(UsbException(static_cast<libusb_error>(numberOfDevices), "Failed to get list of USB devices"));
    }
    //std::vector<libusb_device_descriptor> deviceDescriptors(numberOfDevices);
    std::vector<DeviceInfo> result(numberOfDevices);
    for (unsigned int i = 0; numberOfDevices > i; ++i)
    {
        result[i].busNumber = libusb_get_bus_number(devices[i]);
        result[i].portNumber = libusb_get_port_number(devices[i]);
        result[i].address = libusb_get_device_address(devices[i]);
        libusb_get_device_descriptor(devices[i], &result[i].descriptor);
        result[i].vendor = getVendorString(result[i].descriptor.idVendor);
        result[i].product = getProductString(result[i].descriptor.idVendor, result[i].descriptor.idProduct);
    }
    libusb_free_device_list(devices, 0);
    return result;
}

} // namespace devices
} // namespace rckam

#if 0

///////////////////////////////
gp_port_library_lis:

static std::vector<std::string> UsbPorts::list()

        libusb_context  *ctx;
        libusb_device   **devs = NULL;
        int             nrofdevs = 0;
        struct libusb_device_descriptor *descs;

        C_LIBUSB (libusb_init (&ctx), GP_ERROR_IO);

        nrofdevs = libusb_get_device_list (ctx, &devs);
        if (!nrofdevs) {
                libusb_exit (ctx); /* should free all stuff above */
                goto nodevices;
        }

        C_MEM (descs = calloc (nrofdevs, sizeof(descs[0])));
        for (i=0;i<nrofdevs;i++)
                LOG_ON_LIBUSB_E (libusb_get_device_descriptor(devs[i], &descs[i]));

        for (d = 0; d < nrofdevs; d++) {
                /* Devices which are definitely not cameras. */
                if (    (descs[d].bDeviceClass == LIBUSB_CLASS_HUB)             ||
                        (descs[d].bDeviceClass == LIBUSB_CLASS_HID)             ||
                        (descs[d].bDeviceClass == LIBUSB_CLASS_PRINTER) ||
                        (descs[d].bDeviceClass == LIBUSB_CLASS_COMM)    ||
                        (descs[d].bDeviceClass == 0xe0) /* wireless / bluetooth */
                )
                        continue;
                /* excepts HUBs, usually the interfaces have the classes, not
                 * the device */
                unknownint = 0;
                for (i = 0; i < descs[d].bNumConfigurations; i++) {
                        struct libusb_config_descriptor *config;

                        if (LOG_ON_LIBUSB_E (libusb_get_config_descriptor (devs[d], i, &config))) {
                                unknownint++;
                                continue;
                        }
                        for (i1 = 0; i1 < config->bNumInterfaces; i1++)
                                for (i2 = 0; i2 < config->interface[i1].num_altsetting; i2++) {
                                        const struct libusb_interface_descriptor *intf = &config->interface[i1].altsetting[i2];
                                        if (    (intf->bInterfaceClass == LIBUSB_CLASS_HID)     ||
                                                (intf->bInterfaceClass == LIBUSB_CLASS_PRINTER) ||
                                                (intf->bInterfaceClass == LIBUSB_CLASS_COMM)    ||
                                                (intf->bInterfaceClass == 0xe0) /* wireless/bluetooth*/
                                        )
                                                continue;
                                        unknownint++;
                                }
                        libusb_free_config_descriptor (config);
                }
                /* when we find only hids, printer or comm ifaces  ... skip this */
                if (!unknownint)
                        continue;
                /* Note: We do not skip USB storage. Some devices can support both,
                 * and the Ricoh erronously reports it.
                 */
                nrofdevices++;
        }
        for (d = 0; d < nrofdevs; d++) {
                char path[200];

                /* Devices which are definitely not cameras. */
                if (    (descs[d].bDeviceClass == LIBUSB_CLASS_HUB)             ||
                        (descs[d].bDeviceClass == LIBUSB_CLASS_HID)             ||
                        (descs[d].bDeviceClass == LIBUSB_CLASS_PRINTER) ||
                        (descs[d].bDeviceClass == LIBUSB_CLASS_COMM)
                )
                        continue;
                /* excepts HUBs, usually the interfaces have the classes, not
                 * the device */
                unknownint = 0;
                for (i = 0; i < descs[d].bNumConfigurations; i++) {
                        struct libusb_config_descriptor *config;

                        if (LOG_ON_LIBUSB_E (libusb_get_config_descriptor (devs[d], i, &config))) {
                                unknownint++;
                                continue;
                        }
                        for (i1 = 0; i1 < config->bNumInterfaces; i1++)
                                for (i2 = 0; i2 < config->interface[i1].num_altsetting; i2++) {
                                        const struct libusb_interface_descriptor *intf = &config->interface[i1].altsetting[i2];
                                        if (    (intf->bInterfaceClass == LIBUSB_CLASS_HID)     ||
                                                (intf->bInterfaceClass == LIBUSB_CLASS_PRINTER) ||
                                                (intf->bInterfaceClass == LIBUSB_CLASS_COMM))
                                                continue;
                                        unknownint++;
                                }
                        libusb_free_config_descriptor (config);
                }
                /* when we find only hids, printer or comm ifaces  ... skip this */
                if (!unknownint)
                        continue;
                /* Note: We do not skip USB storage. Some devices can support both,
                 * and the Ricoh erronously reports it.
                 */
                C_GP (gp_port_info_new (&info));
                gp_port_info_set_type (info, GP_PORT_USB);
                gp_port_info_set_name (info, "Universal Serial Bus");
                snprintf (path,sizeof(path), "usb:%03d,%03d",
                        libusb_get_bus_number (devs[d]),
                        libusb_get_device_address (devs[d])
                );
                gp_port_info_set_path (info, path);
                C_GP (gp_port_info_list_append (list, info));
        }

        libusb_free_device_list (devs, 1);
        libusb_exit (ctx); /* should free all stuff above */
        free (descs);
nodevices:
        /* This will only be added if no other device was ever added.
         * Users doing "usb:" usage will enter the regular expression matcher case. */
        if (nrofdevices == 0) {
                C_GP (gp_port_info_new (&info));
                gp_port_info_set_type (info, GP_PORT_USB);
                gp_port_info_set_name (info, "Universal Serial Bus");
                gp_port_info_set_path (info, "usb:");
                C_GP (gp_port_info_list_append (list, info));
        }
        return (GP_OK);




//////////////////////////////////
_get_portinfo_list:{
        int count, result;
        GPPortInfoList *list = NULL;

        if (p->portinfo_list)
                return;

        if (gp_port_info_list_new (&list) < GP_OK)
                return;
        result = gp_port_info_list_load (list);
        if (result < 0) {
                gp_port_info_list_free (list);
                return;
        }
        count = gp_port_info_list_count (list);
        if (count < 0) {
                gp_port_info_list_free (list);
                return;
        }
        p->portinfo_list = list;
        return;

}


} // namespace devices
} // namespace rckam

#endif

