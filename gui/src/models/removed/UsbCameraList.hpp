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

#ifndef MODELS_USB_CAMERA_LIST_HPP
#define MODELS_USB_CAMERA_LIST_HPP

#include <QObject>
#include <QAbstractListModel>
#include <QString>
#include <QVector>

#include "devices/UsbPorts.hpp"

namespace rckam
{
namespace models
{

struct UsbCameraModel
{
    int bus;
    int port;
    int address;
    int vendorId;
    int productId;
    QString vendor;
    QString product;
};

class UsbCameraList : public QAbstractListModel
{
    Q_OBJECT
public:
    enum UsbCameraRoles {
        BusRole = Qt::UserRole + 1,
        PortRole,
        AddressRole,
        VendorIdRole,
        ProductIdRole,
        VendorRole,
        ProductRole
    };
    UsbCameraList(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    const UsbCameraModel&camera(unsigned int i) {return usbCameras[i];}
public slots:
    void click();
    void scanPorts(const QModelIndex &parent = QModelIndex());
private:
    QHash<int, QByteArray> roleNames() const override;
    devices::UsbPorts usbPorts;
    QVector<UsbCameraModel> usbCameras;
};

} // namespace models
} // namespace rckam

#endif // #ifndef MODELS_USB_CAMERA_LIST_HPP
