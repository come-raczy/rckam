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
#include <pigpiod_if2.h>

#include "RckamTrackerOptions.hpp"
#include "Controller.hpp"

int main(int argc, char *argv[])
{
  namespace rt = rckam::tracker;
  // process commandline options
  rt::RckamTrackerOptions options;
  const auto action = options.parse(argc, argv);
  if (rt::RckamTrackerOptions::HELP == action)
  {
    std::cout << options.usage() << std::endl;
    return 0;
  }
  else if (rt::RckamTrackerOptions::VERSION == action)
  {
    std::cout << options.version() << std::endl;
    return 0;
  }
  else if (rt::RckamTrackerOptions::ABORT == action)
  {
    return -1;
  }
  assert(rt::RckamTrackerOptions::RUN == action);
  // initialize gpios
  if (PI_INIT_FAILED == gpioInitialise())
  {
    std::cerr << "ERROR: failed to initialize GPIOs" << std::endl;
    return(-1);
  }
  try
  {
    rckam::tracker::Controller controller(options);
    controller.run();
  }
  catch (...)
  {
    std::cerr << "WARNING: exception thrown while running rckam-tracker" << std::endl;
  }
  gpioTerminate();
}

