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

#ifndef RCKAM_SERVER_GPHOTO2_CONTEXT_HPP
#define RCKAM_SERVER_GPHOTO2_CONTEXT_HPP

#include <gphoto2/gphoto2-context.h>

namespace rckam
{
namespace server
{

class Gphoto2Context
{
public:
  Gphoto2Context();
  ~Gphoto2Context();
  operator GPContext *() {return context_;}
private:
  GPContext *context_;
  /// ref_count not exposed by gphoto2!
  unsigned refCount_;
};

} // namespace server
} // namespace rckam

#endif // #ifndef RCKAM_SERVER_GPHOTO2_CONTEXT_HPP
