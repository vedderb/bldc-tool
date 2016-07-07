A Qt gui to control and debug my custom BLDC controller. A complete description and tutorial about how to use it can be found here: http://vedder.se/2015/01/vesc-open-source-esc/

Quick build instructions for Ubuntu:

1. `sudo apt-get install qtcreator qt-sdk libudev-dev libqt5serialport5-dev`

2. `qmake -qt=qt5`

3. `make clean && make`

4. Start BLDC-tool `sudo ./BLDC_Tool`

Windows and OS X builds available :

https://bldc-tool.support
