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

    // Packets that expect response
    typedef enum {
        COMM_READ_VALUES = 0,
        COMM_PRINT,
        COMM_SEND_SAMPLES,
        COMM_ROTOR_POSITION
    } COMM_RES_PACKET_ID;

    // Packets that don't expect any response
    typedef enum {
        COMM_FULL_BRAKE = 0,
        COMM_SERVO_OFFSET,
        COMM_CAN_TEST,
        COMM_TERMINAL_CMD,
        COMM_RELEASE
    } COMM_NORES_PACKET_ID;

    typedef enum {
        COMM_PACKET_RES = 1,
        COMM_PACKET_NORES = 2
    } COMM_SPECIAL_CMD;

    explicit PacketInterface(QObject *parent = 0);
    void processData(QByteArray &data);
    bool sendPacket(const unsigned char *data, int len);
    bool sendPacket(QByteArray data);
    bool readValues();
    bool testCan();
    bool sendTerminalCmd(QString cmd);

signals:
    void dataToSend(QByteArray &data);
    void valuesReceived(PacketInterface::MC_VALUES values);
    void printReceived(QString str);
    void samplesReceived(QByteArray bytes);
    void rotorPosReceived(double pos);
    
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
