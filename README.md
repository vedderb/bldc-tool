BLDC Tool
=========

A [Qt](https://www.qt.io/) GUI to control and debug my custom BLDC controller. A complete description and tutorial about how to use it [can be found here.](http://vedder.se/2015/01/vesc-open-source-esc/)

### Quick build instructions for [Ubuntu](https://www.ubuntu.com/):

1. `sudo apt-get install qtcreator qt-sdk libudev-dev libqt5serialport5-dev`

2. `qmake -qt=qt5`

3. `make clean && make`

4. Allow for serial access without using sudo: `sudo adduser $USER dialout`

5. Restart for access changes to take effect `sudo reboot now`

6. Start BLDC-tool from inside of the built repo `./BLDC_Tool`

### Quick build instructions for [Fedora](https://getfedora.org/)/[CentOS](https://www.centos.org/):

1. `sudo dnf install qtcreator qt-devel qt5-qtserialport-devel`

2. `qmake-qt5`

3. `make clean && make`

4. Allow for serial access without using sudo: `sudo usermod -a -G dialout $USER`

5. Restart for access changes to take effect `sudo reboot now`

6. Start BLDC-tool from inside of the built repo `./BLDC_Tool`
