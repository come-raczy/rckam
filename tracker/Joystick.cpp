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

#include "Joystick.hpp"

#include <iostream>
#include <cmath>

namespace rckam
{
namespace tracker
{

Joystick::Joystick(const unsigned switchChannel, const unsigned vrxChannel, const unsigned vryChannel)
: mcp3008_(0, 100000, 0)
, switch_(switchChannel)
, vrx_(vrxChannel)
, vry_(vryChannel)
{
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << " " << isPressed() << " " << xValue() << " " << yValue() << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << " " << isPressed() << " " << xValue() << " " << yValue() << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << " " << isPressed() << " " << xValue() << " " << yValue() << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << " " << isPressed() << " " << xValue() << " " << yValue() << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << " " << isPressed() << " " << xValue() << " " << yValue() << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << " " << isPressed() << " " << xValue() << " " << yValue() << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << " " << isPressed() << " " << xValue() << " " << yValue() << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << " " << isPressed() << " " << xValue() << " " << yValue() << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << " " << isPressed() << " " << xValue() << " " << yValue() << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << " " << isPressed() << " " << xValue() << " " << yValue() << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << " " << isPressed() << " " << xValue() << " " << yValue() << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << " " << isPressed() << " " << xValue() << " " << yValue() << std::endl;
}

bool Joystick::isPressed()
{
  return 0 == mcp3008_.read(switch_);
}

unsigned Joystick::swValue()
{
  return mcp3008_.read(switch_);
}

int Joystick::directionValue(const unsigned channel)
{
  const auto maxValue = static_cast<float>(mcp3008_.getMaxValue());
  const auto neutralValue = maxValue / 2;
  const auto currentValue = static_cast<float>(mcp3008_.read(channel));
  int value = round((currentValue - neutralValue) / neutralValue * 100 / 5);
  return ((value <= -5) || value >= 5) ? value : 0;
}

int Joystick::xValue()
{
  return directionValue(vrx_);
}

int Joystick::yValue()
{
  return directionValue(vry_);
}

} // namespace tracker
} // namespace rckam

