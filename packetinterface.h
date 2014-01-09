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
#include "locpoint.h"

class PacketInterface : public QObject
{
    Q_OBJECT
public:
    typedef struct {
        double v_in;
        double temp_mos1;
        double temp_mos2;
        double temp_mos3;
        double temp_mos4;
        double temp_mos5;
        double temp_mos6;
        double temp_pcb;
        double current_motor;
        double current_in;
        double rpm;
        double duty_now;
    } MC_VALUES;

    typedef struct {
        quint8 address;
        quint16 value;
    } ULTRA_SENSOR_VALUE;

    // Packets that expect response
    typedef enum {
        CAR_PACKET_READ_VALUES = 0,
        CAR_PACKET_READ_POS,
        CAR_PACKET_READ_SENS_ULTRA,
        CAR_PACKET_READ_SENS_IR,
        CAR_PACKET_READ_TRAVEL_COUNTER,
        CAR_PACKET_PRINT,
        CAR_PACKET_SEND_SAMPLES,
        CAR_PACKET_PING
    } CAR_RES_PACKET_ID;

    // Packets that only expect ack
    typedef enum {
        CAR_PACKET_SET_POWER_SERVO = 0,
        CAR_PACKET_WRITE_POS,
        CAR_PACKET_ADD_POINT,
        CAR_PACKET_AP_RUN,
        CAR_PACKET_AP_CLEAR,
        CAR_PACKET_RESET_TRAVEL_CNT,
        CAR_PACKET_SET_LIMITED,
        CAR_PACKET_FULL_BRAKE,
        CAR_PACKET_SERVO_OFFSET,
        CAR_PACKET_CAN_TEST,
        CAR_PACKET_TERMINAL_CMD
    } CAR_NORES_PACKET_ID;

    typedef enum {
        CAR_PACKET_RES = 1,
        CAR_PACKET_NORES = 2
    } CAR_SPECIAL_CMD;

    explicit PacketInterface(QObject *parent = 0);
    void processData(QByteArray &data);
    bool sendPacket(const unsigned char *data, int len);
    bool sendPacket(QByteArray data);
    bool setMotorServo(double speed, unsigned char dir);
    bool setPosition(LocPoint &pos);
    bool readValues();
    bool readPosition();
    bool readSensorsUltra();
    bool testCan();
    bool sendTerminalCmd(QString cmd);

signals:
    void dataToSend(QByteArray &data);
    void carValuesReceived(PacketInterface::MC_VALUES values);
    void carPosReceived(LocPoint pos);
    void carPingReceived();
    void carResponseTimedOut();
    void carSensorsUltraReceived(QVector<PacketInterface::ULTRA_SENSOR_VALUE> values);
    void carPrintReceived(QString str);
    void carSamplesReceived(QByteArray bytes);
    
public slots:
    void timerSlot();

private:
    unsigned short crc16(const unsigned char *buf, unsigned int len);
    void processPacket(const unsigned char *data, int len);
    qint16 getInt16FromBuffer(const unsigned char *buffer, unsigned int *index);
    qint32 getInt32FromBuffer(const unsigned char *buffer, unsigned int *index);

    QTimer *mTimer;

    // Packet state machine variables
    int mRxTimer;
    int mRxState;
    unsigned int mPayloadLength;
    unsigned char mRxBuffer[32768];
    unsigned char mRxDataPtr;
    unsigned char mCrcLow;
    unsigned char mCrcHigh;
    
};

#endif // PACKETINTERFACE_H
