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

#include "RckamWindow.hpp"

#include <QToolBar>
#include <QIcon>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>

namespace rckam
{
namespace gui
{

RckamWindow::RckamWindow(const options::RckamOptions &options, QWidget *parent)
    : QMainWindow(parent)
{


    QPixmap newpix("new.png");
    QPixmap openpix("open.png");
    QPixmap quitpix("quit.png");

    QAction *quit = new QAction("&Quit", this);

    QMenu *file;
    file = menuBar()->addMenu("&File");
    file->addAction(quit);

    //connect(quit, &QAction::triggered, parent, &QApplication::quit);
  
    QToolBar *toolbar = addToolBar("main toolbar");
    toolbar->addAction(QIcon(newpix), "New File");
    toolbar->addAction(QIcon(openpix), "Open File");
    toolbar->addSeparator();
  
    QAction *quit2 = toolbar->addAction(QIcon(quitpix), "Quit Application");
    //connect(quit2, &QAction::triggered, parent, &QApplication::quit);

    QTextEdit *edit = new QTextEdit(this);  

    setCentralWidget(edit);

    statusBar()->showMessage("Ready");

}

} // namespace gui
} // namespace rckam

