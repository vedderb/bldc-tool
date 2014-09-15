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

#include "packetinterface.h"
#include "utility.h"
#include <QDebug>
#include <math.h>

namespace {
// CRC Table
const unsigned short crc16_tab[] = { 0x0000, 0x1021, 0x2042, 0x3063, 0x4084,
        0x50a5, 0x60c6, 0x70e7, 0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad,
        0xe1ce, 0xf1ef, 0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7,
        0x62d6, 0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
        0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a,
        0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d, 0x3653, 0x2672,
        0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719,
        0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 0x48c4, 0x58e5, 0x6886, 0x78a7,
        0x0840, 0x1861, 0x2802, 0x3823, 0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948,
        0x9969, 0xa90a, 0xb92b, 0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50,
        0x3a33, 0x2a12, 0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b,
        0xab1a, 0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
        0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49, 0x7e97,
        0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70, 0xff9f, 0xefbe,
        0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78, 0x9188, 0x81a9, 0xb1ca,
        0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f, 0x1080, 0x00a1, 0x30c2, 0x20e3,
        0x5004, 0x4025, 0x7046, 0x6067, 0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d,
        0xd31c, 0xe37f, 0xf35e, 0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214,
        0x6277, 0x7256, 0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c,
        0xc50d, 0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
        0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3,
        0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634, 0xd94c, 0xc96d,
        0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806,
        0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3, 0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e,
        0x8bf9, 0x9bd8, 0xabbb, 0xbb9a, 0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1,
        0x1ad0, 0x2ab3, 0x3a92, 0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b,
        0x9de8, 0x8dc9, 0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0,
        0x0cc1, 0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
        0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0 };
}

PacketInterface::PacketInterface(QObject *parent) :
    QObject(parent)
{
    mSendBuffer = new quint8[255];

    mRxState = 0;
    mRxTimer = 0;

    mPayloadLength = 0;
    mRxDataPtr = 0;
    mCrcLow = 0;
    mCrcHigh = 0;

    mTimer = new QTimer(this);
    mTimer->setInterval(10);
    mTimer->start();

    connect(mTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
}

PacketInterface::~PacketInterface()
{
    delete mSendBuffer;
}

void PacketInterface::processData(QByteArray &data)
{
    unsigned char rx_data;
    const int rx_timeout = 50;

    for(int i = 0;i < data.length();i++) {
        rx_data = data[i];

        switch (mRxState) {
        case 0:
            if (rx_data == 2) {
                mRxState++;
                mRxTimer = rx_timeout;
                mRxDataPtr = 0;
            } else {
                mRxState = 0;
            }
            break;

        case 1:
            mPayloadLength = rx_data;
            mRxState++;
            mRxTimer = rx_timeout;
            break;

        case 2:
            mRxBuffer[mRxDataPtr++] = rx_data;
            if (mRxDataPtr == mPayloadLength) {
                mRxState++;
            }
            mRxTimer = rx_timeout;
            break;

        case 3:
            mCrcHigh = rx_data;
            mRxState++;
            mRxTimer = rx_timeout;
            break;

        case 4:
            mCrcLow = rx_data;
            mRxState++;
            mRxTimer = rx_timeout;
            break;

        case 5:
            if (rx_data == 3) {
                if (crc16(mRxBuffer, mPayloadLength) ==
                        ((unsigned short)mCrcHigh << 8 | (unsigned short)mCrcLow)) {
                    // Packet received!
                    processPacket(mRxBuffer, mPayloadLength);
                }
            }

            mRxState = 0;
            break;

        default:
            mRxState = 0;
            break;
        }
    }
}

void PacketInterface::timerSlot()
{
    if (mRxTimer) {
        mRxTimer--;
    } else {
        mRxState = 0;
    }
}

unsigned short PacketInterface::crc16(const unsigned char *buf, unsigned int len)
{
    unsigned int i;
    unsigned short cksum = 0;
    for (i = 0; i < len; i++) {
        cksum = crc16_tab[(((cksum >> 8) ^ *buf++) & 0xFF)] ^ (cksum << 8);
    }
    return cksum;
}

bool PacketInterface::sendPacket(const unsigned char *data, int len)
{
    unsigned char buffer[len + 5];
    buffer[0] = 2;
    buffer[1] = len;

    memcpy(buffer + 2, data, len);

    unsigned short crc = crc16(data, len);
    buffer[len + 2] = crc >> 8;
    buffer[len + 3] = crc;
    buffer[len + 4] = 3;

    QByteArray sendData = QByteArray::fromRawData((char*)buffer, len + 5);

    emit dataToSend(sendData);

    return true;
}

bool PacketInterface::sendPacket(QByteArray data)
{
    return sendPacket((const unsigned char*)data.data(), data.size());
}

void PacketInterface::processPacket(const unsigned char *data, int len)
{
    int32_t ind = 0;
    MC_VALUES values;
    QByteArray bytes;
    QByteArray tmpArray;
    QVector<double> samples;
    mc_configuration mcconf;

    unsigned char id = data[0];
    data++;
    len--;

    switch (id) {
    case COMM_GET_VALUES:
        ind = 0;
        values.temp_mos1 = ((double)utility::buffer_get_int16(data, &ind)) / 10.0;
        values.temp_mos2 = ((double)utility::buffer_get_int16(data, &ind)) / 10.0;
        values.temp_mos3 = ((double)utility::buffer_get_int16(data, &ind)) / 10.0;
        values.temp_mos4 = ((double)utility::buffer_get_int16(data, &ind)) / 10.0;
        values.temp_mos5 = ((double)utility::buffer_get_int16(data, &ind)) / 10.0;
        values.temp_mos6 = ((double)utility::buffer_get_int16(data, &ind)) / 10.0;
        values.temp_pcb = ((double)utility::buffer_get_int16(data, &ind)) / 10.0;
        values.current_motor = ((double)utility::buffer_get_int32(data, &ind)) / 100.0;
        values.current_in = ((double)utility::buffer_get_int32(data, &ind)) / 100.0;
        values.duty_now = ((double)utility::buffer_get_int16(data, &ind)) / 1000.0;
        values.rpm = (double)utility::buffer_get_int32(data, &ind);
        values.v_in = ((double)utility::buffer_get_int16(data, &ind)) / 10.0;

        emit valuesReceived(values);
        break;

    case COMM_PRINT:
        tmpArray = QByteArray::fromRawData((char*)data, len);
        tmpArray[len] = '\0';
        emit printReceived(QString::fromLatin1(tmpArray));
        break;

    case COMM_SAMPLE_PRINT:
        for (int i = 0;i < len;i++) {
            bytes.append(data[i]);
        }
        emit samplesReceived(bytes);
        break;

    case COMM_ROTOR_POSITION:
        ind = 0;
        emit rotorPosReceived((double)utility::buffer_get_int32(data, &ind) / 100000.0);
        break;

    case COMM_EXPERIMENT_SAMPLE:
        samples.clear();
        ind = 0;

        while (ind < len) {
            samples.append(((double)utility::buffer_get_int32(data, &ind)) / 10000.0);
        }

        emit experimentSamplesReceived(samples);
        break;

    case COMM_GET_MCCONF:
        ind = 0;
        mcconf.pwm_mode = (mc_pwm_mode)data[ind++];
        mcconf.comm_mode = (mc_comm_mode)data[ind++];

        mcconf.l_current_max = (float)utility::buffer_get_int32(data, &ind) / 1000.0;
        mcconf.l_current_min = (float)utility::buffer_get_int32(data, &ind) / 1000.0;
        mcconf.l_in_current_max = (float)utility::buffer_get_int32(data, &ind) / 1000.0;
        mcconf.l_in_current_min = (float)utility::buffer_get_int32(data, &ind) / 1000.0;
        mcconf.l_abs_current_max = (float)utility::buffer_get_int32(data, &ind) / 1000.0;
        mcconf.l_min_erpm = (float)utility::buffer_get_int32(data, &ind) / 1000.0;
        mcconf.l_max_erpm = (float)utility::buffer_get_int32(data, &ind) / 1000.0;
        mcconf.l_max_erpm_fbrake = (float)utility::buffer_get_int32(data, &ind) / 1000.0;
        mcconf.l_min_vin = (float)utility::buffer_get_int32(data, &ind) / 1000.0;
        mcconf.l_max_vin = (float)utility::buffer_get_int32(data, &ind) / 1000.0;
        mcconf.l_slow_abs_current = data[ind++];
        mcconf.l_rpm_lim_neg_torque = data[ind++];

        mcconf.sl_is_sensorless = data[ind++];
        mcconf.sl_min_erpm = (float)utility::buffer_get_int32(data, &ind) / 1000.0;
        mcconf.sl_min_erpm_cycle_int_limit = (float)utility::buffer_get_int32(data, &ind) / 1000.0;
        mcconf.sl_cycle_int_limit = (float)utility::buffer_get_int32(data, &ind) / 1000.0;
        mcconf.sl_cycle_int_limit_high_fac = (float)utility::buffer_get_int32(data, &ind) / 1000.0;
        mcconf.sl_cycle_int_rpm_br = (float)utility::buffer_get_int32(data, &ind) / 1000.0;
        mcconf.sl_bemf_coupling_k = (float)utility::buffer_get_int32(data, &ind) / 1000.0;

        mcconf.hall_dir = data[ind++];
        mcconf.hall_fwd_add = data[ind++];
        mcconf.hall_rev_add = data[ind++];

        mcconf.s_pid_kp = (float)utility::buffer_get_int32(data, &ind) / 1000000.0;
        mcconf.s_pid_ki = (float)utility::buffer_get_int32(data, &ind) / 1000000.0;
        mcconf.s_pid_kd = (float)utility::buffer_get_int32(data, &ind) / 1000000.0;
        mcconf.s_pid_min_rpm = (float)utility::buffer_get_int32(data, &ind) / 1000.0;

        mcconf.cc_startup_boost_duty = (float)utility::buffer_get_int32(data, &ind) / 1000000.0;
        mcconf.cc_min_current = (float)utility::buffer_get_int32(data, &ind) / 1000.0;
        mcconf.cc_gain = (float)utility::buffer_get_int32(data, &ind) / 1000000.0;

        mcconf.meta_description = "Configuration loaded from the motor controller.";

        emit mcconfReceived(mcconf);
        break;

    default:
        break;
    }
}

bool PacketInterface::getValues()
{
    QByteArray buffer;
    buffer.clear();
    buffer.append((char)COMM_GET_VALUES);
    return sendPacket(buffer);
}

bool PacketInterface::sendTerminalCmd(QString cmd)
{
    QByteArray buffer;
    buffer.clear();
    buffer.append((char)COMM_TERMINAL_CMD);
    buffer.append(cmd.toLatin1());
    return sendPacket(buffer);
}

bool PacketInterface::setDutyCycle(double dutyCycle)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_SET_DUTY;
    utility::buffer_append_int32(mSendBuffer, (int32_t)(dutyCycle * 100000.0), &send_index);
    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::setCurrent(double current)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_SET_CURRENT;
    utility::buffer_append_int32(mSendBuffer, (int32_t)(current * 1000.0), &send_index);
    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::setCurrentBrake(double current)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_SET_CURRENT_BRAKE;
    utility::buffer_append_int32(mSendBuffer, (int32_t)(current * 1000.0), &send_index);
    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::setRpm(int rpm)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_SET_RPM;
    utility::buffer_append_int32(mSendBuffer, rpm, &send_index);
    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::setDetect()
{
    QByteArray buffer;
    buffer.clear();
    buffer.append((char)COMM_SET_DETECT);
    return sendPacket(buffer);
}

bool PacketInterface::samplePrint(bool at_start, int sample_len, int dec)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_SAMPLE_PRINT;
    mSendBuffer[send_index++] = at_start;
    utility::buffer_append_uint16(mSendBuffer, sample_len, &send_index);
    mSendBuffer[send_index++] = dec;
    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::getMcconf()
{
    QByteArray buffer;
    buffer.clear();
    buffer.append((char)COMM_GET_MCCONF);
    return sendPacket(buffer);
}

bool PacketInterface::setMcconf(const PacketInterface::mc_configuration &mcconf)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_SET_MCCONF;

    mSendBuffer[send_index++] = mcconf.pwm_mode;
    mSendBuffer[send_index++] = mcconf.comm_mode;

    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.l_current_max * 1000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.l_current_min * 1000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.l_in_current_max * 1000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.l_in_current_min * 1000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.l_abs_current_max * 1000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.l_min_erpm * 1000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.l_max_erpm * 1000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.l_max_erpm_fbrake * 1000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.l_min_vin * 1000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.l_max_vin * 1000.0), &send_index);
    mSendBuffer[send_index++] = mcconf.l_slow_abs_current;
    mSendBuffer[send_index++] = mcconf.l_rpm_lim_neg_torque;

    mSendBuffer[send_index++] = mcconf.sl_is_sensorless;
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.sl_min_erpm * 1000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.sl_min_erpm_cycle_int_limit * 1000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.sl_cycle_int_limit * 1000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.sl_cycle_int_limit_high_fac * 1000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.sl_cycle_int_rpm_br * 1000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.sl_bemf_coupling_k * 1000.0), &send_index);

    mSendBuffer[send_index++] = mcconf.hall_dir;
    mSendBuffer[send_index++] = mcconf.hall_fwd_add;
    mSendBuffer[send_index++] = mcconf.hall_rev_add;

    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.s_pid_kp * 1000000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.s_pid_ki * 1000000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.s_pid_kd * 1000000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.s_pid_min_rpm * 1000.0), &send_index);

    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.cc_startup_boost_duty * 1000000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.cc_min_current * 1000.0), &send_index);
    utility::buffer_append_int32(mSendBuffer, (int32_t)(mcconf.cc_gain * 1000000.0), &send_index);

    return sendPacket(mSendBuffer, send_index);
}
