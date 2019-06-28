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

#include "models/UsbCameraList.hpp"
#include <QtAlgorithms>

#include <iostream>

namespace rckam
{
namespace models
{

UsbCameraList::UsbCameraList(QObject *parent)
    : QAbstractListModel(parent)
{
}

void UsbCameraList::click() {std::cerr << "Camera.click()" << std::endl;}
QHash<int, QByteArray> UsbCameraList::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[BusRole] = "bus";
    roles[PortRole] = "port";
    roles[AddressRole] = "address";
    roles[VendorIdRole] = "vendorId";
    roles[ProductIdRole] = "productId";
    roles[VendorRole] = "vendor";
    roles[ProductRole] = "product";
    return roles;
}

int UsbCameraList::rowCount(const QModelIndex &) const
{
    return usbCameras.size();
}

QVariant UsbCameraList::data(const QModelIndex &index, int role) const
{
    if (index.row() >= rowCount(index)) return QVariant();
    switch(role)
    {
        case BusRole: return usbCameras[index.row()].bus;
        case PortRole: return usbCameras[index.row()].port;
        case AddressRole: return usbCameras[index.row()].address;
        case VendorIdRole: return usbCameras[index.row()].vendorId;
        case ProductIdRole: return usbCameras[index.row()].productId;
        case VendorRole: return usbCameras[index.row()].vendor;
        case ProductRole: return usbCameras[index.row()].product;
        default: return QVariant();
    }
}

bool operator<(const UsbCameraModel &l, const UsbCameraModel r)
{
    return 
        (l.bus < r.bus) ||
        (l.bus == r.bus && l.port < r.port) ||
        (l.bus == r.bus && l.port == r.port && l.address < r.address) ||
        (l.bus == r.bus && l.port == r.port && l.address == r.address && l.vendorId < r.vendorId) ||
        (l.bus == r.bus && l.port == r.port && l.address == r.address && l.vendorId == r.vendorId && l.productId < r.productId);
}

bool operator==(const UsbCameraModel &l, const UsbCameraModel r)
{
    return l.bus == r.bus && l.port == r.port && l.address == r.address && l.vendorId == r.vendorId && l.productId == r.productId;
}

void UsbCameraList::scanPorts(const QModelIndex &parent)
{
    const auto devices = usbPorts.listDevices();
    QVector<UsbCameraModel> newList;
    for (auto &d: devices)
    {
        newList.push_back(UsbCameraModel{d.busNumber, d.portNumber, d.address, d.descriptor.idVendor, d.descriptor.idProduct, QString::fromStdString(d.vendor), QString::fromStdString(d.product)});
    }
    qSort(newList);
    auto iNew = newList.begin();
    auto iOld = usbCameras.begin();
    while(newList.end() != iNew && usbCameras.end() != iOld)
    {
        if (*iNew == *iOld)
        {
            ++iNew;
            ++iOld;
        }
        else if (*iNew < *iOld)
        {
            // add iNew before iOld
            const auto i = iOld - usbCameras.begin();
            beginInsertRows(parent, i, i);
            if (usbCameras.begin() == iOld)
            {
                usbCameras.prepend(*iNew);
                iOld = usbCameras.begin();
            } else {
               iOld = usbCameras.insert(iOld - 1, *iNew);
            }
            endInsertRows();
            ++iOld;
            ++iNew;
        }
        else // iNew > iOld
        {
            // remove iOld
            const auto i = iOld - usbCameras.begin();
            beginRemoveRows(parent, i, i);
            iOld = usbCameras.erase(iOld);
            endRemoveRows();
        }
    }
    if (usbCameras.end() != iOld)
    {
        beginRemoveRows(parent, iOld - usbCameras.begin(), usbCameras.size() -1);
        usbCameras.erase(iOld, usbCameras.end());
        endRemoveRows();
    }
    if (newList.end() != iNew)
    {
        const auto count = newList.end() - iNew;
        beginInsertRows(parent, usbCameras.size(), usbCameras.size() + count - 1);
        while (newList.end() != iNew)
        {
            usbCameras.append(*iNew);
            ++iNew;
        }
        endInsertRows();
    }
}

} // namespace models
} // namespace rckam

//#include "UsbCameraList.moc"

