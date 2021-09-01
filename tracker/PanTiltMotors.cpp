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

#include "PanTiltMotors.hpp"

#include <cassert>
#include <iostream>

namespace rckam
{
namespace tracker
{

PanTiltMotors::PanTiltMotors(const unsigned ain1, const unsigned ain2,
                             const unsigned bin1, const unsigned bin2,
                             const unsigned pwmA, const unsigned pwmB,
                             const unsigned stby, const unsigned pwmFreq,
                             const unsigned panA, const unsigned panB,
                             const unsigned tiltA, const unsigned tiltB)
: tb6612fng_(ain1, ain2, bin1, bin2, pwmA, pwmB, stby, pwmFreq)
, encoders_{Encoder(panA, panB), Encoder(tiltA, tiltB)}
, gotoMonitors_{encoders_[0], encoders_[1]}
{
  std::cerr << "INFO: PanTiltMotors constructed succesfully" << std::endl;
}

PanTiltMotors::~PanTiltMotors()
{
}
void PanTiltMotors::pan(int duty)
{
  assert((-100 <= duty) && (100 >= duty));
  tb6612fng_.setDirection(PAN, duty < 0 ? CCW : CW);
  tb6612fng_.setDuty(PAN, std::abs(duty));
}

void PanTiltMotors::pan(const Direction direction, const unsigned duty)
{
  assert (100U >= duty);
  tb6612fng_.setDirection(PAN, direction);
  tb6612fng_.setDuty(PAN, duty);
}

void PanTiltMotors::tilt(int duty)
{
  assert((-100 <= duty) && (100 >= duty));
  tb6612fng_.setDirection(TILT, duty < 0 ? CCW : CW);
  tb6612fng_.setDuty(TILT, std::abs(duty));
}

void PanTiltMotors::tilt(const Direction direction, const unsigned duty)
{
  assert(100 >= duty);
  tb6612fng_.setDirection(TILT, direction);
  tb6612fng_.setDuty(TILT, duty);
}

void PanTiltMotors::setZero()
{
  for (auto &encoder: encoders_)
  {
    encoder.reset();
  }
}

void PanTiltMotors::GotoMonitor::callback(const int increment)
{
  if (encoder_.counter() == finalPosition_)
  {
    encoder_.setCallback(nullptr);
    stop_();
  }
}

void PanTiltMotors::gotoPosition(const int pan, const int tilt)
{
  auto &panEncoder =  encoders_[static_cast<unsigned>(PAN)];
  auto &tiltEncoder =  encoders_[static_cast<unsigned>(TILT)];
  const auto panDirection = (pan < panEncoder.counter() ? CCW : CW);
  const auto tiltDirection = (tilt < tiltEncoder.counter() ? CCW : CW);
  // set a callback on each encoder
  auto &panGotoMonitor = gotoMonitors_[static_cast<unsigned>(PAN)];
  panGotoMonitor.setFinalPosition(pan);
  panGotoMonitor.setStopFunction(std::bind(static_cast<void(PanTiltMotors::*)(int)>(&PanTiltMotors::pan), this, 0));
  panEncoder.setCallback(std::bind(&GotoMonitor::callback, panGotoMonitor, std::placeholders::_1));
  auto &tiltGotoMonitor = gotoMonitors_[static_cast<unsigned>(TILT)];
  tiltGotoMonitor.setFinalPosition(tilt);
  tiltGotoMonitor.setStopFunction(std::bind(static_cast<void(PanTiltMotors::*)(int)>(&PanTiltMotors::tilt), this, 0));
  tiltEncoder.setCallback(std::bind(&GotoMonitor::callback, tiltGotoMonitor, std::placeholders::_1));
  // set the direction and duty for each motor
  this->pan(panDirection, 20);
  this->tilt(tiltDirection, 20);
}

void PanTiltMotors::drive()
{
  tb6612fng_.drive();
}

void PanTiltMotors::standby()
{
  tb6612fng_.standby();
}
} // namespace tracker
} // namespace rckam

