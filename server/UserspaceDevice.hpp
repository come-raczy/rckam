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

#ifndef RCKAM_SERVER_USERSPACE_DEVICE_HPP
#define RCKAM_SERVER_USERSPACE_DEVICE_HPP

#include <vector>
#include <string>

namespace rckam
{
namespace server
{

/**
 ** \brief simple encapsulation of devices as seen through libudev
 **/
class UserspaceDevice
{
public:
  typedef std::pair<std::string, std::string> Property;
  typedef std::pair<std::string, std::string> Sysattr;
  /// create a device from a path (e.g. "/dev/video0")
  // UserspaceDevice(const std::string &path);
  /// create a device from all its data
  UserspaceDevice(const std::string &devPath, const std::string devType, const std::string &devNode,
                  const std::string &subsystem, const std::string sysPath, const std::string &sysName,
                  const std::string &sysNum, bool isInitialized, const std::vector<std::string> &devlinks,
                  const std::vector<Property> &properties, const std::vector<std::string> &tags,
                  const std::vector<Sysattr> &sysattrs);
  /// this class is meant to be specialized
  virtual ~UserspaceDevice();
  /**
   ** \brief list all devices that match the required property
   **
   ** Useful propertire: MAJOR, DEVLINKS, DEVNAME, DEVPATH, 
   **
   ** Examples:
   **
   ** list all devices with MAJOR 81 (video devices):
   ** const auto videoDevices = UserspaceDevice::matchProperty({"MAJOR", "81"});
   **
   ** list all video capture devices:
   ** const auto videoCaptureDevices = UserspaceDevice::matchProperty({"ID_V4L_CAPABILITIES", ":capture:"});
   **/
  static std::vector<UserspaceDevice> matchProperty(const Property &property);
  /// get the value of the specified property - an empty string if not found
  const std::string &getProperty(const std::string &property);
  /// path of the device - e.g. "/devices/platform/scb/fd500000.pcie/..../video0"
  const std::string &devPath() const {return devPath_;}
  /// path of the device - e.g. "/dev/video0"
  const std::string &devNode() const {return devNode_;}
  /// name of the device - e.g. "video0"
  const std::string &sysName() const {return sysName_;}
  /// numeric index of the device - e.g. "0" for "video0"
  unsigned sysNum() const;
  const std::string &sysNumString() const {return sysNum_;}
  /// the non-numeric prefix of sysName - e.g. "video" for "video0"
  std::string sysType() const;
protected:
  std::string devPath_;
  std::string devType_;
  std::string devNode_;
  std::string subsystem_;
  std::string sysPath_;
  std::string sysName_;
  std::string sysNum_;
  bool isInitialized_;
  std::vector<std::string> devlinks_;
  std::vector<Property> properties_;
  std::vector<std::string> tags_;
  std::vector<Sysattr> sysattrs_;
};

} // namespace server
} // namespace rckam

#endif // #ifndef RCKAM_SERVER_USERSPACE_DEVICE_HPP

