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

namespace rckam
{
namespace tracker
{

Controller::Controller(const RckamTrackerOptions &options)
: joystick_(options)
{
}

void Controller::run()
{
}

} // namespace tracker
} // namespace rckam

