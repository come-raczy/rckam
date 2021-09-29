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

#include "common/Exceptions.hpp"

namespace rckam
{
namespace server
{

CameraFile::CameraFile()
: cameraFile_(nullptr)
{
  const auto ret1 = gp_file_new(&cameraFile_);
  if (GP_OK != ret1)
  {
    auto message = boost::format("ERROR: Failed to create new camera file: %i") % ret1;
    BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
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

std::tuple<const char *, unsigned long int> CameraFile::getDataAndSize()
{
  const char *data = nullptr;
  unsigned long int size;
  const auto ret1 = gp_file_get_data_and_size(cameraFile_, &data, &size);
  if (GP_OK != ret1)
  {
    auto message = boost::format("ERROR: failed to get data and size from camera file: %i") % ret1;
    BOOST_THROW_EXCEPTION(common::Gphoto2Exception(message.str()));
  }
  return std::tuple<const char *, unsigned long int>(data, size);
}

std::ostream &operator <<(std::ostream &os, CameraFile &cameraFile)
{
  const auto dataAndSize = cameraFile.getDataAndSize();
  const auto data = std::get<0>(dataAndSize);
  const auto size = std::get<1>(dataAndSize);
  os.write(data, size);
  return os;
}

} // namespace server
} // namespace rckam

