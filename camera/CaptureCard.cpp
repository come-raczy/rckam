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

#include "CaptureCard.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <linux/media.h>
#include <boost/format.hpp>
#include <map>
#include <string>
#include <iostream>

#include "common/Exceptions.hpp"
#include "UserspaceDevice.hpp"

namespace rckam
{
namespace camera
{

// UserspaceDevice::UserspaceDevice(const std::string &path)
// {
//   // TODO: implement
// }

CaptureCard::CaptureCard(const std::string &busInfo, const std::string &card)
: busInfo_(busInfo)
, card_(card)
{
}

CaptureCard::~CaptureCard()
{
}

void CaptureCard::addDevNode(const std::string &devNode)
{
  devNodes_.emplace_back(devNode);
}

std::vector<CaptureCard> CaptureCard::detect()
{
  auto userspaceDevices = UserspaceDevice::matchProperty({"MAJOR", "81"});
  std::sort(userspaceDevices.begin(), userspaceDevices.end(), [](UserspaceDevice &l, UserspaceDevice &r) {return (l.sysType() < r.sysType()) || ((l.sysType() == r.sysType()) && (l.sysNum() < r.sysNum()));});
  // TODO: remove devices that are links to other devices already in the list
  std::vector<CaptureCard> captureCards;
  for (auto &device: userspaceDevices)
  {
    int fd = open(device.devNode().c_str(), O_RDWR);
    if (fd < 0)
      continue;
    std::string bus_info;
    std::string card;
    struct v4l2_capability vcap;
    int err = ioctl(fd, VIDIOC_QUERYCAP, &vcap);
    if (err)
    {
      struct media_device_info mdi;
      err = ioctl(fd, MEDIA_IOC_DEVICE_INFO, &mdi);
      if (!err)
      {
        if (mdi.bus_info[0])
          bus_info = mdi.bus_info;
        else
          bus_info = std::string("platform:") + mdi.driver;
        if (mdi.model[0])
          card = mdi.model;
        else
          card = mdi.driver;
      }
    }
    else
    {
      bus_info = reinterpret_cast<const char *>(vcap.bus_info);
      card = reinterpret_cast<const char *>(vcap.card);
    }
    close(fd);
    if (err)
      continue;
    const auto found = std::find_if(captureCards.begin(), captureCards.end(), [&bus_info] (const CaptureCard &card) {return card.busInfo() == bus_info;});
    if (captureCards.end() == found)
    {
      captureCards.emplace_back(bus_info, card);
      captureCards.back().addDevNode(device.devNode());
    }
    else
    {
      found->addDevNode(device.devNode());
    }
  }
  return captureCards;
}

} // namespace camera
} // namespace rckam

