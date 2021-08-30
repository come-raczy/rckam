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

#include "Encoder.hpp"

#include <cassert>
#include <boost/format.hpp>
#include <pigpio.h>

#include "Exceptions.hpp"


namespace rckam
{
namespace tracker
{

Encoder::Encoder(const unsigned a, const unsigned b)
: gpios_{a, b}
, lastGpio_(-1)
, counter_(0)
, states_{1, 1}
, callback_(nullptr)
{
  for (const auto gpio: gpios_)
  {
    const auto ret = gpioSetMode(gpio, PI_INPUT);
    if (0 != ret)
    {
      const auto message = boost::format("ERROR: failed to set GPIO %i to INPUT mode: %i") % gpio % ret;
      BOOST_THROW_EXCEPTION(GpioException(message.str()));
    }
    // pull up is needed as encoder common is grounded
    gpioSetPullUpDown(gpio, PI_PUD_UP);
    // monitor encoder level changes
    gpioSetAlertFuncEx(gpio, interrupt, this);
  }
}

Encoder::~Encoder()
{
  for (const auto gpio: gpios_)
  {
    gpioSetAlertFuncEx(gpio, nullptr, this);
    gpioSetPullUpDown(gpio, PI_PUD_OFF);
  }
  callback_ = nullptr;
}

void Encoder::interrupt(int gpio, int level, uint32_t tick, void *encoder)
{
  reinterpret_cast<Encoder *>(encoder)->interrupt(gpio, level, tick);
}

void Encoder::interrupt(const int gpio, const int level, const uint32_t tick)
{
  assert((gpio == (int)gpios_[0]) || (gpio == (int)gpios_[1]));
  const unsigned gpioIndex = (gpio == (int)gpios_[1]);
  std::unique_lock<std::mutex> lock(mutex_);
  states_[gpioIndex] = level;
  // debounce if needed and update the counter on H/H
  if ((gpio != (int)lastGpio_) && (1 == states_[0]) && (1 == states_[1]))
  {
    const int increment = (gpioIndex == 0) ? 1 : -1;
    counter_ = counter_ + increment;
    if (nullptr != callback_)
    {
      callback_(increment);
    }
  }
  lastGpio_ = gpio;
}

} // namespace tracker
} // namespace rckam

