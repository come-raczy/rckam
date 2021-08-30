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

#ifndef RCKAM_TRACKER_ENCODER_HPP
#define RCKAM_TRACKER_ENCODER_HPP

#include <functional>
#include <mutex>

namespace rckam
{
namespace tracker
{

/**
 ** \brief Support for a quadratic encoder
 **
 ** A typical quadratic encoder would have:
 ** - B signal (white)
 ** - A signal (yellow)
 ** - +5V Vcc  (blue)
 ** - Ground   (green)
 **/
class Encoder
{
public:
  //typedef void (*Callback)(int);
  typedef std::function<void(int)> Callback;
  /// Connects and initializes the encoder connected to GPIOs a and b
  Encoder(unsigned a, unsigned b);
  Encoder(const Encoder &) = delete;
  Encoder(Encoder &&) = default;
  Encoder& operator=(Encoder&&) = default;

  ~Encoder();
  void setCallback(Callback callback)
  {
    callback_ = callback;
  }
  int counter() const {return counter_;}
  void reset() {counter_ = 0;}
private:
  const std::array<unsigned, 2> gpios_;
  unsigned lastGpio_;
  int counter_;
  std::array<unsigned, 2> states_;
  std::mutex mutex_;
  /// callback function for user convenience
  Callback callback_;
  /// interrupt callback associated to both a and b
  void interrupt(int gpio, int level, uint32_t tick);
  /// static delegate to use as a C callback
  static void interrupt(int gpio, int level, uint32_t tick, void *encoder);
};

} // namespace tracker
} // namespace rckam

#endif // #ifndef RCKAM_TRACKER_ENCODER_HPP
