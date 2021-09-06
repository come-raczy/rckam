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

#include "CameraController.hpp"

#include <iostream>
#include <fstream>
#include <boost/format.hpp>
#include <chrono>
#include <iomanip>

#include "Debug.hpp"

namespace rckam
{
namespace camera
{

CameraController::CameraController(const char * const model, const char * const port, Gphoto2Context &context)
: camera_(model, port, context)
, dataSocket_()
, communicationSocket_()
{
}

CameraController::~CameraController()
{
}

void CameraController::run()
{
  for (unsigned int i = 0; 100 > i; ++i)
  {
    CameraFile cameraFile;
    common::LockedOstream(std::cerr) << "capturing " << std::setw(3) << "..." << std::endl;
    const auto size = camera_.capturePreview(cameraFile);
    const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    common::LockedOstream(std::cerr) << "capture completed. Received " << size << " bytes" << std::endl;
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
    const auto timeString = std::put_time(std::localtime(&in_time_t), "%X");
    auto fileName = boost::format("/tmp/preview-%05i-%s.%03i.jpg") % i % timeString % ms;
    std::ofstream os(fileName.str());
    os << cameraFile;
  }
}

} // namespace camera
} // namespace rckam

