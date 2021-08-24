# A set of tools to control remotely a camera and gimbal

## GUI (gui subdirectory)

A Qt based GUI enabling visualization on a remote computer and control
over the camera. The GUI uses libgphoto2, usbip and openwrt to control
remotely any camera supported by libgphoto2 over any WiFi Access Point
(AP) that can run some version of linux, especially openwrt. This
includes many cheap travel routers or even the $10 Raspberry Zero W if
real-time previews are not critical.

Detailed README and INSTALL are available in the `gui` subdirectory.

## Automation (auto subdirectory)

A headless solution to automatically detect and track objects of
interest.

Relies on a Raspberry Pi to control the gimbal, a picamera and a Coral
AI USB accelerator for object detection and classification.

Detailed README and INSTALL are available in the `auto` subdirectory.

## Gimbal (gimbal subdirectory)

Documentation for an example of gimbal with all the parts and detailed
assembly and configuration.

