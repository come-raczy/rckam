 /**
  ** rckam
  ** Copyright (c) 2020-2021 Come Raczy
  ** All rights reserved.
  **
  ** This software is provided under the terms and conditions of the
  ** GNU AFFERO GENERAL PUBLIC LICENSE
  **
  ** You should have received a copy of the
  ** GNU AFFERO GENERAL PUBLIC LICENSE
  ** along with this program. If not, see
  ** <https://fsf.org/>
  **/

#include "UserspaceDevice.hpp"

#include <libudev.h>
#include <boost/format.hpp>

#include "common/Exceptions.hpp"

namespace rckam
{
namespace camera
{

// UserspaceDevice::UserspaceDevice(const std::string &path)
// {
//   // TODO: implement
// }

UserspaceDevice::UserspaceDevice(const std::string &devPath, const std::string devType, const std::string &devNode,
                  const std::string &subsystem, const std::string sysPath, const std::string &sysName,
                  const std::string &sysNum, const bool isInitialized, const std::vector<std::string> &devlinks,
                  const std::vector<Property> &properties, const std::vector<std::string> &tags,
                  const std::vector<Sysattr> &sysattrs)
: devPath_(devPath)
, devType_(devType)
, devNode_(devNode)
, subsystem_(subsystem)
, sysPath_(sysPath)
, sysName_(sysName)
, sysNum_(sysNum)
, isInitialized_(isInitialized)
, devlinks_(devlinks)
, properties_(properties)
, tags_(tags)
, sysattrs_(sysattrs)
{
}

UserspaceDevice::~UserspaceDevice()
{
}

std::vector<std::string> getDevlinks(struct udev_device* udevDevice)
{
  std::vector<std::string> devlinks;
  struct udev_list_entry *list =  udev_device_get_devlinks_list_entry(udevDevice);
  struct udev_list_entry* entry;
  udev_list_entry_foreach(entry, list)
  {
    const char *name = udev_list_entry_get_name(entry);
    // value should be null
    // const char *value = udev_list_entry_get_value(le);
    devlinks.emplace_back(name);
  }
  return devlinks;
}

std::vector<UserspaceDevice::Property> getProperties(struct udev_device* udevDevice)
{
  std::vector<UserspaceDevice::Property> properties;
  struct udev_list_entry *list = udev_device_get_properties_list_entry(udevDevice);
  struct udev_list_entry* entry;
  udev_list_entry_foreach(entry, list)
  {
    const char *name = udev_list_entry_get_name(entry);
    const char *value = udev_list_entry_get_value(entry);
    properties.emplace_back(name, value);
  }
  return properties;
}

std::vector<std::string> getTags(struct udev_device* udevDevice)
{
  std::vector<std::string> tags;
  struct udev_list_entry *list = udev_device_get_tags_list_entry(udevDevice);
  struct udev_list_entry* entry;
  udev_list_entry_foreach(entry, list)
  {
    const char *name = udev_list_entry_get_name(entry);
    // value should be null
    // const char *value = udev_list_entry_get_value(le);
    tags.emplace_back(name);
  }
  return tags;
}

std::vector<UserspaceDevice::Sysattr> getSysattrs(struct udev_device* udevDevice)
{
  std::vector<UserspaceDevice::Sysattr> sysattrs;
  struct udev_list_entry *list = udev_device_get_tags_list_entry(udevDevice);
  struct udev_list_entry* entry;
  udev_list_entry_foreach(entry, list)
  {
    const char *name = udev_list_entry_get_name(entry);
    const char *value = (name? udev_device_get_sysattr_value(udevDevice, name):nullptr);
    sysattrs.emplace_back(name, value);
  }
  return sysattrs;
}

std::vector<UserspaceDevice> UserspaceDevice::matchProperty(const Property &property)
{
  using common::RckamException;
  struct udev *udev_ = udev_new();
  if (nullptr == udev_)
  {
    const auto message = boost::format("ERROR: failed to allocate a new udev context object") % errno % strerror(errno);
    BOOST_THROW_EXCEPTION(RckamException(message.str()));
  }
  struct udev_enumerate* enumerate = udev_enumerate_new(udev_);
  if(nullptr == enumerate)
  {
    const auto message = boost::format("ERROR: failed to create a udev enumerate object") % errno % strerror(errno);
    BOOST_THROW_EXCEPTION(RckamException(message.str()));
  }
  udev_enumerate_scan_devices(enumerate);
  struct udev_list_entry* deviceList;
  struct udev_list_entry* listEntry;
  deviceList = udev_enumerate_get_list_entry(enumerate);
  std::vector<UserspaceDevice> devices;
  udev_list_entry_foreach(listEntry, deviceList)
  {
    const char* devPath = udev_list_entry_get_name(listEntry);
    struct udev_device* udevDevice = udev_device_new_from_syspath(udev_, devPath);
    const char *value = udev_device_get_property_value(udevDevice, property.first.c_str());
    if (value && (property.second != value))
    {
      const char* devType = udev_device_get_devtype(udevDevice);
      const char* devNode = udev_device_get_devnode(udevDevice);
      const char *subsystem = udev_device_get_subsystem(udevDevice);
      const char *sysPath = udev_device_get_syspath(udevDevice);
      const char *sysName = udev_device_get_sysname(udevDevice);
      const char *sysNum = udev_device_get_sysnum(udevDevice);
      const int isInitialized = udev_device_get_is_initialized(udevDevice);
      std::vector<std::string> devlinks = getDevlinks(udevDevice);
      std::vector<Property> properties = getProperties(udevDevice);
      std::vector<std::string> tags = getTags(udevDevice);
      std::vector<Sysattr> sysattrs = getSysattrs(udevDevice);
      devices.emplace_back(devPath, devType, devNode, subsystem, sysPath, sysName, sysNum, isInitialized, devlinks, properties, tags, sysattrs);
    }
  }
  return devices;
}

} // namespace camera
} // namespace rckam

