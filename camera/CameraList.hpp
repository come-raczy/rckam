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

#ifndef RCKAM_CAMERA_CAMERA_LIST_HPP
#define RCKAM_CAMERA_CAMERA_LIST_HPP

#include <gphoto2/gphoto2-list.h>

#include "Gphoto2Context.hpp"

namespace rckam
{
namespace camera
{

class CameraList
{
public:
  CameraList(Gphoto2Context &context);
  ~CameraList();
  unsigned count() const {return count_;}
  const char *name(unsigned i) const;
  const char *value(unsigned i) const;
private:
  /// the underlying gphoto2 CameraList
  ::CameraList *cameraList_;
  /// number of cameras in the list
  unsigned count_;
};

} // namespace camera
} // namespace rckam

#endif // #ifdef RCKAM_CAMERA_CAMERA_LIST_HPP

