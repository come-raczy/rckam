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

#include "client/Camera.hpp"

#include <boost/format.hpp>
#include "common/Exceptions.hpp"

namespace rckam
{
namespace client 
{

Camera::Camera()
//    : context(gp_context_new(), std::free)
//    , gpCamera(nullptr)
{
#if 0
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
#endif
}

Camera::~Camera()
{
#if 0
    if (nullptr != gpCamera)
    {
        gp_camera_unref(gpCamera); // gp_camera_free is deprecated
    }
#endif
}

#if 0
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
#endif

} // namespace client
} // namespace rckam
