/**
 **
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
 **
 **/

#include <QApplication>
#include <QWidget>

#include "common/Debug.hpp"
#include "common/Exceptions.hpp"
#include "options/RckamOptions.hpp"

void rckamGui(const rckam::options::RckamOptions &options);

int main(int argc, char *argv[])
{
    rckam::common::run(rckamGui, argc, argv);
}

void rckamGui(const rckam::options::RckamOptions &options)
{
    int argc = 0;
    char *argv[] = {"dummy"};
    QApplication app(argc, argv);

    QWidget window;

    window.resize(250, 150);
    window.setWindowTitle("Simple example");
    window.show();

    const auto ret = app.exec();
    if (0!= ret)
    {
        BOOST_THROW_EXCEPTION(rckam::common::RckamException(ret, "QApplication"));
    }
}
