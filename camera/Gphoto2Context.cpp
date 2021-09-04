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

#include "Gphoto2Context.hpp"

#include <iostream>

namespace rckam
{
namespace camera
{

Gphoto2Context::Gphoto2Context()
: context_(gp_context_new())
, refCount_(1)
{
}

Gphoto2Context::~Gphoto2Context()
{
  while (refCount_)
  {
    gp_context_unref(context_);
    --refCount_;
  }
}

} // namespace camera
} // namespace rckam

