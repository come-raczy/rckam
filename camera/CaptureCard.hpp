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

#ifndef RCKAM_CAMERA_CAPTURE_CARD_HPP
#define RCKAM_CAMERA_CAPTURE_CARD_HPP

#include <vector>
#include <string>

namespace rckam
{
namespace camera
{

/**
 ** \brief simple encapsulation of capture cards
 **/
class CaptureCard
{
public:
  /// create a device from all its data
  CaptureCard(const std::string &busInfo, const std::string &card);
  /// this class is meant to be specialized
  virtual ~CaptureCard();
  /// add a device to the capture card
  void addDevNode(const std::string &devNode);
  /// get the bus info
  const std::string &busInfo() const {return busInfo_;}
  /// get the card
  const std::string &card() const {return card_;}
  /// get the devices
  const std::vector<std::string> devNodes() const {return devNodes_;}
  /**
   ** \brief list all capture cards
   **
   ** Capture cards are video devices with ":capture:" in their list of
   ** ID_V4L_CAPABILITIES. As each card can potentially be connected to
   ** multiple userspace devices, the suitable devices need further 
   ** analysis to group them correctly.
   ** 
   **/
  static std::vector<CaptureCard> detect();
protected:
  std::string busInfo_;
  std::string card_;
  std::vector<std::string> devNodes_;
};

} // namespace camera
} // namespace rckam

#endif // #ifndef RCKAM_CAMERA_CAPTURE_CARD_HPP

