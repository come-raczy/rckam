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

#ifndef RCKAM_CAMERA_CAMERA_CONTROLLER_HPP
#define RCKAM_CAMERA_CAMERA_CONTROLLER_HPP

#include <mutex>
#include <condition_variable>
#include <array>
#include <boost/asio.hpp>

#include "Gphoto2Context.hpp"
#include "Camera.hpp"
#include "DataSocket.hpp"
//#include "CommunicationSocket.hpp"

namespace rckam
{
namespace camera
{

class CameraController
{
public:
  CameraController(const char *model, const char *usbPort, unsigned dataPort, unsigned controlPort, Gphoto2Context &context);
  ~CameraController();
  /// operate the camera
  void run();
  void stopPreview();
private:
  Gphoto2Context *context_;
  /// the underlying gphoto2 camera
  Camera camera_;
  /// the socket to transmit data (image previews)
  //DataSocket dataSocket_;
  // io service for TCP sockets
  //boost::asio::io_service io_service_;
  // io context for UDP sockets
  boost::asio::io_context io_context_;
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint remoteEndpoint_;
  unsigned controlPort_;
  /// acceptor for TCP connections
  //boost::asio::io_service ioService_;
  //boost::asio::ip::tcp::acceptor controlAcceptor_;
  //boost::asio::ip::tcp::socket controlSocket_;
  /// the socket for communication and synchronization
  //CommunicationSocket communicationSocket_;
  std::array<std::mutex, 2> mutexes_;
  std::array<std::condition_variable, 2> conditionVariables_;
  std::array<CameraFile, 2> cameraFiles_;
  std::array<bool, 2> cameraFilesEmpty_;
  bool stopPreview_;
  std::exception_ptr transferDataException_;
  bool stopControl_;
  std::thread controlThread_;
  std::exception_ptr controlException_;
  void transferData();
  void transferDataWrapper();
  void control();
  void controlWrapper();
  void sendResponse(boost::asio::ip::tcp::socket &socket, const std::string &&data);
  std::string listCameras();
  std::string commandNotSupported();
};

} // namespace camera
} // namespace rckam

#endif // #ifdef RCKAM_CAMERA_CAMERA_CONTROLLER_HPP

