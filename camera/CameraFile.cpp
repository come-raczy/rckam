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

#include "CameraFile.hpp"

#include <boost/format.hpp>
#include <gphoto2/gphoto2-port-result.h>

#include "Exceptions.hpp"

namespace rckam
{
namespace camera
{

CameraFile::CameraFile()
: cameraFile_(nullptr)
{
  const auto ret1 = gp_file_new(&cameraFile_);
  if (GP_OK != ret1)
  {
    auto message = boost::format("ERROR: Failed to create new camera file: %i") % ret1;
    BOOST_THROW_EXCEPTION(Gphoto2Exception(message.str()));
  }
}

CameraFile::~CameraFile()
{
  if (nullptr != cameraFile_)
  {
    gp_file_unref(cameraFile_);
    // should be the last reference
    cameraFile_ = nullptr;
  }
}

} // namespace camera
} // namespace rckam
