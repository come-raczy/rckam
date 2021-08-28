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

namespace rckam
{
namespace tracker
{

Joystick::Joystick(const RckamTrackerOptions &options)
: mcp3008_(0, 100000, 0)
{
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << std::endl;
  std::cerr << mcp3008_.read(0) << " " << mcp3008_.read(1) << " " << mcp3008_.read(2) <<  " " << mcp3008_.read(3) << " " << mcp3008_.read(4) <<  " " << mcp3008_.read(5) << " " << mcp3008_.read(6)  << " " << mcp3008_.read(7) << std::endl;
}

} // namespace tracker
} // namespace rckam

