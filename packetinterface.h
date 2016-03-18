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


    Q_INVOKABLE void setLimitedMode(bool is_limited);
    Q_INVOKABLE bool isLimitedMode();
    Q_INVOKABLE bool getFwVersion();
    Q_INVOKABLE bool startFirmwareUpload(QByteArray &newFirmware);
    Q_INVOKABLE double getFirmwareUploadProgress();
    Q_INVOKABLE QString getFirmwareUploadStatus();
    Q_INVOKABLE void cancelFirmwareUpload();
    Q_INVOKABLE bool getValues();
    Q_INVOKABLE bool sendTerminalCmd(QString cmd);
    Q_INVOKABLE bool setDutyCycle(double dutyCycle);
    Q_INVOKABLE bool setCurrent(double current);
    Q_INVOKABLE bool setCurrentBrake(double current);
    Q_INVOKABLE bool setRpm(int rpm);
    Q_INVOKABLE bool setPos(double pos);
    Q_INVOKABLE bool setDetect(disp_pos_mode mode);
    Q_INVOKABLE bool samplePrint(bool at_start, int sample_len, int dec);
    Q_INVOKABLE bool getMcconf();
    Q_INVOKABLE bool getMcconfDefault();
    Q_INVOKABLE bool setMcconf(const mc_configuration &mcconf);
    Q_INVOKABLE bool detectMotorParam(double current, double min_rpm, double low_duty);
    Q_INVOKABLE bool getAppConf();
    Q_INVOKABLE bool getAppConfDefault();
    Q_INVOKABLE bool setAppConf(const app_configuration &appconf);
    Q_INVOKABLE bool reboot();
    Q_INVOKABLE bool sendAlive();
    Q_INVOKABLE bool getDecodedPpm();
    Q_INVOKABLE bool getDecodedAdc();
    Q_INVOKABLE bool getDecodedChuk();
    Q_INVOKABLE bool setServoPos(double pos);
    Q_INVOKABLE bool measureRL();
    Q_INVOKABLE bool measureLinkage(double current, double min_rpm, double low_duty, double resistance);
    Q_INVOKABLE bool measureEncoder(double current);
    Q_INVOKABLE bool measureHallFoc(double current);
    Q_INVOKABLE void setSendCan(bool mSendCan, unsigned int id);
    Q_INVOKABLE void startUdpConnection(QHostAddress ip, int port);
    Q_INVOKABLE void stopUdpConnection();
    Q_INVOKABLE bool isUdpConnected();
    Q_INVOKABLE bool sendCustomAppData(QByteArray data);
    Q_INVOKABLE bool sendCustomAppData(unsigned char *data, unsigned int len);
    Q_INVOKABLE bool setChukData(chuck_data &data);
public slots:
    void processData(const QByteArray &data);
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
    bool sendPacket(const unsigned char *data, unsigned int len_packet);
    bool sendPacket(QByteArray data);
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
