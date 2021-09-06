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

#ifndef RCKAM_CAMERA_CAMERA_CONTROLLER_HPP
#define RCKAM_CAMERA_CAMERA_CONTROLLER_HPP

#include "Gphoto2Context.hpp"
#include "Camera.hpp"
#include "DataSocket.hpp"
#include "CommunicationSocket.hpp"

namespace rckam
{
namespace camera
{

class CameraController
{
public:
  CameraController(const char *model, const char *port, Gphoto2Context &context);
  ~CameraController();
  /// operate the camera
  void run();
private:
  /// the underlying gphoto2 camera
  Camera camera_;
  /// the socket to transmit data (image previews)
  DataSocket dataSocket_;
  /// the socket for communication and synchronization
  CommunicationSocket communicationSocket_;
};

} // namespace camera
} // namespace rckam

#endif // #ifdef RCKAM_CAMERA_CAMERA_CONTROLLER_HPP

