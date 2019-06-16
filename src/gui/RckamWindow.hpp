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

#ifndef GUI_RCKAM_WINDOW_HPP
#define GUI_RCKAM_WINDOW_HPP

#include <QMainWindow>

#include "options/RckamOptions.hpp"

namespace rckam
{
namespace gui
{
class RckamWindow: public QMainWindow 
{
    Q_OBJECT  
public:

  public:
    RckamWindow(const options::RckamOptions &options, QWidget *parent = 0);
    virtual ~RckamWindow() {}
};

} // namespace gui
} // namespace rckam

#endif // #ifndef GUI_RCKAM_WINDOW_HPP

