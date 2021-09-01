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

#ifndef RCKAM_TRACKER_TB6612FNG_HPP
#define RCKAM_TRACKER_TB6612FNG_HPP

#include <boost/core/noncopyable.hpp>

namespace rckam
{
namespace tracker
{

/**
 ** \brief Support for a double-H bridge TB6612FNG
 **
 ** The bridge supports two motors, A and B. Each motor is controlled by 3
 ** inputs: IN1, IN2 and PWM. These inputs control the two outputs OUT1 and
 ** OUT2 that are expected to be be connected to the + and - of the DC motor.
 ** The bridge has an additional input for standby which should be pulled high
 ** fot normal operations.
 ** With standby set to H, the motor will run when the inputs are H/L or L/H. 
 ** In that case the speed is controlled by the PWM input. Other combinations
 ** result in short break or stop as indicated in the table:
 **
 ** IN1 IN2 PWM STBY OUT1 OUT2 Mode
 **  H   H  H/L    H   L    L Short Brake
 **  L   H   H     H   L    H CCW
 **  L   H   L     H   L    L Short Break
 **  H   L   H     H   H    L CW
 **  H   L   L     H   L    L Short Break
 **  L   L   H     H Off  Off Stop
 ** H/L H/L H/L    L Off  Off Standby
 **
 ** Recommended use is to drive the motor as expected, using IN1 and IN2 to set
 ** the direction, and PWM to set the speed.
 ** Limiters can be implemented as "Stops" with switches forcing the normally H
 ** input to L. Conversely they can be implemented as "short brakes" by forcing
 ** the normally L input to H. 
 **/
class Tb6612fng : boost::noncopyable
{
public:
  enum class Motor {A = 0, B = 1};
  enum class Direction {CCW = 0, CW = 1};
  /// Connects and initializes the H-bridge and sets it to standby
  Tb6612fng(unsigned ain1, unsigned ain2, unsigned bin1, unsigned bin2, unsigned pwmA, unsigned pwmB, unsigned stby, unsigned pwmFreq);
  /// set the direction of the specified motor
  void setDirection(Motor motor, Direction direction);
  /// set the duty of the motor as a percentage [0, 100]
  void setDuty(Motor motor, unsigned duty);
  /// stes the H-bridge into standby mode (STBY to L)
  void standby();
  /// stes the H-bridge into drive mode (STBY to H)
  void drive();
private:
  const unsigned in1_[2];
  const unsigned in2_[2];
  const unsigned pwm_[2];
  const unsigned stby_;
  const unsigned freq_;
};

} // namespace tracker
} // namespace rckam

#endif // #ifndef RCKAM_TRACKER_TB6612FNG_HPP
