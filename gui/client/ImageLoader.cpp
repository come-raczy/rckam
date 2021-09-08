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

#include "client/ImageLoader.hpp"

#include <thread>
#include <chrono>
#include <boost/format.hpp>

#include "common/Exceptions.hpp"

namespace rckam
{
namespace client
{

ImageLoader::ImageLoader(client::ImagePreview *imagePreview, const std::string ipAddress, const unsigned dataPort /*, const communicationPort */)
: imagePreview_(imagePreview)
, ioService_()
, socket_(ioService_)
, thread_()
, threadException_(nullptr)
, stop_(false)
{
  using namespace boost::asio;
  using ip::tcp;
  socket_.connect( tcp::endpoint( ip::make_address(ipAddress), dataPort));
  // start the thread to read the data
  thread_ = std::thread(&ImageLoader::read, this);
}

ImageLoader::~ImageLoader()
{
  stop();
  thread_.join();
}

void ImageLoader::readWrapper()
{
  try
  {
    read();
  }
  catch(...)
  {
    threadException_ = std::current_exception();
  }
}

void ImageLoader::read()
{
  using common::RckamException;
  while(!stop_)
  {
    boost::asio::streambuf receive_buffer;
    boost::system::error_code error;
    // read the size of the image in bytes
    const size_t count1 = boost::asio::read(socket_, receive_buffer, boost::asio::transfer_exactly(sizeof(size_t)), error);
    if (error)
    {
      auto message = boost::format("ERROR: failed to read byte count from data socket: %i: %s") % error.value() % error.message();
      BOOST_THROW_EXCEPTION(RckamException(message.str()));
    }
    else if (sizeof(size_t) != count1)
    {
      auto message = boost::format("ERROR: failed to read %i bytes for byte count from data socket: read only %i") % sizeof(size_t) % count1;
      BOOST_THROW_EXCEPTION(RckamException(message.str()));
    }
    const auto byteCount = *(boost::asio::buffer_cast<const size_t*>(receive_buffer.data()));
    receive_buffer.consume(sizeof(size_t));
    // read the image
    const size_t count2 = boost::asio::read(socket_, receive_buffer, boost::asio::transfer_exactly(byteCount), error);
    if (error)
    {
      auto message = boost::format("ERROR: failed to read image data from data socket: %i: %s") % error.value() % error.message();
      BOOST_THROW_EXCEPTION(RckamException(message.str()));
    }
    else if (byteCount != count2)
    {
      auto message = boost::format("ERROR: failed to read %i bytes for image from data socket: read only %i") % byteCount % count2;
      BOOST_THROW_EXCEPTION(RckamException(message.str()));
    }
    // display the image
    imagePreview_->loadFromData(boost::asio::buffer_cast<const uchar *>(receive_buffer.data()), byteCount, "JPG");
    receive_buffer.consume(byteCount);
  }
}

} // namespace client
} // namespace rckam

