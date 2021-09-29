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

#include "Exceptions.hpp"

#include <cerrno>
#include <cstring>

namespace rckam
{
namespace server
{

ExceptionData::ExceptionData(int errorNumber, const std::string& message)
  : boost::exception(), errorNumber_(errorNumber), message_(message)
{
}

std::string ExceptionData::getContext() const
{
  const std::string now;
  return now + ": " + std::string(strerror(errorNumber_)) + ": " + boost::diagnostic_information(*this);
}

IoException::IoException(int errorNumber, const std::string& message)
  : std::ios_base::failure(message), ExceptionData(errorNumber, message)
{
}

Gphoto2Exception::Gphoto2Exception(const std::string& message)
  : std::runtime_error(message), ExceptionData(EIO, message)
{
}

} // namespace server
} // namespace rckam

