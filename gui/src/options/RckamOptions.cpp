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

#include <algorithm>
#include <string>
#include <vector>
#include <ostream>
#include <fstream>
#include <typeinfo>

#include <boost/assign.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string.hpp>

#include "common/Exceptions.hpp"
#include "options/RckamOptions.hpp"
#include "config.h"

namespace rckam
{
namespace options
{

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;
using common::InvalidOptionException;
using boost::format;

RckamOptions::RckamOptions()
{
    //unnamedOptions_.add_options()
    //    ("base-calls-directory"   , bpo::value<std::vector<std::string> >(&baseCallsStringList)->multitoken(),
    //        "Deprecated. Same as --base-calls.")
    //        ;

    //namedOptions_.add_options()
    //    ("base-calls,b"   , bpo::value<std::vector<std::string> >(&baseCallsStringList)->multitoken(),
    //            "full path to the base calls. Multiple entries allowed. Path should point either to a directory or "
    //            "a file depending on --base-calls-format")
    //       ;

}

/**
 * \brief remembers the original argv array and hands over to the base implementation
 */
common::Options::Action RckamOptions::parse(int argc, char *argv[])
{
    const std::vector<std::string> allOptions(argv, argv + argc);
    RCKAM_THREAD_CERR << "Version: " << rckam_VERSION_FULL << std::endl;
    RCKAM_THREAD_CERR << "argc: " << argc << " argv: " << boost::join(allOptions, " ") << std::endl;

    common::Options::Action ret = common::Options::parse(argc, argv);
    return ret;
}

void RckamOptions::postProcess(bpo::variables_map &vm)
{
    if(vm.count("help") ||  vm.count("version"))
    {
        return;
    }
}

} //namespace option
} // namespace rckam
