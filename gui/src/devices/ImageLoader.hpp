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

#ifndef DEVICES_IMAGE_LOADER_HPP
#define DEVICES_IMAGE_LOADER_HPP

//#include <QGuiApplication>
//#include <QQmlApplicationEngine>
//#include <QQmlEngine>
//#include <QQmlContext>
//#include <QQmlComponent>
//#include <QIcon>
//#include <QDebug>
//#include <QQuickPaintedItem>
//#include <QQuickItem>
//#include <QPainter>
//#include <QImage>

#include "models/ImagePreview.hpp"

namespace rckam
{
namespace devices
{

/**
 ** \brief component in charge of capturing the image previews
 **
 ** Reads the data from the socket and use it to initialize a QImage instance
 ** with QImage::loadFromData.
 ** QImage::loadFromData(const uchar *data, int len, const char *format = nullptr)
 ** Note: only supports jpg
 ** Once the QImage instance is initialized, passes it to ImagePreview
 ** TODO: add image format detection and verification
 **/
class ImageLoader
{
public:
    ImageLoader(models::ImagePreview *imagePreview /* IP address and port */);
private:
};

} // namespace devices
} // namespace rckam

#endif // #ifndef DEVICES_IMAGE_LOADER_HPP
