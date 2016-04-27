/*
    Copyright 2012-2016 Benjamin Vedder	benjamin@vedder.se

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
    mSendBuffer = new quint8[mMaxBufferLen + 20];

    mRxState = 0;
    mRxTimer = 0;

    mSendCan = false;
    mCanId = 0;
    mIsLimitedMode = false;

    // Packet state
    mPayloadLength = 0;
    mRxDataPtr = 0;
    mCrcLow = 0;
    mCrcHigh = 0;

    // Firmware state
    mFirmwareIsUploading = false;
    mFirmwareState = 0;
    mFimwarePtr = 0;
    mFirmwareTimer = 0;
    mFirmwareRetries = 0;
    mFirmwareUploadStatus = "FW Upload Status";

    mTimer = new QTimer(this);
    mTimer->setInterval(10);
    mTimer->start();

    mHostAddress = QHostAddress("0.0.0.0");
    mUdpPort = 0;
    mUdpSocket = new QUdpSocket(this);

    connect(mUdpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));
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
                mRxState += 2;
                mRxTimer = rx_timeout;
                mRxDataPtr = 0;
                mPayloadLength = 0;
            } else if (rx_data == 3) {
                mRxState++;
                mRxTimer = rx_timeout;
                mRxDataPtr = 0;
                mPayloadLength = 0;
            } else {
                mRxState = 0;
            }
            break;

        case 1:
            mPayloadLength = (unsigned int)rx_data << 8;
            mRxState++;
            mRxTimer = rx_timeout;
            break;

        case 2:
            mPayloadLength |= (unsigned int)rx_data;
            if (mPayloadLength <= mMaxBufferLen && mPayloadLength > 0) {
                mRxState++;
                mRxTimer = rx_timeout;
            } else {
                mRxState = 0;
            }
            break;

        case 3:
            mRxBuffer[mRxDataPtr++] = rx_data;
            if (mRxDataPtr == mPayloadLength) {
                mRxState++;
            }
            mRxTimer = rx_timeout;
            break;

        case 4:
            mCrcHigh = rx_data;
            mRxState++;
            mRxTimer = rx_timeout;
            break;

        case 5:
            mCrcLow = rx_data;
            mRxState++;
            mRxTimer = rx_timeout;
            break;

        case 6:
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

void PacketInterface::setLimitedMode(bool is_limited)
{
    mIsLimitedMode = is_limited;
}

bool PacketInterface::isLimitedMode()
{
    return mIsLimitedMode;
}

void PacketInterface::timerSlot()
{
    if (mRxTimer) {
        mRxTimer--;
    } else {
        mRxState = 0;
    }

    if (mFirmwareIsUploading) {
        if (mFirmwareTimer) {
            mFirmwareTimer--;
        } else {
            firmwareUploadUpdate(true);
        }
    }
}

void PacketInterface::readPendingDatagrams()
{
    while (mUdpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(mUdpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        mUdpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        processPacket((unsigned char*)datagram.data(), datagram.length());
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

bool PacketInterface::sendPacket(const unsigned char *data, unsigned int len_packet)
{    
    // Only allow firmware commands in limited mode
    if (mIsLimitedMode && data[0] > COMM_WRITE_NEW_APP_DATA) {
        return false;
    }

    static unsigned char buffer[mMaxBufferLen];
    unsigned int ind = 0;

    // If the IP is valid, send the packet over UDP
    if (QString::compare(mHostAddress.toString(), "0.0.0.0") != 0) {
        if (mSendCan) {
            buffer[ind++] = COMM_FORWARD_CAN;
            buffer[ind++] = mCanId;
        }

        memcpy(buffer + ind, data, len_packet);
        ind += len_packet;

        mUdpSocket->writeDatagram(QByteArray::fromRawData((const char*)buffer, ind), mHostAddress, mUdpPort);
        return true;
    }

    int len_tot = len_packet;

    if (mSendCan) {
        len_tot += 2;
    }

    unsigned int data_offs = 0;

    if (len_tot <= 256) {
        buffer[ind++] = 2;
        buffer[ind++] = len_tot;
        data_offs = 2;
    } else {
        buffer[ind++] = 3;
        buffer[ind++] = len_tot >> 8;
        buffer[ind++] = len_tot & 0xFF;
        data_offs = 3;
    }

    if (mSendCan) {
        buffer[ind++] = COMM_FORWARD_CAN;
        buffer[ind++] = mCanId;
    }

    memcpy(buffer + ind, data, len_packet);
    ind += len_packet;

    unsigned short crc = crc16(buffer + data_offs, len_tot);
    buffer[ind++] = crc >> 8;
    buffer[ind++] = crc;
    buffer[ind++] = 3;

    QByteArray sendData = QByteArray::fromRawData((char*)buffer, ind);

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
    app_configuration appconf;
    double detect_cycle_int_limit;
    double detect_coupling_k;
    QVector<int> detect_hall_table;
    int detect_hall_res;
    double dec_ppm;
    double ppm_last_len;
    double dec_adc;
    double dec_adc_voltage;
    double dec_adc2;
    double dec_adc_voltage2;
    int fw_major;
    int fw_minor;

    unsigned char id = data[0];
    data++;
    len--;

    switch (id) {
    case COMM_FW_VERSION:
        if (len == 2) {
            ind = 0;
            fw_major = data[ind++];
            fw_minor = data[ind++];
        } else {
            fw_major = -1;
            fw_minor = -1;
        }
        emit fwVersionReceived(fw_major, fw_minor);
        break;

    case COMM_ERASE_NEW_APP:
    case COMM_WRITE_NEW_APP_DATA:
        firmwareUploadUpdate(!data[0]);
        break;

    case COMM_GET_VALUES:
        ind = 0;
        values.temp_mos1 = utility::buffer_get_double16(data, 10.0, &ind);
        values.temp_mos2 = utility::buffer_get_double16(data, 10.0, &ind);
        values.temp_mos3 = utility::buffer_get_double16(data, 10.0, &ind);
        values.temp_mos4 = utility::buffer_get_double16(data, 10.0, &ind);
        values.temp_mos5 = utility::buffer_get_double16(data, 10.0, &ind);
        values.temp_mos6 = utility::buffer_get_double16(data, 10.0, &ind);
        values.temp_pcb = utility::buffer_get_double16(data, 10.0, &ind);
        values.current_motor = utility::buffer_get_double32(data, 100.0, &ind);
        values.current_in = utility::buffer_get_double32(data, 100.0, &ind);
        values.duty_now = utility::buffer_get_double16(data, 1000.0, &ind);
        values.rpm = utility::buffer_get_double32(data, 1.0, &ind);
        values.v_in = utility::buffer_get_double16(data, 10.0, &ind);
        values.amp_hours = utility::buffer_get_double32(data, 10000.0, &ind);
        values.amp_hours_charged = utility::buffer_get_double32(data, 10000.0, &ind);
        values.watt_hours = utility::buffer_get_double32(data, 10000.0, &ind);
        values.watt_hours_charged = utility::buffer_get_double32(data, 10000.0, &ind);
        values.tachometer = utility::buffer_get_int32(data, &ind);
        values.tachometer_abs = utility::buffer_get_int32(data, &ind);
        values.fault_code = (mc_fault_code)data[ind++];
        values.fault_str = faultToStr(values.fault_code);

        emit valuesReceived(values);
        break;

    case COMM_PRINT:
        tmpArray = QByteArray::fromRawData((char*)data, len);
        tmpArray[len] = '\0';
        emit printReceived(QString::fromLatin1(tmpArray));
        break;

    case COMM_SAMPLE_PRINT:
        bytes.clear();
        for (int i = 0;i < len;i++) {
            bytes.append(data[i]);
        }
        emit samplesReceived(bytes);
        break;

    case COMM_ROTOR_POSITION:
        ind = 0;
        emit rotorPosReceived(utility::buffer_get_double32(data, 100000.0, &ind));
        break;

    case COMM_EXPERIMENT_SAMPLE:
        samples.clear();
        ind = 0;

        while (ind < len) {
            samples.append(utility::buffer_get_double32(data, 10000.0, &ind));
        }

        emit experimentSamplesReceived(samples);
        break;

    case COMM_GET_MCCONF:
    case COMM_GET_MCCONF_DEFAULT:
        ind = 0;
        mcconf.pwm_mode = (mc_pwm_mode)data[ind++];
        mcconf.comm_mode = (mc_comm_mode)data[ind++];
        mcconf.motor_type = (mc_motor_type)data[ind++];
        mcconf.sensor_mode = (mc_sensor_mode)data[ind++];

        mcconf.l_current_max = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_current_min = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_in_current_max = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_in_current_min = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_abs_current_max = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_min_erpm = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_max_erpm = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_max_erpm_fbrake = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_max_erpm_fbrake_cc = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_min_vin = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_max_vin = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_battery_cut_start = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_battery_cut_end = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_slow_abs_current = data[ind++];
        mcconf.l_rpm_lim_neg_torque = data[ind++];
        mcconf.l_temp_fet_start = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_temp_fet_end = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_temp_motor_start = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_temp_motor_end = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.l_min_duty = utility::buffer_get_double32(data, 1000000.0, &ind);
        mcconf.l_max_duty = utility::buffer_get_double32(data, 1000000.0, &ind);

        mcconf.sl_min_erpm = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.sl_min_erpm_cycle_int_limit = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.sl_max_fullbreak_current_dir_change = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.sl_cycle_int_limit = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.sl_phase_advance_at_br = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.sl_cycle_int_rpm_br = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.sl_bemf_coupling_k = utility::buffer_get_double32(data, 1000.0, &ind);

        memcpy(mcconf.hall_table, data + ind, 8);
        ind += 8;
        mcconf.hall_sl_erpm = utility::buffer_get_double32(data, 1000.0, &ind);

        mcconf.foc_current_kp = utility::buffer_get_double32(data, 1e5, &ind);
        mcconf.foc_current_ki = utility::buffer_get_double32(data, 1e5, &ind);
        mcconf.foc_f_sw = utility::buffer_get_double32(data, 1e3, &ind);
        mcconf.foc_dt_us = utility::buffer_get_double32(data, 1e6, &ind);
        mcconf.foc_encoder_inverted = data[ind++];
        mcconf.foc_encoder_offset = utility::buffer_get_double32(data, 1e3, &ind);
        mcconf.foc_encoder_ratio = utility::buffer_get_double32(data, 1e3, &ind);
        mcconf.foc_sensor_mode = (mc_foc_sensor_mode)data[ind++];
        mcconf.foc_pll_kp = utility::buffer_get_double32(data, 1e3, &ind);
        mcconf.foc_pll_ki = utility::buffer_get_double32(data, 1e3, &ind);
        mcconf.foc_motor_l = utility::buffer_get_double32(data, 1e8, &ind);
        mcconf.foc_motor_r = utility::buffer_get_double32(data, 1e5, &ind);
        mcconf.foc_motor_flux_linkage = utility::buffer_get_double32(data, 1e5, &ind);
        mcconf.foc_observer_gain = utility::buffer_get_double32(data, 1e0, &ind);
        mcconf.foc_duty_dowmramp_kp = utility::buffer_get_double32(data, 1e3, &ind);
        mcconf.foc_duty_dowmramp_ki = utility::buffer_get_double32(data, 1e3, &ind);
        mcconf.foc_openloop_rpm = utility::buffer_get_double32(data, 1e3, &ind);
        mcconf.foc_sl_openloop_hyst = utility::buffer_get_double32(data, 1e3, &ind);
        mcconf.foc_sl_openloop_time = utility::buffer_get_double32(data, 1e3, &ind);
        mcconf.foc_sl_d_current_duty = utility::buffer_get_double32(data, 1e3, &ind);
        mcconf.foc_sl_d_current_factor = utility::buffer_get_double32(data, 1e3, &ind);
        memcpy(mcconf.foc_hall_table, data + ind, 8);
        ind += 8;
        mcconf.foc_sl_erpm = utility::buffer_get_double32(data, 1000.0, &ind);

        mcconf.s_pid_kp = utility::buffer_get_double32(data, 1000000.0, &ind);
        mcconf.s_pid_ki = utility::buffer_get_double32(data, 1000000.0, &ind);
        mcconf.s_pid_kd = utility::buffer_get_double32(data, 1000000.0, &ind);
        mcconf.s_pid_min_erpm = utility::buffer_get_double32(data, 1000.0, &ind);

        mcconf.p_pid_kp = utility::buffer_get_double32(data, 1000000.0, &ind);
        mcconf.p_pid_ki = utility::buffer_get_double32(data, 1000000.0, &ind);
        mcconf.p_pid_kd = utility::buffer_get_double32(data, 1000000.0, &ind);
        mcconf.p_pid_ang_div = utility::buffer_get_double32(data, 1e5, &ind);

        mcconf.cc_startup_boost_duty = utility::buffer_get_double32(data, 1000000.0, &ind);
        mcconf.cc_min_current = utility::buffer_get_double32(data, 1000.0, &ind);
        mcconf.cc_gain = utility::buffer_get_double32(data, 1000000.0, &ind);
        mcconf.cc_ramp_step_max = utility::buffer_get_double32(data, 1000000.0, &ind);

        mcconf.m_fault_stop_time_ms = utility::buffer_get_int32(data, &ind);
        mcconf.m_duty_ramp_step = utility::buffer_get_double32(data, 1000000.0, &ind);
        mcconf.m_duty_ramp_step_rpm_lim = utility::buffer_get_double32(data, 1000000.0, &ind);
        mcconf.m_current_backoff_gain = utility::buffer_get_double32(data, 1000000.0, &ind);
        mcconf.m_encoder_counts = utility::buffer_get_uint32(data, &ind);
        mcconf.m_sensor_port_mode = (sensor_port_mode)data[ind++];

        mcconf.meta_description = "Configuration loaded from the motor controller.";

        emit mcconfReceived(mcconf);
        break;

    case COMM_GET_APPCONF:
    case COMM_GET_APPCONF_DEFAULT:
        ind = 0;
        appconf.controller_id = data[ind++];
        appconf.timeout_msec = utility::buffer_get_uint32(data, &ind);
        appconf.timeout_brake_current = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.send_can_status = data[ind++];
        appconf.send_can_status_rate_hz = utility::buffer_get_uint16(data, &ind);

        appconf.app_to_use = (app_use)data[ind++];

        appconf.app_ppm_conf.ctrl_type = (ppm_control_type)data[ind++];
        appconf.app_ppm_conf.pid_max_erpm = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_ppm_conf.hyst = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_ppm_conf.pulse_start = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_ppm_conf.pulse_end = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_ppm_conf.median_filter = data[ind++];
        appconf.app_ppm_conf.safe_start = data[ind++];
        appconf.app_ppm_conf.rpm_lim_start = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_ppm_conf.rpm_lim_end = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_ppm_conf.multi_esc = data[ind++];
        appconf.app_ppm_conf.tc = data[ind++];
        appconf.app_ppm_conf.tc_max_diff = utility::buffer_get_double32(data, 1000.0, &ind);

        appconf.app_adc_conf.ctrl_type = (adc_control_type)data[ind++];
        appconf.app_adc_conf.hyst = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_adc_conf.voltage_start = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_adc_conf.voltage_end = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_adc_conf.use_filter = data[ind++];
        appconf.app_adc_conf.safe_start = data[ind++];
        appconf.app_adc_conf.cc_button_inverted = data[ind++];
        appconf.app_adc_conf.rev_button_inverted = data[ind++];
        appconf.app_adc_conf.voltage_inverted = data[ind++];
        appconf.app_adc_conf.rpm_lim_start = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_adc_conf.rpm_lim_end = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_adc_conf.multi_esc = data[ind++];
        appconf.app_adc_conf.tc = data[ind++];
        appconf.app_adc_conf.tc_max_diff = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_adc_conf.update_rate_hz = utility::buffer_get_uint16(data, &ind);

        appconf.app_uart_baudrate = utility::buffer_get_uint32(data, &ind);

        appconf.app_chuk_conf.ctrl_type = (chuk_control_type)data[ind++];
        appconf.app_chuk_conf.hyst = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_chuk_conf.rpm_lim_start = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_chuk_conf.rpm_lim_end = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_chuk_conf.ramp_time_pos = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_chuk_conf.ramp_time_neg = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_chuk_conf.stick_erpm_per_s_in_cc = utility::buffer_get_double32(data, 1000.0, &ind);
        appconf.app_chuk_conf.multi_esc = data[ind++];
        appconf.app_chuk_conf.tc = data[ind++];
        appconf.app_chuk_conf.tc_max_diff = utility::buffer_get_double32(data, 1000.0, &ind);

        appconf.app_nrf_conf.speed = (NRF_SPEED)data[ind++];
        appconf.app_nrf_conf.power = (NRF_POWER)data[ind++];
        appconf.app_nrf_conf.crc_type = (NRF_CRC)data[ind++];
        appconf.app_nrf_conf.retry_delay = (NRF_RETR_DELAY)data[ind++];
        appconf.app_nrf_conf.retries = data[ind++];
        appconf.app_nrf_conf.channel = data[ind++];
        memcpy(appconf.app_nrf_conf.address, data + ind, 3);
        ind += 3;
        appconf.app_nrf_conf.send_crc_ack = data[ind++];

        emit appconfReceived(appconf);
        break;

    case COMM_DETECT_MOTOR_PARAM:
        ind = 0;
        detect_cycle_int_limit = utility::buffer_get_double32(data, 1000.0, &ind);
        detect_coupling_k = utility::buffer_get_double32(data, 1000.0, &ind);
        for (int i = 0;i < 8;i++) {
            detect_hall_table.append((const signed char)(data[ind++]));
        }
        detect_hall_res = (const signed char)(data[ind++]);
        emit motorParamReceived(detect_cycle_int_limit, detect_coupling_k, detect_hall_table, detect_hall_res);
        break;

    case COMM_DETECT_MOTOR_R_L: {
        ind = 0;
        double r = utility::buffer_get_double32(data, 1e6, &ind);
        double l = utility::buffer_get_double32(data, 1e3, &ind);
        emit motorRLReceived(r, l);

    }
        break;

    case COMM_DETECT_MOTOR_FLUX_LINKAGE: {
        ind = 0;
        double linkage = utility::buffer_get_double32(data, 1e7, &ind);
        emit motorLinkageReceived(linkage);
    }
        break;

    case COMM_DETECT_ENCODER: {
        ind = 0;
        double offset = utility::buffer_get_double32(data, 1e6, &ind);
        double ratio = utility::buffer_get_double32(data, 1e6, &ind);
        bool inverted = data[ind++];
        emit encoderParamReceived(offset, ratio, inverted);
    }
        break;

    case COMM_DETECT_HALL_FOC: {
        ind = 0;
        for (int i = 0;i < 8;i++) {
            detect_hall_table.append((const unsigned char)(data[ind++]));
        }
        int res = (const unsigned char)(data[ind++]);
        emit focHallTableReceived(detect_hall_table, res);
    }
        break;

    case COMM_GET_DECODED_PPM:
        ind = 0;
        dec_ppm = utility::buffer_get_double32(data, 1000000.0, &ind);
        ppm_last_len = utility::buffer_get_double32(data, 1000000.0, &ind);
        emit decodedPpmReceived(dec_ppm, ppm_last_len);
        break;

    case COMM_GET_DECODED_ADC:
        ind = 0;
        dec_adc = utility::buffer_get_double32(data, 1000000.0, &ind);
        dec_adc_voltage = utility::buffer_get_double32(data, 1000000.0, &ind);
        dec_adc2 = utility::buffer_get_double32(data, 1000000.0, &ind);
        dec_adc_voltage2 = utility::buffer_get_double32(data, 1000000.0, &ind);
        emit decodedAdcReceived(dec_adc, dec_adc_voltage, dec_adc2, dec_adc_voltage2);
        break;

    case COMM_GET_DECODED_CHUK:
        ind = 0;
        dec_ppm = utility::buffer_get_double32(data, 1000000.0, &ind);
        emit decodedChukReceived(dec_ppm);
        break;

    case COMM_SET_MCCONF:
        emit ackReceived("MCCONF Write OK");
        break;

    case COMM_SET_APPCONF:
        emit ackReceived("APPCONF Write OK");
        break;

    case COMM_CUSTOM_APP_DATA:
        bytes.clear();
        for (int i = 0;i < len;i++) {
            bytes.append(data[i]);
        }
        emit customAppDataReceived(bytes);
        break;

    default:
        break;
    }
}

QString PacketInterface::faultToStr(mc_fault_code fault)
{
    switch (fault) {
    case FAULT_CODE_NONE: return "FAULT_CODE_NONE";
    case FAULT_CODE_OVER_VOLTAGE: return "FAULT_CODE_OVER_VOLTAGE";
    case FAULT_CODE_UNDER_VOLTAGE: return "FAULT_CODE_UNDER_VOLTAGE";
    case FAULT_CODE_DRV8302: return "FAULT_CODE_DRV8302";
    case FAULT_CODE_ABS_OVER_CURRENT: return "FAULT_CODE_ABS_OVER_CURRENT";
    case FAULT_CODE_OVER_TEMP_FET: return "FAULT_CODE_OVER_TEMP_FET";
    case FAULT_CODE_OVER_TEMP_MOTOR: return "FAULT_CODE_OVER_TEMP_MOTOR";
    default: return "Unknown fault";
    }
}

void PacketInterface::firmwareUploadUpdate(bool isTimeout)
{
    if (!mFirmwareIsUploading) {
        return;
    }

    const int app_packet_size = 200;
    const int retries = 5;
    const int timeout = 350;

    if (mFirmwareState == 0) {
        mFirmwareUploadStatus = "Buffer Erase";
        if (isTimeout) {
            // Erase timed out, abort.
            mFirmwareIsUploading = false;
            mFimwarePtr = 0;
            mFirmwareUploadStatus = "Buffer Erase Timeout";
        } else {
            mFirmwareState++;
            mFirmwareRetries = retries;
            mFirmwareTimer = timeout;
            firmwareUploadUpdate(true);
        }
    } else if (mFirmwareState == 1) {
        mFirmwareUploadStatus = "CRC/Size Write";
        if (isTimeout) {
            if (mFirmwareRetries > 0) {
                mFirmwareRetries--;
                mFirmwareTimer = timeout;
            } else {
                mFirmwareIsUploading = false;
                mFimwarePtr = 0;
                mFirmwareState = 0;
                mFirmwareUploadStatus = "CRC/Size Write Timeout";
                return;
            }

            quint16 crc = crc16((const unsigned char*)mNewFirmware.constData(), mNewFirmware.size());

            qint32 send_index = 0;
            mSendBuffer[send_index++] = COMM_WRITE_NEW_APP_DATA;
            utility::buffer_append_uint32(mSendBuffer, 0, &send_index);
            utility::buffer_append_uint32(mSendBuffer, mNewFirmware.size(), &send_index);
            utility::buffer_append_uint16(mSendBuffer, crc, &send_index);
            sendPacket(mSendBuffer, send_index);
        } else {
            mFirmwareState++;
            mFirmwareRetries = retries;
            mFirmwareTimer = timeout;
            firmwareUploadUpdate(true);
        }
    } else if (mFirmwareState == 2) {
        mFirmwareUploadStatus = "FW Data Write";
        if (isTimeout) {
            if (mFirmwareRetries > 0) {
                mFirmwareRetries--;
                mFirmwareTimer = timeout;
            } else {
                mFirmwareIsUploading = false;
                mFimwarePtr = 0;
                mFirmwareState = 0;
                mFirmwareUploadStatus = "FW Data Write Timeout";
                return;
            }

            int fw_size_left = mNewFirmware.size() - mFimwarePtr;
            int send_size = fw_size_left > app_packet_size ? app_packet_size : fw_size_left;

            qint32 send_index = 0;
            mSendBuffer[send_index++] = COMM_WRITE_NEW_APP_DATA;
            utility::buffer_append_uint32(mSendBuffer, mFimwarePtr + 6, &send_index);
            memcpy(mSendBuffer + send_index, mNewFirmware.constData() + mFimwarePtr, send_size);
            send_index += send_size;
            sendPacket(mSendBuffer, send_index);
        } else {
            mFirmwareRetries = retries;
            mFirmwareTimer = timeout;
            mFimwarePtr += app_packet_size;

            if (mFimwarePtr >= mNewFirmware.size()) {
                mFirmwareIsUploading = false;
                mFimwarePtr = 0;
                mFirmwareState = 0;
                mFirmwareUploadStatus = "FW Upload Done";
                // Upload done. Enter bootloader!
                QByteArray buffer;
                buffer.append((char)COMM_JUMP_TO_BOOTLOADER);
                sendPacket(buffer);
            } else {
                firmwareUploadUpdate(true);
            }
        }
    }
}

bool PacketInterface::getFwVersion()
{
    QByteArray buffer;
    buffer.clear();
    buffer.append((char)COMM_FW_VERSION);
    return sendPacket(buffer);
}

bool PacketInterface::startFirmwareUpload(QByteArray &newFirmware)
{
    mFirmwareIsUploading = true;
    mFirmwareState = 0;
    mFimwarePtr = 0;
    mFirmwareTimer = 500;
    mNewFirmware.clear();
    mNewFirmware.append(newFirmware);
    mFirmwareUploadStatus = "Buffer Erase";

    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_ERASE_NEW_APP;
    utility::buffer_append_uint32(mSendBuffer, mNewFirmware.size(), &send_index);
    return sendPacket(mSendBuffer, send_index);
}

double PacketInterface::getFirmwareUploadProgress()
{
    if (mFirmwareIsUploading) {
        return (double)mFimwarePtr / (double)mNewFirmware.size();
    } else {
        return -1.0;
    }
}

QString PacketInterface::getFirmwareUploadStatus()
{
    return mFirmwareUploadStatus;
}

void PacketInterface::cancelFirmwareUpload()
{
    if (mFirmwareIsUploading) {
        mFirmwareIsUploading = false;
        mFimwarePtr = 0;
        mFirmwareState = 0;
        mFirmwareUploadStatus = "Cancelled";
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
    utility::buffer_append_double32(mSendBuffer, dutyCycle, 100000.0, &send_index);
    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::setCurrent(double current)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_SET_CURRENT;
    utility::buffer_append_double32(mSendBuffer, current, 1000.0, &send_index);
    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::setCurrentBrake(double current)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_SET_CURRENT_BRAKE;
    utility::buffer_append_double32(mSendBuffer, current, 1000.0, &send_index);
    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::setRpm(int rpm)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_SET_RPM;
    utility::buffer_append_int32(mSendBuffer, rpm, &send_index);
    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::setPos(double pos)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_SET_POS;
    utility::buffer_append_double32(mSendBuffer, pos, 1000000.0, &send_index);
    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::setDetect(disp_pos_mode mode)
{
    QByteArray buffer;
    buffer.clear();
    buffer.append((char)COMM_SET_DETECT);
    buffer.append((char)mode);
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

bool PacketInterface::getMcconfDefault()
{
    QByteArray buffer;
    buffer.clear();
    buffer.append((char)COMM_GET_MCCONF_DEFAULT);
    return sendPacket(buffer);
}

bool PacketInterface::setMcconf(const mc_configuration &mcconf)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_SET_MCCONF;

    mSendBuffer[send_index++] = mcconf.pwm_mode;
    mSendBuffer[send_index++] = mcconf.comm_mode;
    mSendBuffer[send_index++] = mcconf.motor_type;
    mSendBuffer[send_index++] = mcconf.sensor_mode;

    utility::buffer_append_double32(mSendBuffer, mcconf.l_current_max, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_current_min, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_in_current_max, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_in_current_min, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_abs_current_max, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_min_erpm, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_max_erpm, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_max_erpm_fbrake, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_max_erpm_fbrake_cc, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_min_vin, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_max_vin, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_battery_cut_start, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_battery_cut_end, 1000, &send_index);
    mSendBuffer[send_index++] = mcconf.l_slow_abs_current;
    mSendBuffer[send_index++] = mcconf.l_rpm_lim_neg_torque;
    utility::buffer_append_double32(mSendBuffer,mcconf.l_temp_fet_start, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_temp_fet_end, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_temp_motor_start, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_temp_motor_end, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_min_duty, 1000000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.l_max_duty, 1000000, &send_index);

    utility::buffer_append_double32(mSendBuffer,mcconf.sl_min_erpm, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.sl_min_erpm_cycle_int_limit, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.sl_max_fullbreak_current_dir_change, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.sl_cycle_int_limit, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.sl_phase_advance_at_br, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.sl_cycle_int_rpm_br, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.sl_bemf_coupling_k, 1000, &send_index);

    memcpy(mSendBuffer + send_index, mcconf.hall_table, 8);
    send_index += 8;
    utility::buffer_append_double32(mSendBuffer,mcconf.hall_sl_erpm, 1000, &send_index);

    utility::buffer_append_double32(mSendBuffer, mcconf.foc_current_kp, 1e5, &send_index);
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_current_ki, 1e5, &send_index);
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_f_sw, 1e3, &send_index);
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_dt_us, 1e6, &send_index);
    mSendBuffer[send_index++] = mcconf.foc_encoder_inverted;
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_encoder_offset, 1e3, &send_index);
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_encoder_ratio, 1e3, &send_index);
    mSendBuffer[send_index++] = mcconf.foc_sensor_mode;
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_pll_kp, 1e3, &send_index);
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_pll_ki, 1e3, &send_index);
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_motor_l, 1e8, &send_index);
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_motor_r, 1e5, &send_index);
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_motor_flux_linkage, 1e5, &send_index);
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_observer_gain, 1e0, &send_index);
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_duty_dowmramp_kp, 1e3, &send_index);
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_duty_dowmramp_ki, 1e3, &send_index);
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_openloop_rpm, 1e3, &send_index);
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_sl_openloop_hyst, 1e3, &send_index);
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_sl_openloop_time, 1e3, &send_index);
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_sl_d_current_duty, 1e3, &send_index);
    utility::buffer_append_double32(mSendBuffer, mcconf.foc_sl_d_current_factor, 1e3, &send_index);
    memcpy(mSendBuffer + send_index, mcconf.foc_hall_table, 8);
    send_index += 8;
    utility::buffer_append_double32(mSendBuffer,mcconf.foc_sl_erpm, 1000, &send_index);

    utility::buffer_append_double32(mSendBuffer,mcconf.s_pid_kp, 1000000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.s_pid_ki, 1000000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.s_pid_kd, 1000000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.s_pid_min_erpm, 1000, &send_index);

    utility::buffer_append_double32(mSendBuffer,mcconf.p_pid_kp, 1000000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.p_pid_ki, 1000000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.p_pid_kd, 1000000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.p_pid_ang_div, 1e5, &send_index);

    utility::buffer_append_double32(mSendBuffer,mcconf.cc_startup_boost_duty, 1000000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.cc_min_current, 1000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.cc_gain, 1000000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.cc_ramp_step_max, 1000000, &send_index);

    utility::buffer_append_int32(mSendBuffer, mcconf.m_fault_stop_time_ms, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.m_duty_ramp_step, 1000000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.m_duty_ramp_step_rpm_lim, 1000000, &send_index);
    utility::buffer_append_double32(mSendBuffer,mcconf.m_current_backoff_gain, 1000000, &send_index);
    utility::buffer_append_uint32(mSendBuffer, mcconf.m_encoder_counts, &send_index);
    mSendBuffer[send_index++] = mcconf.m_sensor_port_mode;

    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::detectMotorParam(double current, double min_rpm, double low_duty)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_DETECT_MOTOR_PARAM;
    utility::buffer_append_double32(mSendBuffer, current, 1000.0, &send_index);
    utility::buffer_append_double32(mSendBuffer, min_rpm, 1000.0, &send_index);
    utility::buffer_append_double32(mSendBuffer, low_duty, 1000.0, &send_index);
    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::getAppConf()
{
    QByteArray buffer;
    buffer.clear();
    buffer.append((char)COMM_GET_APPCONF);
    return sendPacket(buffer);
}

bool PacketInterface::getAppConfDefault()
{
    QByteArray buffer;
    buffer.clear();
    buffer.append((char)COMM_GET_APPCONF_DEFAULT);
    return sendPacket(buffer);
}

bool PacketInterface::setAppConf(const app_configuration &appconf)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_SET_APPCONF;
    mSendBuffer[send_index++] = appconf.controller_id;
    utility::buffer_append_uint32(mSendBuffer, appconf.timeout_msec, &send_index);
    utility::buffer_append_double32(mSendBuffer, appconf.timeout_brake_current, 1000.0, &send_index);
    mSendBuffer[send_index++] = appconf.send_can_status;
    utility::buffer_append_uint16(mSendBuffer, appconf.send_can_status_rate_hz, &send_index);

    mSendBuffer[send_index++] = appconf.app_to_use;

    mSendBuffer[send_index++] = appconf.app_ppm_conf.ctrl_type;
    utility::buffer_append_double32(mSendBuffer, appconf.app_ppm_conf.pid_max_erpm, 1000.0, &send_index);
    utility::buffer_append_double32(mSendBuffer, appconf.app_ppm_conf.hyst, 1000.0, &send_index);
    utility::buffer_append_double32(mSendBuffer, appconf.app_ppm_conf.pulse_start, 1000.0, &send_index);
    utility::buffer_append_double32(mSendBuffer, appconf.app_ppm_conf.pulse_end, 1000.0, &send_index);
    mSendBuffer[send_index++] = appconf.app_ppm_conf.median_filter;
    mSendBuffer[send_index++] = appconf.app_ppm_conf.safe_start;
    utility::buffer_append_double32(mSendBuffer, appconf.app_ppm_conf.rpm_lim_start, 1000.0, &send_index);
    utility::buffer_append_double32(mSendBuffer, appconf.app_ppm_conf.rpm_lim_end, 1000.0, &send_index);
    mSendBuffer[send_index++] = appconf.app_ppm_conf.multi_esc;
    mSendBuffer[send_index++] = appconf.app_ppm_conf.tc;
    utility::buffer_append_double32(mSendBuffer, appconf.app_ppm_conf.tc_max_diff, 1000.0, &send_index);

    mSendBuffer[send_index++] = appconf.app_adc_conf.ctrl_type;
    utility::buffer_append_double32(mSendBuffer, appconf.app_adc_conf.hyst, 1000.0, &send_index);
    utility::buffer_append_double32(mSendBuffer, appconf.app_adc_conf.voltage_start, 1000.0, &send_index);
    utility::buffer_append_double32(mSendBuffer, appconf.app_adc_conf.voltage_end, 1000.0, &send_index);
    mSendBuffer[send_index++] = appconf.app_adc_conf.use_filter;
    mSendBuffer[send_index++] = appconf.app_adc_conf.safe_start;
    mSendBuffer[send_index++] = appconf.app_adc_conf.cc_button_inverted;
    mSendBuffer[send_index++] = appconf.app_adc_conf.rev_button_inverted;
    mSendBuffer[send_index++] = appconf.app_adc_conf.voltage_inverted;
    utility::buffer_append_double32(mSendBuffer, appconf.app_adc_conf.rpm_lim_start, 1000.0, &send_index);
    utility::buffer_append_double32(mSendBuffer, appconf.app_adc_conf.rpm_lim_end, 1000.0, &send_index);
    mSendBuffer[send_index++] = appconf.app_adc_conf.multi_esc;
    mSendBuffer[send_index++] = appconf.app_adc_conf.tc;
    utility::buffer_append_double32(mSendBuffer, appconf.app_adc_conf.tc_max_diff, 1000.0, &send_index);
    utility::buffer_append_uint16(mSendBuffer, appconf.app_adc_conf.update_rate_hz, &send_index);

    utility::buffer_append_uint32(mSendBuffer, appconf.app_uart_baudrate, &send_index);

    mSendBuffer[send_index++] = appconf.app_chuk_conf.ctrl_type;
    utility::buffer_append_double32(mSendBuffer, appconf.app_chuk_conf.hyst, 1000.0, &send_index);
    utility::buffer_append_double32(mSendBuffer, appconf.app_chuk_conf.rpm_lim_start, 1000.0, &send_index);
    utility::buffer_append_double32(mSendBuffer, appconf.app_chuk_conf.rpm_lim_end, 1000.0, &send_index);
    utility::buffer_append_double32(mSendBuffer, appconf.app_chuk_conf.ramp_time_pos, 1000.0, &send_index);
    utility::buffer_append_double32(mSendBuffer, appconf.app_chuk_conf.ramp_time_neg, 1000.0, &send_index);
    utility::buffer_append_double32(mSendBuffer, appconf.app_chuk_conf.stick_erpm_per_s_in_cc, 1000.0, &send_index);
    mSendBuffer[send_index++] = appconf.app_chuk_conf.multi_esc;
    mSendBuffer[send_index++] = appconf.app_chuk_conf.tc;
    utility::buffer_append_double32(mSendBuffer, appconf.app_chuk_conf.tc_max_diff, 1000.0, &send_index);

    mSendBuffer[send_index++] = appconf.app_nrf_conf.speed;
    mSendBuffer[send_index++] = appconf.app_nrf_conf.power;
    mSendBuffer[send_index++] = appconf.app_nrf_conf.crc_type;
    mSendBuffer[send_index++] = appconf.app_nrf_conf.retry_delay;
    mSendBuffer[send_index++] = appconf.app_nrf_conf.retries;
    mSendBuffer[send_index++] = appconf.app_nrf_conf.channel;
    memcpy(mSendBuffer + send_index, appconf.app_nrf_conf.address, 3);
    send_index += 3;
    mSendBuffer[send_index++] = appconf.app_nrf_conf.send_crc_ack;

    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::reboot()
{
    QByteArray buffer;
    buffer.clear();
    buffer.append((char)COMM_REBOOT);
    return sendPacket(buffer);
}

bool PacketInterface::sendAlive()
{
    QByteArray buffer;
    buffer.clear();
    buffer.append((char)COMM_ALIVE);
    return sendPacket(buffer);
}

bool PacketInterface::getDecodedPpm()
{
    QByteArray buffer;
    buffer.clear();
    buffer.append((char)COMM_GET_DECODED_PPM);
    return sendPacket(buffer);
}

bool PacketInterface::getDecodedAdc()
{
    QByteArray buffer;
    buffer.clear();
    buffer.append((char)COMM_GET_DECODED_ADC);
    return sendPacket(buffer);
}

bool PacketInterface::getDecodedChuk()
{
    QByteArray buffer;
    buffer.clear();
    buffer.append((char)COMM_GET_DECODED_CHUK);
    return sendPacket(buffer);
}

bool PacketInterface::setServoPos(double pos)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_SET_SERVO_POS;
    utility::buffer_append_double16(mSendBuffer, pos, 1000.0, &send_index);
    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::measureRL()
{
    QByteArray buffer;
    buffer.clear();
    buffer.append((char)COMM_DETECT_MOTOR_R_L);
    return sendPacket(buffer);
}

bool PacketInterface::measureLinkage(double current, double min_rpm, double low_duty, double resistance)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_DETECT_MOTOR_FLUX_LINKAGE;
    utility::buffer_append_double32(mSendBuffer, current, 1e3, &send_index);
    utility::buffer_append_double32(mSendBuffer, min_rpm, 1e3, &send_index);
    utility::buffer_append_double32(mSendBuffer, low_duty, 1e3, &send_index);
    utility::buffer_append_double32(mSendBuffer, resistance, 1e6, &send_index);
    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::measureEncoder(double current)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_DETECT_ENCODER;
    utility::buffer_append_double32(mSendBuffer, current, 1e3, &send_index);
    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::measureHallFoc(double current)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_DETECT_HALL_FOC;
    utility::buffer_append_double32(mSendBuffer, current, 1e3, &send_index);
    return sendPacket(mSendBuffer, send_index);
}

void PacketInterface::setSendCan(bool sendCan, unsigned int id)
{
    mSendCan = sendCan;
    mCanId = id;
}

void PacketInterface::startUdpConnection(QHostAddress ip, int port)
{
    mHostAddress = ip;
    mUdpPort = port;
    mUdpSocket->close();
    mUdpSocket->bind(QHostAddress::Any, mUdpPort + 1);
}

void PacketInterface::stopUdpConnection()
{
    mHostAddress = QHostAddress("0.0.0.0");
    mUdpPort = 0;
    mUdpSocket->close();
}

bool PacketInterface::isUdpConnected()
{
    return QString::compare(mHostAddress.toString(), "0.0.0.0") != 0;
}

bool PacketInterface::sendCustomAppData(QByteArray data)
{
    return sendCustomAppData((unsigned char*)data.data(), data.size());
}

bool PacketInterface::sendCustomAppData(unsigned char *data, unsigned int len)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_CUSTOM_APP_DATA;
    memcpy(mSendBuffer + send_index, data, len);
    send_index += len;
    return sendPacket(mSendBuffer, send_index);
}

bool PacketInterface::setChukData(chuck_data &data)
{
    qint32 send_index = 0;
    mSendBuffer[send_index++] = COMM_SET_CHUCK_DATA;
    mSendBuffer[send_index++] = data.js_x;
    mSendBuffer[send_index++] = data.js_y;
    mSendBuffer[send_index++] = data.bt_c;
    mSendBuffer[send_index++] = data.bt_z;
    utility::buffer_append_int16(mSendBuffer, data.acc_x, &send_index);
    utility::buffer_append_int16(mSendBuffer, data.acc_y, &send_index);
    utility::buffer_append_int16(mSendBuffer, data.acc_z, &send_index);
    return sendPacket(mSendBuffer, send_index);
}
