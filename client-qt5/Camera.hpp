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

#ifndef RCKAM_CLIENT_CAMERA_HPP
#define RCKAM_CLIENT_CAMERA_HPP

#include <string>
#include <vector>
#include <memory>

namespace rckam
{
namespace client 
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
};

} // namespace client
} // namespace rckam

#endif // #ifndef RCKAM_CLIENT_CAMERA_HPP

