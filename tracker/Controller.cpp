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

#include "Controller.hpp"

#include <thread>
#include <chrono>
#include <iostream>

namespace rckam
{
namespace tracker
{

Controller::Controller(const RckamTrackerOptions &options)
: panTiltMotors_(options.ain1, options.ain2, options.bin1, options.bin2, 
                 options.pwmA, options.pwmB, options.stby, options.pwmFreq,
                 options.panA, options.panB, options.tiltA, options.tiltB)
, joystick_(options.switchChannel, options.vrxChannel, options.vryChannel)
{
  std::cerr << "INFO: Controller succesfully constructed" << std::endl;
}

void f(int gpio, int level, uint32_t tick)
{
  std::cerr << "CALLBACK: GPIO " << gpio << " became " << level << " at " << tick << std::endl;
}

void Controller::run()
{
  panTiltMotors_.drive();
  static const auto CCW = PanTiltMotors::Direction::CCW;
  static const auto CW = PanTiltMotors::Direction::CW;
  panTiltMotors_.pan(0);
  std::cerr << "position: " << panTiltMotors_.panPosition() << ", " << panTiltMotors_.tiltPosition() << std::endl;
  // for (unsigned i = 1; i != 21; ++i)
  // {
    // std::cerr << "panning CCW for 10 second at " << i << "%" << std::endl;
    // panTiltMotors_.pan(CCW, i);
    // std::this_thread::sleep_for (std::chrono::seconds(10));
    // std::cerr << "position: " << panTiltMotors_.panPosition() << ", " << panTiltMotors_.tiltPosition() << std::endl;
  // }
  panTiltMotors_.pan(0);
  std::cerr << "position: " << panTiltMotors_.panPosition() << ", " << panTiltMotors_.tiltPosition() << std::endl;
  std::cerr << "panning CCW for 3 second at 50%" << std::endl;
  panTiltMotors_.pan(CCW, 50);
  std::this_thread::sleep_for (std::chrono::seconds(3));
  panTiltMotors_.pan(0);
  std::cerr << "position: " << panTiltMotors_.panPosition() << ", " << panTiltMotors_.tiltPosition() << std::endl;
  std::cerr << "panning CW for 3 second at 50%" << std::endl;
  panTiltMotors_.pan(CW, 50);
  std::this_thread::sleep_for (std::chrono::seconds(3));
  panTiltMotors_.pan(0);
  std::cerr << "position: " << panTiltMotors_.panPosition() << ", " << panTiltMotors_.tiltPosition() << std::endl;
  std::cerr << "tilting CCW for 3 second at 50%" << std::endl;
  panTiltMotors_.tilt(CCW, 50);
  std::this_thread::sleep_for (std::chrono::seconds(3));
  panTiltMotors_.tilt(0);
  std::cerr << "position: " << panTiltMotors_.panPosition() << ", " << panTiltMotors_.tiltPosition() << std::endl;
  std::cerr << "tilting CW for 3 second at 50%" << std::endl;
  panTiltMotors_.tilt(CW, 50);
  std::this_thread::sleep_for (std::chrono::seconds(3));
  panTiltMotors_.tilt(0);
  std::cerr << "position: " << panTiltMotors_.panPosition() << ", " << panTiltMotors_.tiltPosition() << std::endl;
  //std::cerr << "Going to position 1000, 1000" << std::endl;
  //panTiltMotors_.gotoPosition(1000, 1000);
  //std::cerr << "position: " << panTiltMotors_.panPosition() << ", " << panTiltMotors_.tiltPosition() << std::endl;

  bool done = false;
  gpioSetAlertFunc(4, f);
  gpioSetAlertFunc(6, f);
  gpioSetAlertFunc(14, f);
  gpioSetAlertFunc(15, f);
  while (!done)
  {
    std::cerr << "Joystick: " << joystick_.swValue() << ", " << joystick_.xValue() << ", " << joystick_.yValue() << std::endl;
    panTiltMotors_.pan(joystick_.xValue());
    panTiltMotors_.tilt(joystick_.yValue());
    std::this_thread::sleep_for (std::chrono::seconds(1));
    panTiltMotors_.pan(0);
    panTiltMotors_.tilt(0);
    if (joystick_.isPressed())
    {
      done = true;
    }
  }
  panTiltMotors_.standby();
}

} // namespace tracker
} // namespace rckam

