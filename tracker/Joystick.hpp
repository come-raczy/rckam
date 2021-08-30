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

#ifndef RCKAM_TRACKER_JOYSTICK_HPP
#define RCKAM_TRACKER_JOYSTICK_HPP

#include <boost/core/noncopyable.hpp>

#include "RckamTrackerOptions.hpp"
#include "Mcp3008.hpp"

namespace rckam
{
namespace tracker
{

/**
 ** \brief Suppoprt for a joystick connected to an MCP008
 **
 ** The values can be read on demand.
 ** Note: a 10K resistor is required to connect the SW pin to +3.3V.
 ** TODO: implement callback mechanisms for click, double-click and long-press.
 **/
class Joystick : boost::noncopyable
{
public:
  Joystick(const RckamTrackerOptions &options);
  /// returns true when the switch is pressed
  bool isPressed();
  /// returns a percentage of X motion (value in [-100, +100]
  int xValue();
  /// returns a percentage of Y motion (value in [-100, +100]
  int yValue();
private:
  /// the ADC
  Mcp3008 mcp3008_;
  /// channel used for switch
  const unsigned switch_;
  /// channel used for X
  const unsigned vrx_;
  /// channel used for Y
  const unsigned vry_;
  /// converts the ABC value into a directional percentage [-100, 100]
  int directionValue(const unsigned channel);
};

} // namespace tracker
} // namespace rckam

#endif // #ifndef RCKAM_TRACKER_JOYSTICK_HPP
