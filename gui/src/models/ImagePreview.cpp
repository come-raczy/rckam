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

#include "models/ImagePreview.hpp"

namespace rckam
{
namespace models
{

ImagePreview::ImagePreview(QQuickItem *parent)
: image_("/mnt/c/Users/comer/Downloads/no_image.png")
{
  // Note: can't find the image even when registered in qml.qrc
  // this->current_image = QImage(":/images/no_image.png");
}

Q_INVOKABLE void ImagePreview::setImage(const QImage &image)
{
  image_ = image;
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

} // namespace models
} // namespace rckam

