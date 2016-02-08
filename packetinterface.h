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

#ifndef PACKETINTERFACE_H
#define PACKETINTERFACE_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QUdpSocket>
#include "datatypes.h"

class PacketInterface : public QObject
{
    Q_OBJECT
public:
    explicit PacketInterface(QObject *parent = 0);
    ~PacketInterface();

    bool sendPacket(const unsigned char *data, unsigned int len_packet);
    bool sendPacket(QByteArray data);
    void processData(QByteArray &data);
    void setLimitedMode(bool is_limited);
    bool isLimitedMode();
    bool getFwVersion();
    bool startFirmwareUpload(QByteArray &newFirmware);
    double getFirmwareUploadProgress();
    QString getFirmwareUploadStatus();
    void cancelFirmwareUpload();
    bool getValues();
    bool sendTerminalCmd(QString cmd);
    bool setDutyCycle(double dutyCycle);
    bool setCurrent(double current);
    bool setCurrentBrake(double current);
    bool setRpm(int rpm);
    bool setPos(double pos);
    bool setDetect(disp_pos_mode mode);
    bool samplePrint(bool at_start, int sample_len, int dec);
    bool getMcconf();
    bool getMcconfDefault();
    bool setMcconf(const mc_configuration &mcconf);
    bool detectMotorParam(double current, double min_rpm, double low_duty);
    bool getAppConf();
    bool getAppConfDefault();
    bool setAppConf(const app_configuration &appconf);
    bool reboot();
    bool sendAlive();
    bool getDecodedPpm();
    bool getDecodedAdc();
    bool getDecodedChuk();
    bool setServoPos(double pos);
    bool measureRL();
    bool measureLinkage(double current, double min_rpm, double low_duty, double resistance);
    bool measureEncoder(double current);
    bool measureHallFoc(double current);
    void setSendCan(bool mSendCan, unsigned int id);
    void startUdpConnection(QHostAddress ip, int port);
    void stopUdpConnection();
    bool isUdpConnected();
    bool sendCustomAppData(QByteArray data);
    bool sendCustomAppData(unsigned char *data, unsigned int len);
    bool setChukData(chuck_data &data);

signals:
    void dataToSend(QByteArray &data);
    void fwVersionReceived(int major, int minor);
    void ackReceived(QString ackType);
    void valuesReceived(MC_VALUES values);
    void printReceived(QString str);
    void samplesReceived(QByteArray bytes);
    void rotorPosReceived(double pos);
    void experimentSamplesReceived(QVector<double> samples);
    void mcconfReceived(mc_configuration mcconf);
    void motorParamReceived(double cycle_int_limit, double bemf_coupling_k, QVector<int> hall_table, int hall_res);
    void appconfReceived(app_configuration appconf);
    void decodedPpmReceived(double value, double last_len);
    void decodedAdcReceived(double value, double voltage, double value2, double voltage2);
    void decodedChukReceived(double value);
    void motorRLReceived(double r, double l);
    void motorLinkageReceived(double flux_linkage);
    void encoderParamReceived(double offset, double ratio, bool inverted);
    void customAppDataReceived(QByteArray data);
    void focHallTableReceived(QVector<int> hall_table, int res);
    
public slots:
    void timerSlot();
    void readPendingDatagrams();

private:
    unsigned short crc16(const unsigned char *buf, unsigned int len);
    void processPacket(const unsigned char *data, int len);
    QString faultToStr(mc_fault_code fault);
    void firmwareUploadUpdate(bool isTimeout);

    QTimer *mTimer;
    quint8 *mSendBuffer;
    bool mSendCan;
    unsigned int mCanId;
    bool mIsLimitedMode;
    QUdpSocket *mUdpSocket;
    QHostAddress mHostAddress;
    int mUdpPort;

    // FW upload state
    QByteArray mNewFirmware;
    bool mFirmwareIsUploading;
    int mFirmwareState;
    int mFimwarePtr;
    int mFirmwareTimer;
    int mFirmwareRetries;
    QString mFirmwareUploadStatus;

    // Packet state machine variables
    static const unsigned int mMaxBufferLen = 4096;
    int mRxTimer;
    int mRxState;
    unsigned int mPayloadLength;
    unsigned char mRxBuffer[mMaxBufferLen];
    unsigned int mRxDataPtr;
    unsigned char mCrcLow;
    unsigned char mCrcHigh;
    
};

#endif // PACKETINTERFACE_H
