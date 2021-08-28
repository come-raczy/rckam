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

#include "Mcp3008.hpp"

#include <boost/format.hpp>

#include "Exceptions.hpp"

namespace rckam
{
namespace tracker
{

Mcp3008::Mcp3008(const unsigned chipSelect, const unsigned baud, const unsigned flags)
: spi_(spiOpen(chipSelect, baud, flags))
{
  if (0 > spi_)
  {
    const auto message = boost::format("ERROR: failed to open SPI device: CS=%i, baud=%i, flags=%i: $i") % chipSelect % baud % flags % spi_;
    BOOST_THROW_EXCEPTION(GpioException(message.str()));
  }
  // Need to set GPIO as outputs otherwise wave will have no effect.
  //gpioSetMode(rawSPI.clk,  PI_OUTPUT);
  //gpioSetMode(rawSPI.mosi, PI_OUTPUT);
  //gpioSetMode(SPI_SS,      PI_OUTPUT);
  // Flush any old unused wave data.
  //gpioWaveAddNew();
}

Mcp3008::~Mcp3008()
{
  if (0 <= spi_)
  {
    spiClose(spi_);
    spi_ = -1;
  }
}

unsigned Mcp3008::read(const unsigned channel)
{
  // channel corresponds to pins 1 (ch0) to 8 (ch7)
  // we have to send three bytes:
  //  - byte 0 has 7 zeros and a start bit
  //  - byte 1 has the top bit set to indicate single rather than differential operation
  //    the next three bits contain the channel the bottom four bits are zero
  //  - byte 2 contains zeros (don't care)
  //  3 bytes are returned:
  //  - byte 0 is ignored
  //  - byte 1 contains the high 2 bits
  //  - byte 2 contains the low 8 bits
  assert(channel == (channel & 0x7));
  char buffer[] = {1, static_cast<char>((0b1000 | channel) << 4), 0};
  const auto ret = spiXfer(spi_, buffer, buffer, 3);
  if (3 != ret)
  {
    const auto message = boost::format("ERROR: failed to transfer 3 bytes for channel %i: %i") % channel % ret;
    BOOST_THROW_EXCEPTION(GpioException(message.str()));
  }
  return (static_cast<unsigned>(buffer[1]&3)<<8) | buffer[2];
}

#if 0
void Mcp3008::getReading(
  int adcs,  // Number of attached ADCs.
  int *MISO, // The GPIO connected to the ADCs data out.
  int OOL,   // Address of first OOL for this reading.
  int bytes, // Bytes between readings.
  int bits,  // Bits per reading.
  char *buf)
{
    int i, a, p;
    uint32_t level;

    p = OOL;

    for (i=0; i<bits; i++)
    {
      level = rawWaveGetOut(p);

      for (a=0; a<adcs; a++)
      {
        putBitInBytes(i, buf+(bytes*a), level & (1<<MISO[a]));
      }

      p--;
    }
  }
#endif

} // namespace tracker
} // namespace rckam

