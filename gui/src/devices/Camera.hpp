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

#ifndef DEVICES_CAMERA_HPP
#define DEVICES_CAMERA_HPP

#include <string>
#include <vector>
#include <memory>

//#include <gphoto2/gphoto2.h>

namespace rckam
{
namespace devices 
{

class Camera
{
    Camera(const  Camera&) = delete;
    Camera& operator=(const  Camera&) = delete;
public:
    Camera();
    ~Camera();
    static std::vector<std::string> listCameras();
    void connect(const std::string name);
    void disconnect();
    // capture a preview and return the buffer
    //char *capturePreview();
private:
//    typedef ::Camera GpCamera;
//    std::unique_ptr<GPContext, decltype(std::free) *> context;
//    GpCamera *gpCamera; // not using unique_ptr because of gp_camera_unref
};

} // namespace devices
} // namespace rckam

#endif // #ifndef DEVICES_CAMERA_HPP

