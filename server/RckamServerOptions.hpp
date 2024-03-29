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

#ifndef RCKAM_SERVER_OPTIONS_HPP
#define RCKAM_SERVER_OPTIONS_HPP

#include <boost/core/noncopyable.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/program_options.hpp>

namespace rckam
{
namespace server
{

#define STRINGIFY(s) XSTRINGIFY(s)
#define XSTRINGIFY(s) #s

namespace bpo = boost::program_options;

class RckamServerOptions : boost::noncopyable
{
public:
  enum Action { RUN, HELP, VERSION, ABORT };
  RckamServerOptions();
  virtual ~RckamServerOptions() {}
  Action             parse(int argc, const char* const argv[]);
  std::string        usage() const;
  int                argc() const { return argc_; }
  const char* const* argv() const { return argv_; }

  std::string getCommandLine() const
  {
    const std::vector<std::string> allOptions(argv(), argv() + argc());
    return boost::join(allOptions, " ");
  }

  bool exists(const std::string& opt) const { return vm_.count(opt); }
  static std::string version() { return std::string(std::string("rckam-server version ") + STRINGIFY(RCKAM_VERSION) + "-" + STRINGIFY(VERSION_STRING)); }

  /// port to use
  unsigned port = 11005;
  
protected:
  bool                                version_ = false;
  bpo::options_description            namedOptions_;
  bpo::options_description            unnamedOptions_;
  bpo::positional_options_description positionalOptions_;

  typedef boost::shared_ptr<boost::program_options::option_description> OptionDescriptionPtr;
  typedef std::vector<OptionDescriptionPtr>                             OptionDescriptionPtrs;
  std::string helpDefaults(const OptionDescriptionPtrs& options) const;
  std::string help(const OptionDescriptionPtrs& options, const bool markdown) const;

private:
  std::string usagePrefix() const {return "rckam-server [options]";}
  std::string usageSuffix() const { return ""; }
  void        postProcess(bpo::variables_map&);

  static const unsigned MARKDOWN_LINE_LENGTH = 120;
  // "parse" will store the state in vm_ so that "usage" can access the details of parsed command line
  bpo::variables_map vm_;
  int                argc_ = 0;
  const char* const* argv_ = 0;
};

} // namespace server
} // namespace rckam

#endif // #ifndef RCKAM_SERVER_OPTIONS_HPP

