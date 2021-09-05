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

#include "CameraController.hpp"

#include <iostream>

namespace rckam
{
namespace camera
{

CameraController::CameraController(const char * const model, const char * const port, Gphoto2Context &context)
: camera_(model, port, context)
{
}

CameraController::~CameraController()
{
}

void CameraController::run()
{
  for (unsigned int i = 0; 100 > i; ++i)
  {
    CameraFile cameraFile;
    const auto size = camera_.capture(cameraFile);
    std::cerr << "capture completed. Received " << size << " bytes" << std::endl;
  }
}

} // namespace camera
} // namespace rckam

