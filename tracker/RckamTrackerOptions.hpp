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

#ifndef RCKAM_AUTO_RCKAM_AUTO_OPTIONS_HPP
#define RCKAM_AUTO_RCKAM_AUTO_OPTIONS_HPP

#include <boost/core/noncopyable.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/program_options.hpp>

namespace rckam
{
namespace tracker
{

#define STRINGIFY(s) XSTRINGIFY(s)
#define XSTRINGIFY(s) #s



namespace bpo = boost::program_options;

class RckamTrackerOptions : boost::noncopyable
{
public:
  enum Action { RUN, HELP, VERSION, ABORT };
  RckamTrackerOptions();
  virtual ~RckamTrackerOptions() {}
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
  static std::string version() { return std::string(std::string("rckam-tracker version ") + STRINGIFY(RCKAM_VERSION) + "-" + STRINGIFY(VERSION_STRING)); }
  
  /// ADC channel for the joystick switch
  unsigned switchChannel = 7;
  /// ADC channel for X control
  unsigned vrxChannel = 6;
  /// ADC channel for Y control
  unsigned vryChannel = 5;
  /// GPIO pin to TB6612FNG AIN1
  unsigned ain1 = 16; // BCM 16 == BOARD 36
  /// GPIO pin to TB6612FNG AIN2
  unsigned ain2 = 20; // BCM 20 == BOARD 38
  /// GPIO pin to TB6612FNG PWMA
  unsigned pwmA = 13; // BCM 13 == BOARD 33 == PWM1
  /// GPIO pin to TB6612FNG BIN1
  unsigned bin1 = 14; // BCM 14 == BOARD  8 == TXD
  /// GPIO pin to TB6612FNG BIN2
  unsigned bin2 = 15; // BCM 15 == BOARD 10 == RXD
  /// GPIO pin to TB6612FNG PWMB
  unsigned pwmB = 18; // BCM 18 == BOARD 12 == PWM0
  /// GPIO pin to TB6612FNG STBY
  unsigned stby = 23; // BCM 23 == BOARD 16
  /// PWM frequency for HW PWM control of the motors
  unsigned pwmFreq = 200;
  /// GPIO pin from channel A of encoder for pan motor
  unsigned panA = 2; // BCM 2 == BOARD 3
  /// GPIO pin from channel B of encoder for pan motor
  unsigned panB = 3; // BCM 3 == BOARD 5
  /// GPIO pin from channel A of encoder for tilt motor
  unsigned tiltA = 19; // BCM 19 == BOARD 35 == MISO
  /// GPIO pin from channel B of encoder for tilt motor
  unsigned tiltB = 26; // BCM 26 == BOARD 37
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
  std::string usagePrefix() const {return "rckam-tracker [options]";}
  std::string usageSuffix() const { return ""; }
  void        postProcess(bpo::variables_map&);

  static const unsigned MARKDOWN_LINE_LENGTH = 120;
  // "parse" will store the state in vm_ so that "usage" can access the details of parsed command line
  bpo::variables_map vm_;
  int                argc_ = 0;
  const char* const* argv_ = 0;
};

} // namespace tracker
} // namespace rckam

#endif // #ifndef RCKAM_AUTO_RCKAM_AUTO_OPTIONS_HPP

