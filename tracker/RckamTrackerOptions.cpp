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

#include "RckamTrackerOptions.hpp"

#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/exception/diagnostic_information.hpp> 
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/foreach.hpp>

// TODO: move this into a system compatibility layer
#define HAVE_SYS_IOCTL_H
#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif  // #ifdef HAVE_SYS_IOCTL_H

namespace rckam
{
namespace tracker
{

RckamTrackerOptions::RckamTrackerOptions()
{
  namedOptions_.add_options()("help,h", "produce help message and exit");
  namedOptions_.add_options()(
    "help-md", "produce help message pre-formatted as a markdown file section and exit");
  namedOptions_.add_options()(
    "help-defaults", "produce tab-delimited list of command line options and their default values");
  namedOptions_.add_options()("version,V", bpo::bool_switch(&version_), "print program version information");
  namedOptions_.add_options()(
    "response-file", bpo::value<std::string>(), "file with more command line arguments");
  namedOptions_.add_options()(
    "switchChannel",  bpo::value<unsigned>(&switchChannel)->default_value(switchChannel), "ADC channel for the joystick switch")(
    "vrxChannel",  bpo::value<unsigned>(&vrxChannel)->default_value(vrxChannel), "ADC channel for the joystick vrx")(
    "vryChannel",  bpo::value<unsigned>(&vryChannel)->default_value(vryChannel), "ADC channel for the joystick vry")(
    "ain1",  bpo::value<unsigned>(&ain1)->default_value(ain1), "GPIO pin used as input 1 of motor A (BCM number)")(
    "ain2",  bpo::value<unsigned>(&ain2)->default_value(ain2), "GPIO pin used as input 2 of motor A (BCM number)")(
    "bin1",  bpo::value<unsigned>(&bin1)->default_value(bin1), "GPIO pin used as input 1 of motor B (BCM number)")(
    "bin2",  bpo::value<unsigned>(&bin2)->default_value(bin2), "GPIO pin used as input 2 of motor B (BCM number)")(
    "pwmA",  bpo::value<unsigned>(&pwmA)->default_value(pwmA), "GPIO pin used as PWM control of motor A (BCM number)")(
    "pwmB",  bpo::value<unsigned>(&pwmB)->default_value(pwmB), "GPIO pin used as PWM control of motor B (BCM number)")(
    "stby",  bpo::value<unsigned>(&stby)->default_value(stby), "GPIO pin used for standby mode on TB6612FNG  (BCM number)")(
    "pwmFreq",  bpo::value<unsigned>(&pwmFreq)->default_value(pwmFreq), "PWM frequency to use for TB6612FNG")(
    "panA",  bpo::value<unsigned>(&panA)->default_value(panA), "GPIO pin used for channel A of encoder for panning (BCM number)")(
    "panB",  bpo::value<unsigned>(&panB)->default_value(panB), "GPIO pin used for channel B of encoder for panning (BCM number)")(
    "tiltA",  bpo::value<unsigned>(&tiltA)->default_value(tiltA), "GPIO pin used for channel A of encoder for tilting (BCM number)")(
    "tiltB",  bpo::value<unsigned>(&tiltB)->default_value(tiltB), "GPIO pin used for channel B of encoder for tilting (BCM number)");
}

RckamTrackerOptions::Action RckamTrackerOptions::parse(int argc, const char* const argv[])
{
  argc_ = argc;
  argv_ = argv;
  try {
    bpo::options_description allOptions("Allowed options");
    allOptions.add(namedOptions_).add(unnamedOptions_);
    vm_.clear();
    bpo::store(
        bpo::command_line_parser(argc, argv).options(allOptions).positional(positionalOptions_).run(), vm_);

    if (vm_.count("response-file")) {
      // Load the file and tokenize it
      std::ifstream ifs(vm_["response-file"].as<std::string>().c_str());
      if (!ifs) {
        std::clog << "Could not open response file: " << vm_["response-file"].as<std::string>().c_str()
                  << std::endl;
        return ABORT;
      }
      // Read the whole file into a string
      std::stringstream ss;
      ss << ifs.rdbuf();
      if (!ifs) {
        std::clog << "Could not read response file: " << vm_["response-file"].as<std::string>().c_str()
                  << std::endl;
        return ABORT;
      }
      // Split the file content
      boost::char_separator<char>                   sep(" \n\r");
      std::string                                   ResponsefileContents(ss.str());
      boost::tokenizer<boost::char_separator<char>> tok(ResponsefileContents, sep);
      std::vector<std::string>                      args;
      copy(tok.begin(), tok.end(), std::back_inserter(args));
      // Parse the file and store the options
      bpo::store(bpo::command_line_parser(args).options(allOptions).run(), vm_);
    }

    bpo::notify(vm_);
    if (vm_.count("help") || vm_.count("help-md") || vm_.count("help-defaults")) {
      return HELP;
    } else if (version_) {
      return VERSION;
    }
    postProcess(vm_);
  } catch (const boost::program_options::multiple_values& e) {
    std::clog << usage() << std::endl;
    std::clog << "Failed to parse the options: " << e.what() << ": " << e.get_option_name() << std::endl;
    return ABORT;
  } catch (const boost::program_options::multiple_occurrences& e) {
    std::clog << usage() << std::endl;
    std::clog << "Failed to parse the options: " << e.what() << ": " << e.get_option_name() << std::endl;
    return ABORT;
  } catch (const boost::program_options::required_option& e) {
    std::clog << usage() << std::endl;
    std::clog << "Failed to parse the options: " << e.what() << ": " << e.get_option_name() << std::endl;
    return ABORT;
  } catch (const boost::exception& e) {
    std::clog << usage() << std::endl;
    std::clog << "Failed to parse the options: " << boost::diagnostic_information(e) << std::endl;
    return ABORT;
  } catch (const std::exception& e) {
    std::clog << usage() << std::endl;
    std::clog << "Failed to parse the options: " << e.what() << std::endl;
    return ABORT;
  }
  return RUN;
}

bool compareOptionName(
    const boost::shared_ptr<boost::program_options::option_description>& left,
    const boost::shared_ptr<boost::program_options::option_description>& right)
{
  return left->long_name() < right->long_name();
}

std::string RckamTrackerOptions::usage() const
{
  OptionDescriptionPtrs sortedOptions = namedOptions_.options();
  std::sort(sortedOptions.begin(), sortedOptions.end(), compareOptionName);

  if (vm_.count("help-defaults")) {
    return helpDefaults(sortedOptions);
  }

  return help(sortedOptions, vm_.count("help-md"));
}

int getTerminalWindowSize(unsigned short int& ws_row, unsigned short int& ws_col)
{
#ifdef HAVE_SYS_IOCTL_H
  winsize ws = {0, 0, 0, 0};
  if (!ioctl(STDERR_FILENO, TIOCGWINSZ, &ws)) {
    ws_row = ws.ws_row;
    ws_col = ws.ws_col;
    return 0;
  }
#endif  // HAVE_SYS_IOCTL_H

  return -1;
}

static unsigned short getTerminalColumns()
{
  unsigned short int ws_row = 0;
  unsigned short int ws_col = 0;

  if (-1 == getTerminalWindowSize(ws_row, ws_col)) {
    return bpo::options_description::m_default_line_length;
  }

  return ws_col;
}

std::string RckamTrackerOptions::helpDefaults(const OptionDescriptionPtrs& sortedOptions) const
{
  std::string ret;
  BOOST_FOREACH (const OptionDescriptionPtr& odPtr, sortedOptions) {
    ret += odPtr->long_name() + "\t" + odPtr->format_parameter() + "\n";
  }
  return ret;
}

std::string RckamTrackerOptions::help(const OptionDescriptionPtrs& sortedOptions, const bool markdown) const
{
  std::ostringstream os;
  if (!markdown) {
    const unsigned           effectiveLineLength = getTerminalColumns();
    bpo::options_description printedDescriptions(
        "Command line options",
        effectiveLineLength < 50 ? bpo::options_description::m_default_line_length : effectiveLineLength,
        effectiveLineLength < 50 ? bpo::options_description::m_default_line_length / 2
                                 : effectiveLineLength - 50);

    for (const OptionDescriptionPtr& odPtr : sortedOptions) {
      printedDescriptions.add(odPtr);
    }

    os << this->usagePrefix() << "\n\n" << printedDescriptions << std::endl;
  } else {
    // markdown lines are prepended by two spaces
    const unsigned           effectiveLineLength = MARKDOWN_LINE_LENGTH - 2;
    bpo::options_description printedDescriptions(effectiveLineLength, effectiveLineLength - 50);

    for (const OptionDescriptionPtr& odPtr : sortedOptions) {
      printedDescriptions.add(odPtr);
    }

    std::vector<std::string> lines;
    os << printedDescriptions << std::endl;
    std::string str = os.str();
    boost::algorithm::split(lines, str, boost::algorithm::is_any_of("\n\r"));
    os.str("");

    os << "**Usage**\n\n" << this->usagePrefix() << "\n\n**Options**\n" << std::endl;
    BOOST_FOREACH (const std::string& line, lines) {
      // pre-pend two spaces to the two spaces that boost adds so that we get 4 spaces for markdown to
      // recognise pre-formatted text.
      os << "  " << line << std::endl;
    }
  }
  return os.str();
}

void RckamTrackerOptions::postProcess(bpo::variables_map&)
{
  // nothing at the moment
}

} // namespace tracker
} // namespace rckam

