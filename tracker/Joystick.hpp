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

#include "RckamTrackerOptions.hpp"
#include "Mcp3008.hpp"

namespace rckam
{
namespace tracker
{

/**
 ** \brief Suppoprt for a joystick connected to an MCP008
 **/
class Joystick
{
public:
  Joystick(const RckamTrackerOptions &options);
private:
  Mcp3008 mcp3008_;
};

} // namespace tracker
} // namespace rckam

#endif // #ifndef RCKAM_TRACKER_JOYSTICK_HPP
