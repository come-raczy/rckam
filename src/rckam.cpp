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

void rckamGui(const rckam::options::RckamOptions &options);

int main(int argc, char *argv[])
{
    rckam::common::run(rckamGui, argc, argv);
}

void rckamGui(const rckam::options::RckamOptions &options)
{
    int argc = 0;
    char *argv[] = {"dummy"};
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QQmlContext *objectContext = new QQmlContext(engine.rootContext());

    rckam::models::Camera camera;
    engine.rootContext()->setContextProperty("camera", &camera);
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

