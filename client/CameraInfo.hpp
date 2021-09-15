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

#ifndef RCKAM_CLIENT_CAMERA_INFO_HPP
#define RCKAM_CLIENT_CAMERA_INFO_HPP

#include <string>

namespace rckam
{
namespace client
{

struct CameraInfo
{
public:
  std::string model;
  std::string port;
};

} // namespace client
} // namespace rckam

#endif // #ifndef RCKAM_CLIENT_CAMERA_INFO_HPP

