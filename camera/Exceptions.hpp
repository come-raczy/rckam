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

#ifndef RCKAM_CAMERA_EXCEPTIONS_HPP
#define RCKAM_CAMERA_EXCEPTIONS_HPP

#include <boost/cerrno.hpp>
#include <boost/exception/all.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/throw_exception.hpp>
#include <ios>
#include <stdexcept>
#include <string>

namespace rckam
{
namespace camera
{

/**
 ** \brief base class to all the exception classes.
 **
 ** Use BOOST_THROW_EXCEPTION to get the context info (file, function, line)
 ** at the throw site.
 **/
class ExceptionData : public boost::exception {
public:
  ExceptionData(int errorNumber = 0, const std::string& message = "");
  ExceptionData(const ExceptionData& e)
    : boost::exception(e), errorNumber_(e.errorNumber_), message_(e.message_)
  {
  }
  virtual ~ExceptionData() throw() {}
  int                getErrorNumber() const { return errorNumber_; }
  const std::string& getMessage() const { return message_; }
  std::string        getContext() const;

private:
  const int         errorNumber_;
  const std::string message_;
  ExceptionData&    operator=(const ExceptionData&);
};

class RckamException : public std::exception, public ExceptionData {
public:
  RckamException(int errorNumber, const std::string& message) : ExceptionData(errorNumber, message) {}
  RckamException(const std::string& message) : ExceptionData(0, message) {}
  RckamException(const RckamException& e) : std::exception(e), ExceptionData(e) {}
  virtual const char* what() const throw() { return getMessage().c_str(); }

private:
  RckamException& operator=(const RckamException&);
};

/**
 * \brief Exception thrown when there are problems with the IO operations
 */
class IoException : public std::ios_base::failure, public ExceptionData {
public:
  IoException(int errorNumber, const std::string& message);
};

/**
 * \brief Eny gphoto2-related error, with explanation message
 */
class Gphoto2Exception : public std::runtime_error, public ExceptionData {
public:
  Gphoto2Exception(const std::string& message);
};

} // namespace camera
} // namespace rckam

#endif // #ifndef RCKAM_CAMERA_EXCEPTIONS_HPP

