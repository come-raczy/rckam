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

#include "Tb6612fng.hpp"

#include <iostream>
#include <cassert>
#include <pigpio.h>

#include <boost/format.hpp>

#include "Exceptions.hpp"

namespace rckam
{
namespace tracker
{

Tb6612fng::Tb6612fng(const unsigned ain1, const unsigned ain2, const unsigned bin1, const unsigned bin2, const unsigned pwmA, const unsigned pwmB, const unsigned stby, const unsigned pwmFreq)
: in1_{ain1, bin1}
, in2_{ain2, bin2}
, pwm_{pwmA, pwmB}
, stby_(stby)
, freq_(pwmFreq)
{
  std::cerr << "INFO: initializing Tb6612fng..." << std::endl;
  // TODO: initialize the GPIOs to output
  for (const auto in: {in1_, in2_, pwm_})
  {
    for (const int i: {0, 1})
    {
      const auto ret = gpioSetMode(in[i], PI_OUTPUT);
      if (0 != ret)
      {
        const auto message = boost::format("ERROR: failed to set GPIO %i to OUTPUT mode: %i") % in[i] % ret;
        BOOST_THROW_EXCEPTION(GpioException(message.str()));
      }
    }
  }
  std::cerr << "INFO: Tb6612fng constructed succesfully" << std::endl;
}

void Tb6612fng::setDuty(const Motor motor, const unsigned duty)
{
  assert(100 >= duty);
  const auto pwm = pwm_[static_cast<unsigned>(motor)];
  // Note: fully on is 1000000
  const auto ret = gpioHardwarePWM(pwm , freq_, duty * 10000);
  if (0 != ret)
  {
    const auto message = boost::format("ERROR: failed to set hardware PWM for GPIO %i to frequency %i and duty %i: %i") % pwm % freq_ % duty % ret;
    BOOST_THROW_EXCEPTION(GpioException(message.str()));
  }
}

void Tb6612fng::setDirection(const Motor motor, const Direction direction)
{
  const auto m = static_cast<unsigned>(motor);
  // levels for IN1 and IN2 respectively are L=0 and H=1 for CCW (direction=0)
  auto level = static_cast<unsigned>(direction);
  for (const auto in: {in1_, in2_})
  {
    const auto ret = gpioWrite(in[m], level);
    if (0 != ret)
    {
      const auto message = boost::format("ERROR: failed to set GPIO %i to %i: %i") % in[m] % level % ret;
      BOOST_THROW_EXCEPTION(GpioException(message.str()));
    }
    level = (level + 1) % 2;
  }
}

void Tb6612fng::standby()
{
  gpioWrite(stby_, 0);
}

void Tb6612fng::drive()
{
  gpioWrite(stby_, 1);
}

} // namespace tracker
} // namespace rckam

