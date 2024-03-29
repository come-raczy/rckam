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

#ifndef RCKAM_SERVER_CAMERA_HPP
#define RCKAM_SERVER_CAMERA_HPP

#include <string>

#include <boost/core/noncopyable.hpp>
#include <gphoto2/gphoto2-camera.h>

#include "Gphoto2Context.hpp"
#include "CameraFile.hpp"

namespace rckam
{
namespace server
{
/**
 ** \brief Encapsulation of the gphoto2 Camera concept and relevant functions
 **
 ** Can't be copied as the destructor does call the functions gp_camera_exit
 ** and g_camera_free on the underlying gphoto2 camera pointer.
 **/
class Camera: boost::noncopyable
{
public:
  Camera(Gphoto2Context &context, const char *model = nullptr, const char *port = nullptr);
  ~Camera();
  /// capture an image into the given camera file and returtn the size
  unsigned long int capturePreview(CameraFile &cameraFile);
  /// set the abilities for the specified model
  void setAbilities(const char *model);
  /// set the port info for the specified port
  void setPortInfo(const char *port);
private:
  Gphoto2Context *context_;
  /// the underlying gphoto2 camera
  ::Camera *camera_;
  /// the underlying gphoto2 camera abilities list
  static CameraAbilitiesList *abilitiesList(Gphoto2Context &context);
  /// get the index of the driver for a given camera model
  static int driverIndex(const char *model, Gphoto2Context &context);
  /// underlying gphoto2 port info
  GPPortInfoList *portInfoList();
};

} // namespace server
} // namespace rckam

#endif // #ifdef RCKAM_SERVER_CAMERA_HPP

