# Open-source Camera Remote Control

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

This will generate the binary opencamrc in the subdirectory bin.

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


