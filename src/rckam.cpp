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

#include <iostream>
#include <boost/format.hpp>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QIcon>
#include <QDebug>

#include "common/Debug.hpp"
#include "common/Exceptions.hpp"
#include "options/RckamOptions.hpp"
#include "models/Camera.hpp"
#include "models/UsbCameraList.hpp"
#include "devices/UsbPorts.hpp"

void rckamGui(const rckam::options::RckamOptions &options);

int main(int argc, char *argv[])
{
    rckam::common::run(rckamGui, argc, argv);
}

void rckamGui(const rckam::options::RckamOptions &options)
{
    rckam::devices::UsbPorts usbPorts;
    const auto usbDevices = usbPorts.listDevices();
    std::cerr << "found " << usbDevices.size() << " USB devices:" << std::endl;
    for (const auto &device: usbDevices)
    {
        const auto d = device.descriptor;
        const auto message = boost::format("bus:%2i port:%2i address:%2i type: %4i bcd: 0x%04x class: 0x%02x:0x%02x protocol: 0x%02x vendor: 0x%04x product: 0x%04x %s - %s") % (int)device.busNumber % (int)device.portNumber % (int)device.address % (int)d.bDescriptorType % d.bcdUSB % (int)d.bDeviceClass % (int)d.bDeviceSubClass % d.bDeviceProtocol % d.idVendor % d.idProduct % device.vendor % device.product;
        std::cerr << message.str() << std::endl;
    }

    int argc = 0;
    char *argv[] = {"dummy"};
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QQmlContext *objectContext = new QQmlContext(engine.rootContext());

    rckam::models::Camera camera;
    rckam::models::UsbCameraList usbCameraList;
    usbCameraList.scanPorts();

for (unsigned int i = 0; i < usbCameraList.rowCount(); ++i)
{
    const auto c = usbCameraList.camera(i);
    std::cerr << c.product.toUtf8().constData() << std::endl;
}


    QQmlContext *context = engine.rootContext();
    context->setContextProperty("camera", &camera);
    context->setContextProperty("usbCameraList", &usbCameraList);

    std::cerr << "Loading rckam.qml" << std::endl;
    engine.load(QUrl(QStringLiteral("qrc:/rckam.qml")));
    if (engine.rootObjects().isEmpty())
        BOOST_THROW_EXCEPTION(rckam::common::RckamException(-1, "engine.rootObjects().isEmpty()"));
    const auto ret = app.exec();
    if (0!= ret)
    {
        BOOST_THROW_EXCEPTION(rckam::common::RckamException(ret, "QApplication"));
    }
}

