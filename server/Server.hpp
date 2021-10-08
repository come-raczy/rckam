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

#ifndef RCKAM_SERVER_SERVER_HPP
#define RCKAM_SERVER_SERVER_HPP

#include <string>
#include <boost/asio/ip/tcp.hpp>

#include "common/Exceptions.hpp"
#include "Gphoto2Context.hpp"
#include "CameraController.hpp"

namespace rckam
{
namespace server
{

/**
 ** \brief top level class encapsulating all relevant components on the server side
 **
 ** The main functions are:
 ** - communication and synchronization with the client
 ** - identification, management and control of local hardware
 **
 **/
class Server
{
public:
  Server(unsigned int controlPort, unsigned int dataPort);
  ~Server();
  /// establish the communication socket listening to the client and dispatch the client commands
  void run();
private:
  unsigned int controlPort_;
  bool stop_;
  Gphoto2Context gphoto2Context_;
  CameraController cameraController_;
  std::string commandNotSupported();
  std::vector<std::string> parse(const char *data, size_t available) const;
  std::string success() const;
  std::string failure(const common::ExceptionData &e) const;
  std::string failure(const std::exception&e) const;
  void sendResponse(boost::asio::ip::tcp::socket &socket, const std::string &&data);
};

} // namespace server
} // namespace rckam

#endif // #ifndef RCKAM_SERVER_SERVER_HPP

