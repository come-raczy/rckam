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

#include <cstring>
#include <cerrno>
#include <boost/date_time.hpp>

#include "common/Exceptions.hpp"

namespace rckam
{
namespace common
{

ExceptionData::ExceptionData(int errorNumber, const std::string &message) : boost::exception(),
            errorNumber_(errorNumber), message_(message)
{
}

std::string ExceptionData::getContext() const
{
    const std::string now = boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time());
    return now + ": " + std::string(strerror(errorNumber_)) + ": " + boost::diagnostic_information(*this);
}

IoException::IoException(int errorNumber, const std::string &message)
    : std::ios_base::failure(message)
    , ExceptionData(errorNumber, message)
{
}

Gphoto2Exception::Gphoto2Exception(const std::string& message)
  : std::runtime_error(message), ExceptionData(EIO, message)
{
}

GtkmmException::GtkmmException(int errorNumber, const std::string &message)
    : std::ios_base::failure(message)
    , ExceptionData(errorNumber, message)
{
}

ResourceException::ResourceException(int errorNumber, const std::string &message)
    : ExceptionData(errorNumber, message)
{
}


MemoryException::MemoryException(const std::string &message)
    : std::bad_alloc(),
      ExceptionData(ENOMEM, message)
{
}

QmlException::QmlException(const std::string &message)
    : std::logic_error(message)
    , ExceptionData(EINVAL, message)
{
}

UnsupportedVersionException::UnsupportedVersionException(const std::string &message)
    : std::logic_error(message)
    , ExceptionData(EINVAL, message)
{
}

FeatureNotAvailable::FeatureNotAvailable(const std::string &message)
    : std::logic_error(message)
    , ExceptionData(EINVAL, message)
{
}

InvalidParameterException::InvalidParameterException(const std::string &message)
    : std::logic_error(message)
    , ExceptionData(EINVAL, message)
{
}

InvalidOptionException::InvalidOptionException(const std::string &message)
    : std::logic_error(message)
    , ExceptionData(EINVAL, message)
{
}

PreConditionException::PreConditionException(const std::string &message)
    : std::logic_error(message)
    , ExceptionData(EINVAL, message)
{
}

PostConditionException::PostConditionException(const std::string &message)
    : std::logic_error(message)
    , ExceptionData(EINVAL, message)
{
}

LibXsltException::LibXsltException()
    : RckamException(EINVAL, "libxslt failure")
{
}

} // namespace common
} // namespace rckam
