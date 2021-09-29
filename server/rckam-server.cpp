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

#include <memory>
#include <iostream>
#include <gphoto2/gphoto2-camera.h>

#include "RckamCameraOptions.hpp"
#include "Gphoto2Context.hpp"
#include "CameraList.hpp"
#include "CameraController.hpp"
#include "CaptureCard.hpp"
#include "common/Rckam.hpp"

int main(int argc, char *argv[])
{
  namespace rt = rckam::server;
  // process commandline options
  rt::RckamCameraOptions options;
  const auto action = options.parse(argc, argv);
  if (rt::RckamCameraOptions::HELP == action)
  {
    std::cout << options.usage() << std::endl;
    return 0;
  }
  else if (rt::RckamCameraOptions::VERSION == action)
  {
    std::cout << options.version() << std::endl;
    return 0;
  }
  else if (rt::RckamCameraOptions::ABORT == action)
  {
    return -1;
  }
  assert(rt::RckamCameraOptions::RUN == action);
  rckam::server::Gphoto2Context context;
  try
  {

const auto captureCards = rckam::server::CaptureCard::detect();
for (const auto &card: captureCards)
{
  std::cerr << card.card() << " (" << card.busInfo() << ")" << std::endl;
  for (const auto &device: card.devNodes())
  {
    std::cerr << "    " << device << std::endl;
  }
}

    rckam::server::CameraList cameraList(context);
    if (0 < cameraList.count())
    {
      for (unsigned i = 0; i < cameraList.count(); ++i)
      {
        std::cerr << "   - " << cameraList.model(i) << ": " << cameraList.port(i) << std::endl;
      }
      const unsigned selected = 0;
      const auto model = cameraList.model(selected);
      const auto usbPort = cameraList.port(selected);
      //rckam::camera::Camera camera(cameraList.name(selected), cameraList.value(selected), context);
      rckam::server::CameraController controller(model, usbPort, rckam::common::dataPort, rckam::common::controlPort, context);
      controller.run();
    }
    else
    {
      std::cerr << "WARNING: no camera detected. Exiting." << std::endl;
    }
  }
  catch (...)
  {
  }
}
