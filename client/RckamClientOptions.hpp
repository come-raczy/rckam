/**
 **
 ** rckam: a Qt remote control for digital cameras
 **
 ** Copyright (C) <year>  <name of author>
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Affero General Public License as
 ** published by the Free Software Foundation, either version 3 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Affero General Public License for more details.
 **
 ** You should have received a copy of the GNU Affero General Public License
 ** along with this program.  If not, see <https://www.gnu.org/licenses/>.
 **
 **/

#ifndef RCKAM_CLIENT_RCKAM_CLIENT_OPTIONS_HPP
#define RCKAM_CLIENT_RCKAM_CLIENT_OPTIONS_HPP

#include <string>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "common/Program.hpp"
#include "common/Rckam.hpp"

namespace rckam
{
namespace client
{

class RckamClientOptions : public common::Options
{
public:
  RckamClientOptions();
  common::Options::Action parse(int argc, char *argv[]);
  std::string ipAddress;
  unsigned dataPort = common::dataPort;
  unsigned controlPort = common::controlPort;
private:
  std::string usagePrefix() const {return "rckam-client -r <reference> -b <base calls> [optional arguments]";}
  void postProcess(boost::program_options::variables_map &vm);

public:
  std::string description;
};

} // namespace client
} // namespace rckam

#endif // #ifndef RCKAM_CLIENT_RCKAM_CLIENT_OPTIONS_HPP
