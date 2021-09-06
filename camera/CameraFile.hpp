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

#ifndef RCKAM_CAMERA_CAMERA_FILE_HPP
#define RCKAM_CAMERA_CAMERA_FILE_HPP

#include <tuple>
#include <iostream>
#include <gphoto2/gphoto2-file.h>

namespace rckam
{
namespace camera
{

class CameraFile
{
public:
  CameraFile();
  ~CameraFile();
  operator ::CameraFile *() {return cameraFile_;}
  std::tuple<const char *, unsigned long int> getDataAndSize();
private:
  /// the underlying gphoto2 camera
  ::CameraFile *cameraFile_;
  /// reference count
  unsigned refCount_;
};

std::ostream &operator <<(std::ostream &os, CameraFile &cameraFile);

} // namespace camera
} // namespace rckam

#endif // #ifdef RCKAM_CAMERA_CAMERA_FILE_HPP

