# Communication with the camera

## Requirements
   ============

- libgphoto2-dev (>=5.26)
- libboost-dev (>=1.67.0.1)

## Overview
   ========

This is a server running on the Raspberry Pi.
The communicaton to the client has two aspects:

- information about the camera (capabilities and settings)
- preview images

The communication from the client has two aspects:

- commands to the camera
- commands to the mount

All communications are made over sockets. There will be one dedicated to
the images only. There will be another one for all the other information
with an appropriate protocol to dispatch the commands where the y belong.

