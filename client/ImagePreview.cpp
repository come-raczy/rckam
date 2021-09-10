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

#include "client/ImagePreview.hpp"

#include <filesystem>
#include <fstream>
#include <vector>
#include <iostream>
#include <boost/asio.hpp>

namespace rckam
{
namespace client
{

ImagePreview::ImagePreview(QQuickItem *)
: image_(":/images/no_image.png")
{

#if 0
std::filesystem::path filePath = "/tmp/preview0000.jpg";
const size_t fileSize = file_size(filePath);
std::vector<char> data(fileSize);
std::ifstream is(filePath);
if (!is.read(data.data(), fileSize))
{
  std::cerr << "failed to read  " << filePath << std::endl;
}
std::cerr << "read " << is.gcount() << " expected " << fileSize << std::endl;
//std::vector<uchar> udata(fileSize);
//for (unsigned int i = 0; data.size() > i; ++i)
//{
//  udata[i] = static_cast<uchar>(data[i]);
//}
//image_.loadFromData(data.data(), fileSize, "JPG");
image_.loadFromData(reinterpret_cast<const uchar *>(data.data()), fileSize, "JPG");
update();
#endif

}

Q_INVOKABLE void ImagePreview::loadFromData(const uchar *data, int len, const char *format)
{
  image_.loadFromData(data, len, format);
  update();
}

void ImagePreview::paint(QPainter *painter)
{
    QRectF bounding_rect = boundingRect();
    QImage scaled = image_.scaledToHeight(bounding_rect.height());
    QPointF center = bounding_rect.center() - scaled.rect().center();

    if(center.x() < 0)
        center.setX(0);
    if(center.y() < 0)
        center.setY(0);
   painter->drawImage(center, scaled);
}

QImage ImagePreview::image() const
{
  return image_;
}

} // namespace client
} // namespace rckam

