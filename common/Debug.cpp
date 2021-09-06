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

#include "Debug.hpp"

#include <chrono>
#include <iomanip>
#include <thread>

namespace rckam
{
namespace common
{

std::mutex LockedOstream::mutex_;

void timestamp(std::ostream &os);

LockedOstream::LockedOstream(::std::ostream &os, const bool timeStamp)
: os_(os)
{
  mutex_.lock();
  if (timeStamp)
  {
    timestamp(os_);
  }
}

void timestamp(std::ostream &os)
{
  const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
  auto timeNow = std::chrono::system_clock::to_time_t(now);
  const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
  os << std::put_time(std::localtime(&timeNow), "%Y-%m-%d %X") << "."
     << std::setfill('0') << std::setw(3) << milliseconds << "\t["
     << std::this_thread::get_id() << "]\t";
}

} // namespace common
} // namespace rckam

