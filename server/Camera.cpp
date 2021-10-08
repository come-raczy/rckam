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

#include "Camera.hpp"

#include <iostream>
#include <boost/format.hpp>

#include "common/Exceptions.hpp"

namespace rckam
{
namespace server
{

Camera::Camera(Gphoto2Context &context, const char *model, const char *port)
: context_(&context)
, camera_(nullptr)
{
  const auto ret1 = gp_camera_new (&camera_);
  if (GP_OK != ret1)
  {
    auto message = boost::format("ERROR: failed to initialize a camera object: %i") % ret1;
    BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
  }
  if (nullptr != model)
  {
    setAbilities(model);
  }
  if (nullptr != port)
  {
    setPortInfo(port);
  }
}

Camera::~Camera()
{
  if (nullptr != camera_)
  {
    gp_camera_exit (camera_, *context_);
    gp_camera_free (camera_);
    camera_ = nullptr;
  }
}

unsigned long int Camera::capturePreview(CameraFile &cameraFile)
{
  const auto ret1 = gp_camera_capture_preview(camera_, cameraFile, *context_);
  if (GP_OK != ret1)
  {
    auto message = boost::format("ERROR: failed to capture preview: %i") % ret1;
    BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
  }
  const char *data = nullptr;
  unsigned long int size;
  const auto ret2 = gp_file_get_data_and_size(cameraFile, &data, &size);
  if (GP_OK != ret2)
  {
    auto message = boost::format("ERROR: failed to get data and size from camera file: %i") % ret2;
    BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
  }
  return size;
}

void Camera::setAbilities(const char *model)
{
  if (nullptr == camera_)
  {
    BOOST_THROW_EXCEPTION(common::Gphoto2Exception("ERROR: camera not initialized"));
  }
  const auto index = driverIndex(model, *context_);
  ::CameraAbilities abilities;
  const auto ret1 = gp_abilities_list_get_abilities (abilitiesList(*context_), index, &abilities);
  if (GP_OK > ret1)
  {
    auto message = boost::format("ERROR: failed to get abilities for camera %s: %i") % model % ret1;
    BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
  }
  const auto ret2 = gp_camera_set_abilities (camera_, abilities);
  if (GP_OK > ret2)
  {
    auto message = boost::format("ERROR: failed to set abilities for camera %s: %i") % model % ret2;
    BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
  }
}

CameraAbilitiesList *Camera::abilitiesList(Gphoto2Context &context)
{
  static CameraAbilitiesList *abilitiesList_ = nullptr;
  if (nullptr == abilitiesList_)
  {
    // Load all the camera drivers we have
    const auto ret1 = gp_abilities_list_new (&abilitiesList_);
    if (GP_OK > ret1)
    {
      auto message = boost::format("ERROR: Failed to create a new abilities list: %i") % ret1;
      BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
    }
    auto ret2 = gp_abilities_list_load (abilitiesList_, context);
    if (GP_OK > ret2)
    {
        auto message = boost::format("ERROR: Failed to load abilities list: %i") % ret2;
        BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
    }
  }
  return abilitiesList_;
}

int Camera::driverIndex(const char *model, Gphoto2Context &context)
{
  const auto index = gp_abilities_list_lookup_model (abilitiesList(context), model);
  if (GP_OK > index)
  {
    auto message = boost::format("ERROR: Failed to find driver for camera %s: %i") % model % index;
    BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
  }
  return index;
}

GPPortInfoList *Camera::portInfoList()
{
  static GPPortInfoList *portInfoList_ = nullptr;
  if (nullptr == portInfoList_)
  {
    const auto ret1 = gp_port_info_list_new(&portInfoList_);
    if (GP_OK > ret1)
    {
      auto message = boost::format("ERROR: failed to create new port info list: %i") % ret1;
      BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
    }
    const auto ret2 = gp_port_info_list_load (portInfoList_);
    if (GP_OK > ret2)
    {
      auto message = boost::format("ERROR: failed to load port info list: %i") % ret2;
      BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
    }
    const auto ret3 = gp_port_info_list_count (portInfoList_);
    if (0 > ret3)
    {
        auto message = boost::format("ERROR: failed to count the number of ports: %i") % ret3;
        BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
    }
  }
  return portInfoList_;
}

void Camera::setPortInfo(const char *port)
{
  if (nullptr == camera_)
  {
    BOOST_THROW_EXCEPTION(common::Gphoto2Exception("ERROR: camera not initialized"));
  }
  const auto ret1 = gp_port_info_list_lookup_path (portInfoList(), port);
  if (GP_OK > ret1)
  {
    auto message = boost::format("ERROR: failed to lookup path for port '%s' (use 'gphoto2 --list-ports'): %i") % port % ret1;
    BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
  }
  const auto index = ret1;
  GPPortInfo portInfo;
  const auto ret2 = gp_port_info_list_get_info (portInfoList(), index, &portInfo);
  if (GP_OK > ret2)
  {
    auto message = boost::format("ERROR: failed to get port info for port '%s': %i") % port % ret2;
    BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
  }
  const auto ret3 = gp_camera_set_port_info (camera_, portInfo);
  if (GP_OK > ret3)
  {
    auto message = boost::format("ERROR: failed to set camera port info for port '%s': %i") % port % ret3;
    BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
  }
}

} // namespace server
} // namespace rckam


