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

#include "client/RckamClientOptions.hpp"

namespace rckam
{
namespace client
{

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;
using common::InvalidOptionException;
using boost::format;

RckamClientOptions::RckamClientOptions()
{
  //unnamedOptions_.add_client()
  //    ("some-option"   , bpo::value<int>(&someValue)->default_value(someValue),
  //        "some documentation")
  //        ;

  namedOptions_.add_options()
    ("ip-address,i", bpo::value<std::string>(&ipAddress)->required(),
              "ip address of the server (v4)")
    ("data-port,d", bpo::value<unsigned>(&dataPort)->default_value(dataPort),
              "the port to use for data transfer")
    ;

}

/**
 * \brief remembers the original argv array and hands over to the base implementation
 */
common::Options::Action RckamClientOptions::parse(int argc, char *argv[])
{
  const std::vector<std::string> allOptions(argv, argv + argc);
  RCKAM_THREAD_CERR << "Version: " << rckam_VERSION_FULL << std::endl;
  RCKAM_THREAD_CERR << "argc: " << argc << " argv: " << boost::join(allOptions, " ") << std::endl;

  common::Options::Action ret = common::Options::parse(argc, argv);
  return ret;
}

void RckamClientOptions::postProcess(bpo::variables_map &vm)
{
  if(vm.count("help") ||  vm.count("version"))
  {
    return;
  }
}

} //namespace option
} // namespace rckam
