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

#include "CameraList.hpp"

#include <cassert>
#include <boost/format.hpp>
#include <gphoto2/gphoto2-camera.h>

#include "common/Exceptions.hpp"

namespace rckam
{
namespace server
{

CameraList::CameraList(Gphoto2Context &context)
: cameraList_(nullptr)
, count_(0)
{
  const auto ret = gp_list_new(&cameraList_);
  if (GP_OK != ret)
  {
    auto message = boost::format("ERROR: failed to greate the camera list: %i") % ret;
    BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
  }
  gp_list_reset (cameraList_);
  const auto ret2 = gp_camera_autodetect (cameraList_, context);
  if (GP_OK > count_)
  {
    auto message = boost::format("ERROR: failed to autodetect cameras: %i") % count_;
    BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
  }
  assert(0 <= ret2);
  count_ = static_cast<unsigned>(ret2);
}

CameraList::~CameraList()
{
  if (nullptr != cameraList_)
  {
    gp_list_free(cameraList_);
    cameraList_ = nullptr;
  }
}

const char * CameraList::model(const unsigned i) const
{
  assert(count_ > i);
  const char * tmp;
  gp_list_get_name  (cameraList_, i, &tmp);
  return tmp;
}

const char * CameraList::port(const unsigned i) const
{
  assert(count_ > i);
  const char * tmp;
  gp_list_get_value  (cameraList_, i, &tmp);
  return tmp;
}

} // namespace server
} // namespace rckam


