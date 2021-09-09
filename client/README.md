# A Qt remote control for digital cameras

There are several ways to control a camera remotely. The simplest might be to
use usbip to export the USB port where the camera is connected. The main issues
with this approach are:

- usbip might not be freely availableon the client (e.g. MS Windows, Android).
- the client might not even have access to the USB port (e.g. WSL)
- no control on the part of data transfer that needs to be optimized (previews)

The alternative is to run the control of the camera locally and to communicate
with the client over sockets. It is a little bit more work but that's a small
price for portability and - more importantly - lower latency in the previews.

This is the GUI on the client side. It is connected via a pair of sockets to
the server where the camera is. The sockets are:

- a dedicated UDP socket for image data exclusively
- a TCP/IP socket for communication and synchronization

The GUI is divided into three domains of functionalities

- a preview area that displays the image previews as they are captured
- a camera control and status area
- a gimbal control area

Note that some of the camera controls and the gimbal controls might overlap
the preview area (e.g. pointing the gimbal into the right direction, selecting
autofocus areas, etc.)

## Requirements
   ============

* qtcreator (>= 4.11.0)
* qt-5-default (>= 5.12.8)
* qml (>= 5.12.8)
* qtdeclarative5-dev (>= 5.12.8)
* qml-module-qtquick-controls2 (>= 5.12.8)
* libqt5svg5-dev (>= 5.12.8)
* Cmake (>= 3.18.1)
* boost (>= 1.71)

# Obsolete
  ========

This project uses libgphoto2, usbip and openwrt to control remotely any
camera supported by libgphoto2 over any WiFi Access Point (AP) that can
run some version of linux, especially openwrt. This includes many cheap
travel routers or even the $10 Raspberry Zero W if real-time previews
are not critical.

There are already a very large number of solutions that enable some form of
wireless control of digital cameras but the vast majority are proprietary
solutions. In addition, most existing solutions implement ad hoc server
side tools for tunelling the commands and data to and from the camera. This
software leverages existing general-purpose UST To Network tools instead.

In addition to being open source, this solution also targets strong aspects
of automation and the integration with other systems for situations where
the camera is used with additional hardware such as gimbals, rails, focusers,
triggers, lighting, etc.

## Requirements
   ============

* Server:
  * usbip or any alternative virtualization of USB ports
* Client:
  * libgphoto2 including support for usb and ptp
  * usbip
  * Qt5

## Build and install
   =================

There isn't any specific software to build on the server side. See in the
documentation examples of router configuration to expose the router USB ports
to the client.

Client and server are likely to run on radically different platforms and
might require specific packaging for deployment. For instance, the client
could be built as an Android app while the server would be an openwrt
package of a specific device.

### Client
    ======

When building on the target system, simply type:

```
make client
```

This will generate the binary rckam in the subdirectory bin.

When building for a different target, the correct compiler flags must be
specified in the environment or on the make command line.

### Server
    ======

In the rare situation where the server is built on the target system, simply
type:

```
make server
```

In most cases the server will run on routers or APs running some version of
openwrt. In this case, the options are:
* downloading and installing a pre-built package if it exists for the targety device
* building for the target device using the SDK for the appropriate version of openwrt


