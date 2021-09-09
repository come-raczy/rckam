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

#ifndef RCKAM_COMMON_DEBUG_HPP
#define RCKAM_COMMON_DEBUG_HPP

#include <mutex>
#include <iostream>

namespace rckam
{
namespace common
{

#define RCKAM_THREAD_CERR  common::LockedOstream(std::cerr, true)

/**
 ** \brief wrap a mutex around a stream for clean multithreaded output
 **
 ** Usage: LockedOstream() << a << b << c << std::endl;
 ** Note: the mutex is shared among all the streams that use this class
 **/
class LockedOstream
{
public:
  LockedOstream(::std::ostream &os, const bool timeStamp = true);
  ~LockedOstream() { mutex_.unlock(); }

  template <class T>
  LockedOstream &operator << (const T &value)
  {
    os_ << value;
    return *this;
  }

  /// std::endl requires a special treatment
  LockedOstream& operator<<(std::ostream& (*fp)(std::ostream&))
  {
    os_ << fp;
    return *this;
  }

private:
    ::std::ostream &os_;
    static ::std::mutex mutex_;
};

} // namespace common
} // namespace rckam

#endif // #ifndwef RCKAM_COMMON_DEBUG_HPP
