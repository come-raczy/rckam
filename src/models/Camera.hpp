/**
 ** rckam: a Qt remote control for digital cameras
 **
 ** Copyright (C) <year>  <name of author>
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Affero General Public License as
 ** published by the Free Software Foundation, either version 3 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Affero General Public License for more details.
 **
 ** You should have received a copy of the GNU Affero General Public License
 ** along with this program.  If not, see <https://www.gnu.org/licenses/>.
 **/

#ifndef MODELS_CAMERA_HPP
#define MODELS_CAMERA_HPP

//#include <QGuiApplication>
//#include <QQmlApplicationEngine>
//#include <QQmlEngine>
//#include <QQmlContext>
//#include <QQmlComponent>
//#include <QIcon>
//#include <QDebug>
#include <QObject>

namespace rckam
{
namespace models
{

class Camera : public QObject
{
    Q_OBJECT
public slots:
    void click();
};

} // namespace models
} // namespace rckam

#endif // #ifndef MODELS_CAMERA_HPP
