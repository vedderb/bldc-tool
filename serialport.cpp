/*
    Copyright 2012-2014 Benjamin Vedder	benjamin@vedder.se

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#include <time.h>
#include <signal.h>
#include <errno.h>
#include <QtDebug>

#include <cstdio>   /* Standard input/output definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#ifndef __APPLE__
#include <linux/serial.h>
#endif
#include <sys/ioctl.h>
#include <iostream>

#include "serialport.h"

SerialPort::SerialPort(QObject *parent) :
    QThread(parent)
{
    mIsOpen = false;
    
    mSettings.dataBits = DATA_8;
    mSettings.parity = PARITY_NONE;
    mSettings.stopBits = STOP_1;
    mSettings.baudrate = 115200;
    
    mBufferSize = 32768;
    mReadBuffer = new char[mBufferSize];
    mBufferRead = 0;
    mBufferWrite = 0;
    
    mCaptureBytes = 0;
    mCaptureBuffer = 0;
    mCaptureWrite = 0;
}

SerialPort::~SerialPort()
{
    closePort();
    delete mReadBuffer;
    //std::cout << "SerialPort destructor called";
}

int SerialPort::openPort(
        const QString& port,
        int baudrate,
        SerialDataBits dataBits,
        SerialStopBits stopBits,
        SerialParity parity)
{
    if (mIsOpen) {
        closePort();
    }

    mFd = open(port.toLocal8Bit().data(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (mFd == -1) {
        qCritical() << "Opening serial port failed.";
        return -1;
    }
    
    mIsOpen = true;
    
    // No-blocking reads
    fcntl(mFd, F_SETFL, FNDELAY);
    
    struct termios options;
    if (0 != tcgetattr(mFd, &options)) {
        qCritical() << "Reading serial port options failed.";
        return -2;
    }
    
    // Enable the receiver and set local mode...
    options.c_cflag |= CLOCAL | CREAD;
    
    // Raw input
    options.c_lflag &= ~(ICANON|ECHO|ECHOE|ECHOK|ECHONL|ISIG);
    
    // Disable flow control
    options.c_cflag &= ~CRTSCTS;
    options.c_iflag &= ~(IXON|IXOFF|IXANY);
    
    // ???
    /*set up other port settings*/
    options.c_cflag |= CREAD|CLOCAL;
    options.c_lflag &= (~(ICANON|ECHO|ECHOE|ECHOK|ECHONL|ISIG));
    options.c_iflag &= (~(INPCK|IGNPAR|PARMRK|ISTRIP|ICRNL|IXANY));
    options.c_oflag &= (~OPOST);
    options.c_cc[VMIN] = 0;
#ifdef _POSIX_VDISABLE  // Is a disable character available on this system?
    // Some systems allow for per-device disable-characters, so get the
    //  proper value for the configured device
    const long vdisable = ::fpathconf(mFd, _PC_VDISABLE);
    options.c_cc[VINTR] = vdisable;
    options.c_cc[VQUIT] = vdisable;
    options.c_cc[VSTART] = vdisable;
    options.c_cc[VSTOP] = vdisable;
    options.c_cc[VSUSP] = vdisable;
#endif //_POSIX_VDISABLE
    
    //Set the new options for the port...
    if (0 != tcsetattr(mFd, TCSANOW, &options)) {
        qCritical() << "Writing serial port options failed.";
        closePort();
        return -3;
    }
    
    if (false == setDataBits(dataBits)) {
        qCritical() << "Setting data bits failed.";
        closePort();
        return -4;
    }

    if (false == setStopBits(stopBits)) {
        qCritical() << "Setting stopbits failed.";
        closePort();
        return -5;
    }

    if (false == setParity(parity)) {
        qCritical() << "Setting parity faield.";
        closePort();
        return -6;
    }

    if (false == setBaudrate(baudrate)) {
        qCritical() << "Setting baudrate failed.";
        closePort();
        return -7;
    }
    
    mAbort = false;
    start(LowPriority);
    return 0;
}

bool SerialPort::isOpen()
{
    return mIsOpen;
}

void SerialPort::closePort()
{
    struct termios options;
    tcgetattr(mFd, &options);
    options.c_cc[VTIME] = 1;
    options.c_cc[VMIN] = 0;
    tcsetattr(mFd, TCSANOW, &options);
    
    mMutex.lock();
    mAbort = true;
    mCondition.wakeOne();
    mMutex.unlock();
    
    wait();
    
    mMutex.lock();
    if (mIsOpen) {
        close(mFd);
        mIsOpen = false;
    }
    mMutex.unlock();
}

bool SerialPort::setBaudrate(int baudrate)
{
    if (!mIsOpen) {
        qCritical() << "Serial port not open.";
        return false;
    }
    
    speed_t baud = 0;
    switch (baudrate) {
    case      50: baud =      B50; break;
    case      75: baud =      B75; break;
    case     110: baud =     B110; break;
    case     134: baud =     B134; break;
    case     150: baud =     B150; break;
    case     200: baud =     B200; break;
    case     300: baud =     B300; break;
    case     600: baud =     B600; break;
    case    1200: baud =    B1200; break;
    case    1800: baud =    B1800; break;
    case    2400: baud =    B2400; break;
    case    4800: baud =    B4800; break;
    case    9600: baud =    B9600; break;
    case   19200: baud =   B19200; break;
    case   38400: baud =   B38400; break;
    case   57600: baud =   B57600; break;
    case  115200: baud =  B115200; break;
    case  230400: baud =  B230400; break;
    #ifndef __APPLE__
    case  460800: baud =  B460800; break;
    case  500000: baud =  B500000; break;
    case  576000: baud =  B576000; break;
    case  921600: baud =  B921600; break;
    case 1000000: baud = B1000000; break;
    case 1152000: baud = B1152000; break;
    case 1500000: baud = B1500000; break;
    case 2000000: baud = B2000000; break;
    case 2500000: baud = B2500000; break;
    case 3000000: baud = B3000000; break;
    case 3500000: baud = B3500000; break;
    case 4000000: baud = B4000000; break;
    #endif
    default:      baud = 0; break;
    }
    
    struct termios options;
    if (0 != tcgetattr(mFd, &options)) {
        qCritical() << "Reading serial port options failed.";
        return false;
    }
    
    #ifndef __APPLE__
    serial_struct ser_info;
    
    if (0 == baud) {
        if (0 != ioctl(mFd, TIOCGSERIAL, &ser_info)) {
            qCritical() << "Reading ser_info struct failed. Try a standard baudrate.";
            return false;
        }
        
        ser_info.flags &= ~ASYNC_SPD_MASK;
        ser_info.flags |= ASYNC_SPD_CUST | ASYNC_LOW_LATENCY;
        ser_info.custom_divisor = ser_info.baud_base / baudrate;
        qDebug() << "Baud base: " << ser_info.baud_base;
        
        cfsetspeed(&options, B38400);
        if (0 != tcsetattr(mFd, TCSANOW, &options)) {
            qCritical() << "Writing serial port options failed";
            return false;
        }
        
        if (0 != ioctl(mFd, TIOCSSERIAL, &ser_info)) {
            qCritical() << "Writing ser_info struct failed";
            return false;
        }
        
        qDebug() << "Custom type baudrate set";
    } else {
        cfsetspeed(&options, baud);
        if (0 != tcsetattr(mFd, TCSANOW, &options)) {
            qCritical() << "Writing serial port options failed";
            return false;
        }
        
        qDebug() << "Standard type baudrate set";
        
        if (0 != ioctl(mFd, TIOCGSERIAL, &ser_info)) {
            qWarning() << "Reading ser_info struct failed.";
        } else {
            ser_info.flags &= ~ASYNC_SPD_CUST;
            ser_info.custom_divisor = 0;
            qDebug() << "Baud base: " << ser_info.baud_base;
            if (0 != ioctl(mFd, TIOCSSERIAL, &ser_info)) {
                qCritical() << "Writing ser_info struct failed";
            }
        }
    }
    
    #endif
    mSettings.baudrate = baudrate;
    return true;
}

bool SerialPort::setDataBits(SerialDataBits dataBits)
{
    if (!mIsOpen) {
        qCritical() << "Serial port not open.";
        return false;
    }
    
    struct termios options;
    if (0 != tcgetattr(mFd, &options)) {
        qCritical() << "Reading serial port options failed.";
        return false;
    }
    
    options.c_cflag &= ~CSIZE; /* Mask the character size bits */
    
    switch (dataBits) {
    case DATA_5:
        options.c_cflag |= CS5;
        break;
        
    case DATA_6:
        options.c_cflag |= CS6;
        break;
        
    case DATA_7:
        options.c_cflag |= CS7;
        break;
        
    default:
        options.c_cflag |= CS8;
        break;
    }
    
    if (0 != tcsetattr(mFd, TCSANOW, &options)) {
        qCritical() << "Writing serial port options failed.";
        return false;
    }
    
    mSettings.dataBits = dataBits;
    return true;
}

bool SerialPort::setStopBits(SerialStopBits stopBits)
{
    if (!mIsOpen) {
        qCritical() << "Serial port not open.";
        return false;
    }
    
    struct termios options;
    if (0 != tcgetattr(mFd, &options)) {
        qCritical() << "Reading serial port options failed.";
        return false;
    }
    
    switch (stopBits) {
    case STOP_2:
        options.c_cflag |= CSTOPB;
        break;
        
    default:
        options.c_cflag &= ~CSTOPB;
        break;
    }
    
    if (0 != tcsetattr(mFd, TCSANOW, &options)) {
        qCritical() << "Writing serial port options failed.";
        return false;
    }
    
    mSettings.stopBits = stopBits;
    return true;
}

bool SerialPort::setParity(SerialParity parity)
{
    if (!mIsOpen) {
        qCritical() << "Serial port not open.";
        return false;
    }
    
    struct termios options;
    if (0 != tcgetattr(mFd, &options)) {
        qCritical() << "Reading serial port options failed.";
        return false;
    }
    
    switch (parity) {
    case PARITY_EVEN:
        options.c_cflag |= PARENB;
        options.c_cflag &= ~PARODD;
        break;
        
    case PARITY_ODD:
        options.c_cflag |= PARENB;
        options.c_cflag |= PARODD;
        break;
        
    default:
        options.c_cflag &= ~PARENB;
        break;
        
    }
    
    if (0 != tcsetattr(mFd, TCSANOW, &options)) {
        qCritical() << "Writing serial port options failed.";
        return false;
    }
    
    mSettings.parity = parity;
    return true;
}

bool SerialPort::readByte(char &byte)
{
    if (!mIsOpen) {
        qCritical() << "Serial port not open.";
        return false;
    }
    
    {
        QMutexLocker locker(&mMutex);
        if (mBufferRead == mBufferWrite) {
            return false;
        }
        
        char tmp = mReadBuffer[mBufferRead];
        mBufferRead++;
        
        if (mBufferRead == mBufferSize) {
            mBufferRead = 0;
        }
        
        byte = tmp;
        return true;
    }
}

int SerialPort::readBytes(char* buffer, int bytes)
{
    if (!mIsOpen) {
        qCritical() << "Serial port not open.";
        return -1;
    }
    
    {
        QMutexLocker locker(&mMutex);
        for (int i = 0;i < bytes;i++) {
            if (mBufferRead == mBufferWrite) {
                return i;
            }
            
            buffer[i] = mReadBuffer[mBufferRead];
            mBufferRead++;
            if (mBufferRead == mBufferSize) {
                mBufferRead = 0;
            }
        }
    }
    
    return bytes;
}

int SerialPort::readString(QString& string, int length)
{
    if (!mIsOpen) {
        qCritical() << "Serial port not open.";
        return -1;
    }
    
    string = "";
    
    QMutexLocker locker(&mMutex);
    for (int i = 0;i < length;i++) {
        if (mBufferRead == mBufferWrite) {
            return i;
        }
        
        if ('\0' != mReadBuffer[mBufferRead]) {
            string.append(mReadBuffer[mBufferRead]);
        }
        
        mBufferRead++;
        if (mBufferRead == mBufferSize) {
            mBufferRead = 0;
        }
    }
    
    return length;
}

QByteArray SerialPort::readAll()
{
    if (!mIsOpen) {
        qCritical() << "Serial port not open.";
        return 0;
    }
    
    int len = bytesAvailable();
    char data[len];
    
    readBytes(data, len);
    QByteArray bytes;
    bytes.clear();
    bytes.append(data, len);
    
    return bytes;
}

int SerialPort::writeData(const char *data, int length, bool block)
{
    if (!mIsOpen) {
        qCritical() << "Serial port not open.";
        return -2;
    }
    
    int res = 0;
    int written = 0;
    fd_set set;
    timespec timeout;
    
    if (block) {
        while (length) {
            FD_ZERO(&set); /* clear the set */
            FD_SET(mFd, &set); /* add our file descriptor to the set */
            timeout.tv_sec = 0;
            timeout.tv_nsec = 1000000;
            res = pselect(mFd + 1, NULL, &set, NULL, &timeout, NULL);
            
            if(res < 0) {
                qCritical().nospace() << "PSelect failed in writeData (" << res << "), ignoring";
                //return res;
            } else if(res == 0) {
                // Timeout
            } else {
                res = write(mFd, data + written, length);
                if (res >= 0) {
                    length -= res;
                    written += res;
                } else {
                    qCritical().nospace() << "Writing to serial port failed (" << res << "), ignoring";
                    //return res;
                }
            }
            
            if (!mIsOpen) {
                qCritical() << "Serial port closed during write";
                return -2;
            }
        }
        return written;
    } else {
        return write(mFd, data, length);
    }
}

bool SerialPort::writeByte(char byte, bool block)
{
    if (!mIsOpen) {
        qCritical() << "Serial port not open.";
        return false;
    }
    
    if (1 == writeData(&byte, 1, block)) {
        return true;
    }
    
    return false;
}

int SerialPort::bytesAvailable()
{
    if (!mIsOpen) {
        qCritical() << "Serial port not open.";
        return -1;
    }
    
    {
        QMutexLocker locker(&mMutex);
        if (mBufferRead <= mBufferWrite) {
            return mBufferWrite - mBufferRead;
        } else {
            return (mBufferSize - 1) - mBufferRead + mBufferWrite;
        }
    }
}

bool SerialPort::writeString(const QString& string, bool block)
{
    if (string.length() == writeData(string.toLocal8Bit().data(), string.length(), block)) {
        return true;
    } else {
        return false;
    }
}

int SerialPort::captureBytes(char *buffer, int num, int timeoutMs, const QString& preTransmit)
{
    if (!mIsOpen) {
        qCritical() << "Serial port not open.";
        return -1;
    }
    
    {
        QMutexLocker locker(&mMutex);
        mCaptureBuffer = buffer;
        mCaptureBytes = num;
        mCaptureWrite = 0;
    }
    
    if (preTransmit.length() > 0) {
        if (!writeString(preTransmit)) {
            return -2;
        }
    }
    
    mMutex.lock();
    mCondition.wait(&mMutex, timeoutMs);
    mMutex.unlock();
    
    {
        QMutexLocker locker(&mMutex);
        mCaptureBytes = 0;
        return mCaptureWrite;
    }
}

int SerialPort::captureBytes(char *buffer, int num, int timeoutMs, const char *preTransmit, int preTransLen)
{
    if (!mIsOpen) {
        qCritical() << "Serial port not open.";
        return -1;
    }
    
    {
        QMutexLocker locker(&mMutex);
        mCaptureBuffer = buffer;
        mCaptureBytes = num;
        mCaptureWrite = 0;
    }
    
    if (preTransLen > 0) {
        if (0 > writeData(preTransmit, preTransLen)) {
            return -2;
        }
    }
    
    mMutex.lock();
    mCondition.wait(&mMutex, timeoutMs);
    mMutex.unlock();
    
    {
        QMutexLocker locker(&mMutex);
        mCaptureBytes = 0;
        return mCaptureWrite;
    }
}

void SerialPort::run()
{
    unsigned char buffer[1024];
    int res = 0;
    int failed_reads = 0;
    fd_set set;
    timespec timeout;
    
    while (false == mAbort) {
        FD_ZERO(&set); /* clear the set */
        FD_SET(mFd, &set); /* add our file descriptor to the set */
        timeout.tv_sec = 0;
        timeout.tv_nsec = 10000000;
        res = pselect(mFd + 1, &set, NULL, NULL, &timeout, NULL);
        
        if(res < 0) {
            qWarning() << "Select failed in read thread";
        } else if(res == 0) {
            // Timeout
        } else {
            res = read(mFd, buffer, 1024);
            if (res > 0) {
                failed_reads = 0;
                
                QMutexLocker locker(&mMutex);
                for (int i = 0;i < res;i++) {
                    if (mCaptureBytes > 0) {
                        if (mCaptureBuffer != 0) {
                            mCaptureBuffer[mCaptureWrite] = buffer[i];
                            mCaptureWrite++;
                        }
                        mCaptureBytes--;
                        if (mCaptureBytes == 0) {
                            mCondition.wakeOne();
                        }
                    } else {
                        mReadBuffer[mBufferWrite] = buffer[i];
                        mBufferWrite++;
                        
                        if (mBufferWrite == mBufferSize) {
                            mBufferWrite = 0;
                        }
                        Q_EMIT serial_data_available();
                    }
                }
            } else {
                if (res < 0) {
                    qCritical().nospace() << "Reading failed. MSG: " << strerror(errno);
                } else {
                    qCritical().nospace() << "Reading serial port returned 0";
                }
                
                failed_reads++;
                
                if (failed_reads > 3) {
                    QMutexLocker locker(&mMutex);
                    if (mIsOpen) {
                        close(mFd);
                        mIsOpen = false;
                    }
                    mAbort = true;
                    qCritical().nospace() << "Too many consecutive failed reads. Closing port.";
                    Q_EMIT serial_port_error(res);
                    return;
                }
            }
        }
    }
}

