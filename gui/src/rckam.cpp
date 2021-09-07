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
#include "models/ImagePreview.hpp"
#include "devices/ImageLoader.hpp"
//#include "models/Camera.hpp"
//#include "models/UsbCameraList.hpp"
//#include "devices/UsbPorts.hpp"

void rckamGui(const rckam::options::RckamOptions &options);

int main(int argc, char *argv[])
{
  rckam::common::run(rckamGui, argc, argv);
}

void rckamGui(const rckam::options::RckamOptions &options)
{
  std::cerr << "rckamGui: " << options.description << std::endl;
  //rckam::devices::UsbPorts usbPorts;
  //const auto usbDevices = usbPorts.listDevices();
  //std::cerr << "found " << usbDevices.size() << " USB devices:" << std::endl;
  //for (const auto &device: usbDevices)
  //{
  //  const auto d = device.descriptor;
  //  const auto message = boost::format("bus:%2i port:%2i address:%2i type: %4i bcd: 0x%04x class: 0x%02x:0x%02x protocol: 0x%02x vendor: 0x%04x product: 0x%04x %s - %s") % (int)device.busNumber % (int)device.portNumber % (int)device.address % (int)d.bDescriptorType % d.bcdUSB % (int)d.bDeviceClass % (int)d.bDeviceSubClass % d.bDeviceProtocol % d.idVendor % d.idProduct % device.vendor % device.product;
  //  std::cerr << message.str() << std::endl;
  //}

  int argc = 0;
  char arg0[] = "dummy";
  char *argv[] = {arg0};
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  //QQmlContext *objectContext = new QQmlContext(engine.rootContext());

  //rckam::models::Camera camera;
  //rckam::models::UsbCameraList usbCameraList;
  //usbCameraList.scanPorts();

//for (int i = 0; i < usbCameraList.rowCount(); ++i)
//{
//  const auto c = usbCameraList.camera(i);
//  std::cerr << c.product.toUtf8().constData() << std::endl;
//}

  qmlRegisterType<rckam::models::ImagePreview>("myextension", 1, 0, "ImagePreview");
  //QQmlContext *context = engine.rootContext();
  //context->setContextProperty("camera", &camera);
  //context->setContextProperty("usbCameraList", &usbCameraList);

  std::cerr << "Loading rckam.qml" << std::endl;
  engine.load(QUrl(QStringLiteral("qrc:/rckam.qml")));
  if (engine.rootObjects().isEmpty())
    BOOST_THROW_EXCEPTION(rckam::common::RckamException(-1, "engine.rootObjects().isEmpty()"));

  // Select the root object named rckamApplicationWindow
  int index = 0;
  while ((engine.rootObjects().size() > index) && (engine.rootObjects()[index]->objectName().toStdString() != "rckamApplicationWindow"))
  {
    ++index;
  }
  if (engine.rootObjects().size() <= index)
  {
    BOOST_THROW_EXCEPTION(rckam::common::QmlException("ERROR: failed to find rckam Application Window in QML Application Engine"));
  }
  auto rckamApplicationWindow = engine.rootObjects()[index];
  auto imagePreview = rckamApplicationWindow->findChild< rckam::models::ImagePreview *>("imagePreview");
  if (nullptr == imagePreview)
  {
    BOOST_THROW_EXCEPTION(rckam::common::QmlException("ERROR: failed to find image preview in QML Application Engine"));
  }

  // start the background imageLoader
  rckam::devices::ImageLoader imageLoader(imagePreview, options.ipAddress, options.dataPort);
  const auto ret = app.exec();
  if (0!= ret)
  {
    BOOST_THROW_EXCEPTION(rckam::common::RckamException(ret, "QApplication"));
  }
}

