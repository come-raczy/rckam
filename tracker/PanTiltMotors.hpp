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

#ifndef RCKAM_TRACKER_PAN_TILT_MOTORS_HPP
#define RCKAM_TRACKER_PAN_TILT_MOTORS_HPP

#include <boost/core/noncopyable.hpp>
#include <array>

#include "Tb6612fng.hpp"
#include "Encoder.hpp"

namespace rckam
{
namespace tracker
{

/**
 ** \brief abstraction of a pair of motors for pan and tilt motion
 **
 ** The assumption is that the two motors are both connected to the
 ** same H-bridge (TB6612FNG) and that each motor has its own independent
 ** incremental quadratic encoder.
 **/
class PanTiltMotors : boost::noncopyable
{
public:
  typedef Tb6612fng::Motor Motor;
  typedef Tb6612fng::Direction Direction;
  static constexpr auto PAN = Motor::A;
  static constexpr auto TILT = Motor::B;
  static constexpr auto CCW = Direction::CCW;
  static constexpr auto CW = Direction::CW;
  /**
   ** \brief constructor
   **
   ** GPIO pins connected to the TB6612FNG (OUTPUT GPIOs):
   ** - ain1 and ain2: inputs controlling direction and mode of motor A
   ** - bin1 and bin2: inputs controlling direction and mode of motor B
   ** - pwma and pwmb: PWM signal for motor A and motor B respectively
   ** - stby: standby (HIGH for normal operation)
   **
   ** GPIO pins connected to the encoders (INPUT GPIOs):
   ** - panA and panB: channel A and channel B of the encoder for panning
   ** - tiltA and tiltB: channel A and channel B of the encoder for tilting
   **
   ** Other parameters:
   ** - pwmFreq: the operating frequency to use for the PWM signqal
   **/
  PanTiltMotors(unsigned ain1, unsigned ain2, unsigned bin1, unsigned bin2, unsigned pwma, unsigned pwmb, unsigned stby, unsigned pwmFreq, unsigned panA, unsigned panB, unsigned tiltA, unsigned tiltB);
  ~PanTiltMotors();
  /// pan at the specified duty - [-100, 100], negatif is CCW
  void pan(int duty);
  /// pan in the specified direction at the specified duty [0, 100]
  void pan(Direction direction, unsigned duty);
  ///  at the specified duty - [-100, 100], negatif is CCW
  void tilt(int duty);
  ///  at in the specified direction at the specified duty [0, 100]
  void tilt(Direction direction, unsigned duty);
  /// set the current position as the zero position
  void setZero();
  /// go to the specidied absolute position
  void gotoPosition(int pan, int tilt);
  /// standby
  void standby();
private:
  Tb6612fng tb6612fng_;
  std::array<Encoder, 2> encoders_;
  class GotoMonitor
  {
  public:
    void callback(int increment);
    GotoMonitor(Encoder &encoder)
    : encoder_(encoder)
    {
    }
    void setFinalPosition(int finalPosition) {finalPosition_ = finalPosition;}
    typedef std::function<void()> Stop;
    void setStopFunction(Stop stop) {stop_ = stop;}
  private:
    int finalPosition_;
    Encoder &encoder_;
    Stop stop_;
  };
  std::array<GotoMonitor, 2> gotoMonitors_;
};

} // namespace tracker
} // namespace rckam

#endif // #ifndef RCKAM_TRACKER_PAN_TILT_MOTORS_HPP
