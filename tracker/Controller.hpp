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

#ifndef RCKAM_TRACKER_CONTROLLER_HPP
#define RCKAM_TRACKER_CONTROLLER_HPP

#include "RckamTrackerOptions.hpp"
#include "PanTiltMotors.hpp"
#include "Joystick.hpp"

namespace rckam
{
namespace tracker
{

/**
 ** \brief a controller for a motorized pan/tilt gimbal
 **
 ** The controlled is connected to the following devices:
 ** - 1 TB6612FNG for PWM control of the two motors
 ** - 2 incremental quadratic controlers - one for each motor
 ** - 1 joystick with switch, connected to a 10-bits ADC (MPC3008)
 **/
class Controller
{
public:
  Controller(const RckamTrackerOptions &options);
  void run();
private:
  PanTiltMotors panTiltMotors_;
  Joystick joystick_;
};

} // namespace tracker
} // namespace rckam

#endif // #ifndef RCKAM_TRACKER_CONTROLLER_HPP
