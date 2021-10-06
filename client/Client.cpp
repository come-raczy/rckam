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

#include "client/Client.hpp"

#include <chrono>
#include <thread>
#include <boost/asio.hpp>

#include "common/Rckam.hpp"

namespace rckam
{
namespace client
{

Client::Client(const RckamClientOptions &options)
: ipAddress_(options.ipAddress)
, dataPort_(options.dataPort)
, controlPort_(options.controlPort)
{
}

Client::~Client()
{
}

const std::string &Client::ipAddress() const
{
  return ipAddress_;
}

void Client::setIpAddress(const std::string &ipAddress)
{
  ipAddress_ = ipAddress;
}

unsigned Client::dataPort() const
{
  return dataPort_;
}

void Client::setDataPort(unsigned dataPort)
{
  dataPort_ = dataPort;
}

unsigned Client::controlPort() const
{
  return controlPort_;
}

void Client::setControlPort(unsigned controlPort)
{
  controlPort_ = controlPort;
}

bool Client::isReady()
{
  return false;
}
  
std::vector<CameraInfo> Client::listCameras(boost::system::error_code error)
{
  std::vector<CameraInfo> cameraList;
  const std::string response = query(common::ServerCommand::LIST_CAMERAS, error);
  if (error)
  {
    return cameraList;
  }
  std::istringstream is(response);
  while(is)
  {
    CameraInfo cameraInfo;
    getline(is, cameraInfo.model, '\t');
    getline(is, cameraInfo.port);
    cameraList.emplace_back(cameraInfo);
  }
  return cameraList;
}

// TODO: add support for arguments to the command (a buffer)
std::string Client::query(const common::ServerCommand command, boost::system::error_code error)
{
  using namespace boost::asio;
  using ip::tcp;
  boost::asio::io_service io_service;
  tcp::socket socket(io_service);
  RCKAM_THREAD_CERR << "INFO: connecting socket to " << ipAddress_ << ":" << controlPort_ << std::endl;
  socket.connect( tcp::endpoint( boost::asio::ip::make_address(ipAddress_), controlPort_), error);
  if (error)
  {
    RCKAM_THREAD_CERR << "ERROR: failed to connect socket: " << error.value() << ": " << error.message() << std::endl;
    return std::string();
  }
  boost::asio::const_buffer cbuffer(reinterpret_cast<const char *>(&command), sizeof(command));
  RCKAM_THREAD_CERR << "INFO: sending " << sizeof(command) << " bytes:";
  for (unsigned int i=0; sizeof(command) > i; ++i)
  {
    std::cerr << " " << (int)(*(reinterpret_cast<const char *>(cbuffer.data())+i));
  }
  std::cerr << std::endl;
  const size_t sentCount  = boost::asio::write( socket, cbuffer, boost::asio::transfer_all(), error);
  if (error)
  {
    RCKAM_THREAD_CERR << "ERROR: failed to send command to the server: " << error.value() << ": " << error.message() << std::endl;
    return std::string();
  }
  if (sizeof(command) != sentCount)
  {
    RCKAM_THREAD_CERR << "ERROR: failed to send " << sizeof(command) << " bytes to the server: " << sentCount << " bytes sent" << std::endl;
    return std::string();
  }
  RCKAM_THREAD_CERR << "INFO: successfully sent " << sentCount << " bytes" << std::endl;
  std::array<char, common::COMMAND_BUFFER_SIZE> buffer;
  size_t receivedCount = 0;
  boost::asio::mutable_buffer inputBuffer (buffer.data(), buffer.size());
  boost::asio::async_read(socket, inputBuffer, [&](const boost::system::error_code& resultError, std::size_t count)
  {
    if (!error)
    {
      RCKAM_THREAD_CERR << "INFO: received " << count << " bytes from server" << std::endl;
    }
    else
    {
      RCKAM_THREAD_CERR << "ERROR: failed to receive data from server: " << count << " bytes received: " << error.value() << ": " << error.message() << std::endl;
    }
    error = resultError;
    receivedCount = count;
  });
  io_service.restart();
  const auto timeout = std::chrono::milliseconds(1000);
  io_service.run_for(timeout);
  if (!io_service.stopped())
  {
    RCKAM_THREAD_CERR << "ERROR: timeout while waiting for the server" << std::endl;
    // Close the socket to cancel the outstanding asynchronous operation.
    socket.close();
    // Run the io_context again until the operation completes.
    io_service.run();
    if (!error)
    {
      error = make_error_code(boost::system::errc::timed_out);
      //error = std::make_error(ETIMEDOUT);
    }
  }
  if(error && error != boost::asio::error::eof)
  {
    return std::string();
  }
  else
  {
    std::string s(buffer.data(), receivedCount);
    return s;
  }
}

} // namespace client
} // namespace rckam

