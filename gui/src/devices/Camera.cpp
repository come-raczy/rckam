/**
 ** rckam: a Qt remote control for digital cameras
 **
 ** Copyright (C) <year>  <name of author>
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Affero General Public License as
 ** published by the Free Software Foundation, either version 3 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Affero General Public License for more details.
 **
 ** You should have received a copy of the GNU Affero General Public License
 ** along with this program.  If not, see <https://www.gnu.org/licenses/>.
 **/

#include "devices/Camera.hpp"

#include <boost/format.hpp>
#include "common/Exceptions.hpp"

namespace rckam
{
namespace devices 
{

Camera::Camera()
    : context(gp_context_new(), std::free)
    , gpCamera(nullptr)
{
    const auto ret = gp_camera_new(&gpCamera);
    if (GP_OK != ret)
    {
        switch (ret) 
        {
            case GP_ERROR_NO_MEMORY: // failed to alloc memory for functioni, pc, fs, ...
                BOOST_THROW_EXCEPTION(common::MemoryException("Failed to allocate new camera: GP_ERROR_NO_MEMORY"));
                break;
            default: // shouldn't happen
                BOOST_THROW_EXCEPTION(common::RckamException((boost::format("Failed to allocate new camera: GP_ERROR: %i: %s") % ret % gp_result_as_string(ret)).str()));
                break;
        }
    }
}

Camera::~Camera()
{
    if (nullptr != gpCamera)
    {
        gp_camera_unref(gpCamera); // gp_camera_free is deprecated
    }
}

std::vector<std::string> Camera::listCameras()
{
    return std::vector<std::string>();
}

void Camera::connect(const std::string name)
{
    auto ret = gp_camera_init(gpCamera, context.get());
    if(ret != GP_OK)
    {
        BOOST_THROW_EXCEPTION(common::RckamException((boost::format("Failed to initialize camera: GP_ERROR: %i: %s") % ret % gp_result_as_string(ret)).str()));
    }
    // clear events from initialization
    CameraEventType eventType;
    void *eventData;
    do {
            ret = gp_camera_wait_for_event (gpCamera, 10, &eventType, &eventData, context.get());
    } while ((ret == GP_OK) && (eventType != GP_EVENT_TIMEOUT));
    if(ret != GP_OK)
    {
        BOOST_THROW_EXCEPTION(common::RckamException((boost::format("Failed to clear event queue: GP_ERROR: %i: %s") % ret % gp_result_as_string(ret)).str()));
    }
}

void Camera::disconnect()
{
}

} // namespace devices
} // namespace rckam
