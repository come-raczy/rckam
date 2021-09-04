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

#ifndef RCKAM_CAMERA_CAMERA_HPP
#define RCKAM_CAMERA_CAMERA_HPP

#include <string>

#include <gphoto2/gphoto2-camera.h>

#include "Gphoto2Context.hpp"

namespace rckam
{
namespace camera
{

class Camera
{
public:
  Camera(const char *model, const char *port, Gphoto2Context &context);
  ~Camera();
private:
  /// the underlying gphoto2 camera
  ::Camera *camera_;
  Gphoto2Context *context_;
  /// set the abilities for the specified model
  void setAbilities(const char *model);
  /// set the port info for the specified port
  void setPortInfo(const char *port);
  /// the underlying gphoto2 camera abilities list
  static CameraAbilitiesList *abilitiesList(Gphoto2Context &context);
  /// get the index of the driver for a given camera model
  static int driverIndex(const char *model, Gphoto2Context &context);
  /// underlying gphoto2 port info
  GPPortInfoList *portInfoList();
};

} // namespace camera
} // namespace rckam

#endif // #ifdef RCKAM_CAMERA_CAMERA_HPP

