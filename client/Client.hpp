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

#ifndef RCKAM_CLIENT_CLIENT_HPP
#define RCKAM_CLIENT_CLIENT_HPP

#include <vector>
#include <boost/system/error_code.hpp> // NOTE: boost::asio does not support std::error_code

#include "client/RckamClientOptions.hpp"
#include "client/CameraInfo.hpp"

namespace rckam
{
namespace client
{

/**
 ** \brief proxy component to coordinate communications with the server
 **
 ** There are several aspects related to the server side:
 ** - localization and configuration (ip address, ports)
 ** - camera queries
 ** - camera commands
 ** - mount command
 ** 
 **/
class Client
{
public:
  Client(const RckamClientOptions &options);
  ~Client();
  const std::string &ipAddress() const;
  void setIpAddress(const std::string &ipAddress);
  unsigned dataPort() const;
  void setDataPort(unsigned dataPort);
  unsigned controlPort() const;
  void setControlPort(unsigned cobtrolPort);
  bool isReady();
  std::vector<CameraInfo> listCameras(boost::system::error_code error);
protected:
private:
  std::string ipAddress_;
  unsigned dataPort_;
  unsigned controlPort_;
  std::string query(const common::ServerCommand command, boost::system::error_code error);
};

} // namespace client
} // namespace rckam

#endif // #ifndef RCKAM_CLIENT_CLIENT_HPP

