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

#include <iostream>

#include "RckamCameraOptions.hpp"

int main(int argc, char *argv[])
{
  namespace rt = rckam::camera;
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
  try
  {
  }
  catch (...)
  {
  }
}

