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
#include <chrono>
#include <iomanip>
#include <cerrno>
#include <boost/system/error_code.hpp> // NOTE: boost::asio does not support boost::system::error_code
#include <boost/format.hpp>

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>


#include "common/Debug.hpp"
#include "common/Rckam.hpp"
#include "common/Exceptions.hpp"

namespace rckam
{
namespace server
{

CameraController::CameraController(/*const char * const model, const char * const usbPort,*/ const unsigned dataPort,/* const unsigned controlPort,*/ Gphoto2Context &context)
: context_(&context)
, cameraList_(*context_)
, camera_(*context_)
//, dataSocket_()
//, io_service_()
//, acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), dataPort))
//, socket_(io_service_)
//, resolver_(io_context_)
, io_context_()
, socket_(io_context_, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), dataPort))
//, controlPort_(controlPort)
//, stopControl_(false)
//, controlThread_(&CameraController::controlWrapper, this)
//, communicationSocket_()
{
}

CameraController::~CameraController()
{
//  stopControl_ = true;
//  if (controlThread_.joinable())
//  {
//    controlThread_.join();
//  }
}

#if 0
void CameraController::controlWrapper()
{
  try
  {
    control();
  }
  catch(std::exception &e)
  {
    RCKAM_THREAD_CERR << "WARNING: control thread terminated: " << e.what() << std::endl;
    controlException_ = std::current_exception();
  }
  catch(...)
  {
    RCKAM_THREAD_CERR << "WARNING: control thread terminated:unknown exception" << std::endl;
    controlException_ = std::current_exception();
  }
}

void CameraController::control()
{
  using common::ServerCommand;
  using namespace boost::asio;
  using ip::tcp;
  io_service ioService;
  tcp::acceptor acceptor(ioService, tcp::endpoint(tcp::v4(), controlPort_));
  tcp::socket socket(ioService);
  while (!stopControl_)
  {
    boost::system::error_code error;
    // wait for a request
    acceptor.accept(socket);
    boost::asio::streambuf buffer;
    const unsigned count = boost::asio::read(socket, buffer, boost::asio::transfer_all(), error);
    if(error)
    {
      RCKAM_THREAD_CERR << "WARNING: error while reading the control socket: " << error.value() << error.message() << std::endl;
      continue;
    }
    if (sizeof(ServerCommand) > count)
    //read operation
    {
      RCKAM_THREAD_CERR << "WARNING: error while reading the control socket: message too short: received " << count << " bytes - ServerCommands is " << sizeof(ServerCommand) << " bytes" << std::endl;
      continue;
    }
    std::string data(boost::asio::buffer_cast<const char*>(buffer.data()));
    assert(data.size() > sizeof(ServerCommand));
    const ServerCommand command = *(reinterpret_cast<ServerCommand *>(data.data()));
    switch (command)
    {
      case ServerCommand::ECHO_ONLY:
        sendResponse(socket, std::move(data));
        break;
      case ServerCommand::LIST_CAMERAS:
        sendResponse(socket, listCameras());
        break;
      default:
        sendResponse(socket, commandNotSupported());
    }
  }
}

void CameraController::sendResponse(boost::asio::ip::tcp::socket &socket, const std::string &&data)
{
  boost::system::error_code error;
  boost::asio::write(socket, boost::asio::buffer(data), error);
  if(error)
  {
    RCKAM_THREAD_CERR << "WARNING: error while sending response on control socket: " << error.value() << ": " << error.message() << std::endl;
  }
}

std::string CameraController::commandNotSupported()
{
  static const std::string response{0,common::ResponseCode::NOT_SUPPORTED};
  return response;
}
#endif

std::string CameraController::listCameras()
{
  std::string response;
  std::string separator;
  cameraList_ = CameraList(*context_);
  for(unsigned i = 0; cameraList_.count() > i; ++i)
  {
    response.append(separator);
    response.append(std::to_string(i));
    response.append(1, '\t');
    response.append(cameraList_.model(i));
    response.append(1, '\t');
    response.append(cameraList_.port(i));
    separator = "\n";
  }
  return response;
}

void CameraController::selectCamera(const char * const model, const char * const usbPort)
{
  using common::Gphoto2Exception;
  camera_.setAbilities(model);
  camera_.setPortInfo(usbPort);
}

#if 0
void CameraController::run()
{
  using common::RckamException;
  // wait for the connection - TCP
  // acceptor_.accept(socket_);
  // wait for the connection - UDP
  std::array<char, 1> receiveBuffer;
  boost::system::error_code error;
  constexpr boost::asio::socket_base::message_flags FLAGS = 0;
  RCKAM_THREAD_CERR << "INFO: waiting for data on " << socket_.local_endpoint().address() << ":" << socket_.local_endpoint().port() << "..." << std::endl;
  socket_.receive_from(boost::asio::buffer(receiveBuffer), remoteEndpoint_, FLAGS, error);
  if (error)
  {
    RCKAM_THREAD_CERR << "ERROR: socket failed to receive from remote: " << error.value() << ": " << error.message() << std::endl;
    BOOST_THROW_EXCEPTION(RckamException(std::string("socket failed to receive from remote: ") +  error.message()));
  }
  RCKAM_THREAD_CERR << "INFO: sending image previews to " << remoteEndpoint_.address() << ":" << remoteEndpoint_.port() << std::endl;
  // create a thread to send the data
  stopPreview_ = false;
  transferDataException_ = nullptr;
  std::thread dataTransferThread = std::thread(&CameraController::transferDataWrapper, this);;
  for (auto &cameraFileEmpty: cameraFilesEmpty_)
  {
    cameraFileEmpty = true;
  }
  unsigned index = 0;
  unsigned i = 0;
  while (!stopPreview_)
  {
    std::unique_lock<std::mutex> lock(mutexes_[index]);
    conditionVariables_[index].wait(lock, [&] {return cameraFilesEmpty_[index] || stopPreview_;});
    if (stopPreview_)
    {
      break;
    }
    CameraFile &cameraFile = cameraFiles_[index];
    RCKAM_THREAD_CERR << "INFO: capturing " << std::setw(4) << i << "..." << std::endl;
    const std::chrono::time_point<std::chrono::system_clock> captureBegin = std::chrono::system_clock::now();
    const size_t byteCount = camera_.capturePreview(cameraFile);
    const std::chrono::time_point<std::chrono::system_clock> captureEnd = std::chrono::system_clock::now();
    if (0 == (i % 100))
    {
      auto in_time_t = std::chrono::system_clock::to_time_t(captureBegin);
      const auto beginMs = std::chrono::duration_cast<std::chrono::milliseconds>(captureBegin.time_since_epoch()).count() % 1000;
      const auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(captureEnd.time_since_epoch()).count() 
                            - std::chrono::duration_cast<std::chrono::milliseconds>(captureBegin.time_since_epoch()).count();
      const auto timeString = std::put_time(std::localtime(&in_time_t), "%X");
      auto fileName = boost::format("/tmp/preview-%05i-%s.%03i-%05i.jpg") % i % timeString % beginMs % durationMs;
      std::ofstream os(fileName.str());
      os << cameraFile;
    }
    RCKAM_THREAD_CERR << "INFO: capturing " << std::setw(4) << i << "... done (" << byteCount << " bytes)" << std::endl;
    cameraFilesEmpty_[index] = false;
    conditionVariables_[index].notify_one();
    index = (index + 1) % 2;
    ++i;
  }
  stopPreview_ = true;
  for (auto &conditionVariable: conditionVariables_)
  {
    conditionVariable.notify_all();
  }
  if (dataTransferThread.joinable())
  {
    dataTransferThread.join();
  }
}
#endif

void CameraController::transferDataWrapper()
{
  try
  {
    transferData();
  }
  catch(std::exception &e)
  {
    RCKAM_THREAD_CERR << "WARNING: data transfer thread terminated: " << e.what() << std::endl;
    transferDataException_ = std::current_exception();
  }
  catch(...)
  {
    RCKAM_THREAD_CERR << "WARNING: data transfer thread terminated: unkmown exception" << std::endl;
    transferDataException_ = std::current_exception();
  }
}

void CameraController::transferData()
{
  using common::RckamException;
  constexpr boost::asio::socket_base::message_flags FLAGS = 0;
  unsigned index = 0;
  unsigned currentImageId = 0;
  while (!stopPreview_)
  {
    std::unique_lock<std::mutex> lock(mutexes_[index]);
    conditionVariables_[index].wait(lock, [&]{return (false == cameraFilesEmpty_[index]) || stopPreview_;});
    if (stopPreview_)
    {
      break;
    }
    CameraFile &cameraFile = cameraFiles_[index];
    const auto dataAndSize = cameraFile.getDataAndSize();
    const auto data = std::get<0>(dataAndSize);
    const size_t byteCount = std::get<1>(dataAndSize);
    RCKAM_THREAD_CERR << "INFO: sending " << std::setw(4) << currentImageId << "... (" << byteCount << " bytes)" << std::endl;
    // const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    // common::LockedOstream(std::cerr) << "capture completed. Received " << size << " bytes" << std::endl;
    // auto in_time_t = std::chrono::system_clock::to_time_t(now);
    // const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
    // const auto timeString = std::put_time(std::localtime(&in_time_t), "%X");
    // auto fileName = boost::format("/tmp/preview-%05i-%s.%03i.jpg") % i % timeString % ms;
    // std::ofstream os(fileName.str());
    // os << cameraFile;
    boost::system::error_code error;
    // TCP
    //const auto count1 = boost::asio::write( socket_, boost::asio::buffer(&byteCount, sizeof(size_t)), boost::asio::transfer_exactly(sizeof(size_t)), error);
    // UDP
    std::array<char, 4> header; // 2 bytes for image id, 2 bytes for packet id
    uint16_t * const imageId = reinterpret_cast<uint16_t*>(header.data());
    uint16_t * const packetId = reinterpret_cast<uint16_t*>(header.data() + 2);
    *imageId = currentImageId;
    *packetId = 0;
    constexpr size_t MAX_PACKET_SIZE = 1472;
    constexpr size_t MAX_PAYLOAD = MAX_PACKET_SIZE - header.size();
    using boost::asio::const_buffer;
    const_buffer headerBuffer = const_buffer(header.data(), header.size());
    size_t offset = 0;
    std::array<const_buffer, 2> bufferSequence{headerBuffer, const_buffer(reinterpret_cast<const char *>(&byteCount), sizeof(size_t))};
    const auto count1 = socket_.send_to(bufferSequence, remoteEndpoint_, FLAGS, error); 
    if (error)
    {
      auto message = boost::format("ERROR: failed to send byte count to data socket: %i: %s") % error.value() % error.message();
      BOOST_THROW_EXCEPTION(RckamException(message.str()));
    }
    else if (sizeof(size_t) + header.size() != count1)
    {
      auto message = boost::format("ERROR: failed to send %i bytes for byte count to data socket: sent only %i") % (header.size() + sizeof(size_t)) % count1;
      BOOST_THROW_EXCEPTION(RckamException(message.str()));
    }
    // TCP
    //const auto count2 = boost::asio::write( socket_, boost::asio::buffer(data, byteCount), boost::asio::transfer_exactly(byteCount), error);
    // UDP
    while(byteCount > offset)
    {
      ++(*packetId);
      const size_t remaining = byteCount - offset;
      const size_t size = std::min(MAX_PAYLOAD, remaining);
      bufferSequence[1] = const_buffer(data + offset, size);
      //RCKAM_THREAD_CERR << "INFO: sending " << (size + header.size()) << "=" << size << "+" << header.size() << " bytes at offset " << offset << " for " << *imageId << ":" << *packetId << std::endl;
      const auto count2 = socket_.send_to(bufferSequence, remoteEndpoint_, FLAGS, error); 
      if (error)
      {
        auto message = boost::format("ERROR: failed to send image data to data socket: %i: %s") % error.value() % error.message();
        BOOST_THROW_EXCEPTION(RckamException(message.str()));
      }
      else if (size + header.size() != count2)
      {
        auto message = boost::format("ERROR: failed to send %i bytes of image data to data socket: sent only %i") % (size + header.size()) % count2;
        BOOST_THROW_EXCEPTION(RckamException(message.str()));
      }
      offset += size;
    }
    RCKAM_THREAD_CERR << "INFO: sending " << std::setw(4) << currentImageId << "... done" << std::endl;
    cameraFilesEmpty_[index] = true;
    conditionVariables_[index].notify_one();
    index = (index + 1) % 2;
    ++currentImageId;

//stopPreview_ = (currentImageId > 3);

  }
}

void CameraController::startHdmiCapture()
{
  using common::RckamException;
  // NOTE: for more information and examplese:
  // * https://kernel.readthedocs.io/en/latest/media/uapi/v4l/capture-example.html
  // * https://gist.github.com/maxlapshin/1253534
  // * https://jayrambhia.com/blog/capture-v4l2
  // * https://gist.github.com/jayrambhia/5866483
  // * https://web.archive.org/web/20110520211256/http://v4l2spec.bytesex.org/spec/capture-example.html
  // TODO: parametrize the device
  int fd = open("/dev/video0", O_RDWR);
  // TODO: parametrize the format
  // TODO: add frame rate
  struct v4l2_format format = {0};
  format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  format.fmt.pix.width = 1920;
  format.fmt.pix.height = 1080;
  //format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
  format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
  format.fmt.pix.field = V4L2_FIELD_NONE;
  if(-1 == ioctl(fd, VIDIOC_S_FMT, &format))
  {
    auto message = boost::format("ERROR: failed to set the HDMI video format: ") % errno % strerror(errno);
    BOOST_THROW_EXCEPTION(RckamException(message.str()));
  }
  // TODO: parametrize the number of buffers
  const int count = 2;
  struct v4l2_requestbuffers req = {0};
  req.count = count;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  // NOTE: this is MMAP, alternatives are READ and USRPTR
  req.memory = V4L2_MEMORY_MMAP;
  if (-1 == ioctl(fd, VIDIOC_REQBUFS, &req))
  {
    auto message = boost::format("ERROR: failed to request buffers for HDMI video: ") % errno % strerror(errno);
    BOOST_THROW_EXCEPTION(RckamException(message.str()));
  }
  struct v4l2_buffer buf = {0};
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  buf.index = 0;
  if(-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf))
  {
    auto message = boost::format("ERROR: failed to retrieve V4L2 buffers for HDMI video: ") % errno % strerror(errno);
    BOOST_THROW_EXCEPTION(RckamException(message.str()));
  }
  auto buffer = (u_int8_t*)mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
  unsigned int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if(-1 == ioctl(fd, VIDIOC_STREAMON, &type))
  {
    auto message = boost::format("ERROR: failed to start HDMI video stream: ") % errno % strerror(errno);
    BOOST_THROW_EXCEPTION(RckamException(message.str()));
  }
  stopHdmiCapture_ = false;
  while (!stopHdmiCapture_)
  {
    struct v4l2_buffer bufd = {0};
    bufd.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufd.memory = V4L2_MEMORY_MMAP;
    bufd.index = 0;
    if(-1 == ioctl(fd, VIDIOC_QBUF, &bufd))
    {
      auto message = boost::format("ERROR: failed to queue HDMI video buffer: ") % errno % strerror(errno);
      BOOST_THROW_EXCEPTION(RckamException(message.str()));
    }
    //return bufd.bytesused;
    //Wait for io operation
    int camera = fd;
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(camera, &fds);
    struct timeval tv = {0};
    tv.tv_sec = 2; //set timeout to 2 second
    if(-1 == select(camera+1, &fds, NULL, NULL, &tv))
    {
      auto message = boost::format("ERROR: failed to wait for an HDMI frame: ") % errno % strerror(errno);
      BOOST_THROW_EXCEPTION(RckamException(message.str()));
    }
    // TODO: transfer the buffer
    //int file = open("output.yuy", O_WRONLY);
    //write(file, buffer, size); //size is obtained from the query_buffer function
    //dequeue_buffer(camera);
    if(-1 == ioctl(fd, VIDIOC_DQBUF, &bufd))
    {
      auto message = boost::format("ERROR: failed to dequeue HDMI video buffer: ") % errno % strerror(errno);
      BOOST_THROW_EXCEPTION(RckamException(message.str()));
    }
  }
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if(-1 == ioctl(fd, VIDIOC_STREAMON, &type))
  {
    auto message = boost::format("ERROR: failed to stop HDMI video capture: ") % errno % strerror(errno);
    BOOST_THROW_EXCEPTION(RckamException(message.str()));
  }
}

void CameraController::stopHdmiCapture()
{
  stopHdmiCapture_ = true;
}

} // namespace server
} // namespace rckam

