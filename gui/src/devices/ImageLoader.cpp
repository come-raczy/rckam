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

#include "devices/ImageLoader.hpp"

namespace rckam
{
namespace devices
{

ImageLoader::ImageLoader(models::ImagePreview *imagePreview /* IP address and port */)
{
  QImage image("/mnt/c/Users/comer/Downloads/tmp/preview-00000-19\:32\:41.850.jpg");
  imagePreview->setImage(image);
}

} // namespace devices
} // namespace rckam

