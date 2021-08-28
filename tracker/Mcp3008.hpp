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

#ifndef RCKAM_TRACKER_MCP3008_HPP
#define RCKAM_TRACKER_MCP3008_HPP

#include <array>
#include <boost/core/noncopyable.hpp>
#include <pigpio.h>

namespace rckam
{
namespace tracker
{

#define BX 8               // Bit position of data bit B9.
#define B0 (BX + BITS - 1) // Bit position of data bit B0.
#define BUFFER 250       // Generally make this buffer as large as possible.
#define REPEAT_MICROS 40 // Reading every x microseconds.
#define SAMPLES 2000000  // Number of samples to take,

/**
 ** \brief Support for an MCP3008 as an ADC on SPI0
 **/
class Mcp3008 : boost::noncopyable
{
public:
  // Pins for SPI0
  constexpr static unsigned MISO =  9; // Note: pigpio desines BSC_MISO_2711 as 10
  constexpr static unsigned MOSI = 10; // Note: pigpio desines BSC_MOSI_2711 as  9
  constexpr static unsigned SCLK = BSC_SCL_SCLK_2711; // expected to be 11
  constexpr static unsigned  CE0 = BSC_CE_N_2711;     // expected to be 8
  constexpr static unsigned  CE1 =  9; // Note: not defined in pigpio
  constexpr static unsigned BITS = 10;
  constexpr static unsigned CHANNELS = 8; // number of channels available on the MCP3008
  /// \brief constructor - defaults to 
  Mcp3008(unsigned chipSelect = 0, unsigned baud = 100000, unsigned flags = 0);
  ~Mcp3008();
  /// \brief read a single 10-bits value for the given channel (ch0...ch7)
  unsigned read(unsigned channel);
  constexpr unsigned getMaxValue() const {return (1U << BITS);}
private:
  /// \brief handle to the spi device associated to the chip select
  int spi_;
};

} // namespace tracker
} // namespace rckam

#endif // #ifndef RCKAM_TRACKER_MCP3008_HPP
