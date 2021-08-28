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
#include "Joystick.hpp"

namespace rckam
{
namespace tracker
{

class Controller
{
public:
  Controller(const RckamTrackerOptions &options);
  void run();
private:
  const Joystick joystick_;
};

} // namespace tracker
} // namespace rckam

#endif // #ifndef RCKAM_TRACKER_CONTROLLER_HPP
