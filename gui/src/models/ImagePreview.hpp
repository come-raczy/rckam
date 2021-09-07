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

#ifndef MODELS_IMAGE_PREVIEW_HPP
#define MODELS_IMAGE_PREVIEW_HPP

//#include <QGuiApplication>
//#include <QQmlApplicationEngine>
//#include <QQmlEngine>
//#include <QQmlContext>
//#include <QQmlComponent>
//#include <QIcon>
//#include <QDebug>
#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QImage>
#include <QObject>

namespace rckam
{
namespace models
{

/**
 ** \brief the component in charge of capturing and displaying preview images
 **
 ** Uses QImage::loadFromData(const uchar *data, int len, const char *format = nullptr)
 ** Note: only supports jpg
 ** TODO: add image format detection and verification
 ** Usage:
 ** - register the ImagePreview in main (rckam.cpp):
 **   qmlRegisterType<ImageItem>("myextension", 1, 0, "ImagePreview");
 ** - use in QMO (qml/rckam.qml)
 **   import myextension 1.0
 **   ...
 **   ImageItem {
 **     id: liveImageItem
 **     height: parent.height
 **     width: parent.width
 **   }
 **/
class ImagePreview : public QQuickPaintedItem
{
Q_OBJECT
    Q_PROPERTY(QImage image READ image NOTIFY imageChanged)
public:
    ImagePreview(QQuickItem *parent = nullptr);
    Q_INVOKABLE void loadFromData(const uchar *data, int len, const char *format = "JPG");
    void paint(QPainter *painter);
    QImage image() const;
signals:
    void imageChanged();
private:
    QImage image_;
};

} // namespace models
} // namespace rckam

#endif // #ifndef MODELS_IMAGE_PREVIEW_HPP
