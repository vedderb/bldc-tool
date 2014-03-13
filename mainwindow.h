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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QByteArray>
#include "qcustomplot.h"
#include "serialport.h"
#include "packetinterface.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool eventFilter(QObject *object, QEvent *e);
    bool setDytyCycle(double duty);

private slots:
    void serialDataAvailable();
    void timerSlot();
    void packetDataToSend(QByteArray &data);
    void mcValuesReceived(PacketInterface::MC_VALUES values);
    void printReceived(QString str);
    void samplesReceived(QByteArray data);
    void rotorPosReceived(double pos);

    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_getDataButton_clicked();
    void on_getDataStartButton_clicked();
    void on_dutyButton_clicked();
    void on_rpmButton_clicked();
    void on_offButton_clicked();
    void on_replotButton_clicked();
    void on_rescaleButton_clicked();
    void on_horizontalZoomBox_clicked();
    void on_verticalZoomBox_clicked();
    void on_filterLogScaleBox_clicked(bool checked);
    void on_filterLogScaleBox2_clicked(bool checked);
    void on_detectButton_clicked();
    void on_resetBufferButton_clicked();
    void on_clearTerminalButton_clicked();
    void on_resetFaultButton_clicked();
    void on_sendTerminalButton_clicked();

private:
    Ui::MainWindow *ui;
    SerialPort *mPort;
    QTimer *mTimer;
    QLabel *mStatusLabel;
    int mSampleInt;
    QByteArray curr1Array;
    QByteArray curr2Array;
    QByteArray ph1Array;
    QByteArray ph2Array;
    QByteArray ph3Array;
    QByteArray vZeroArray;
    QByteArray statusArray;
    QByteArray currTotArray;
    QByteArray tmpCurr1Array;
    QByteArray tmpCurr2Array;
    QByteArray tmpPh1Array;
    QByteArray tmpPh2Array;
    QByteArray tmpPh3Array;
    QByteArray tmpVZeroArray;
    QByteArray tmpStatusArray;
    QByteArray tmpCurrTotArray;

    QVector<double> tempMos1Vec;
    QVector<double> tempMos2Vec;
    QVector<double> tempMos3Vec;
    QVector<double> tempMos4Vec;
    QVector<double> tempMos5Vec;
    QVector<double> tempMos6Vec;
    QVector<double> tempPcbVec;
    QVector<double> currInVec;
    QVector<double> currMotorVec;
    QVector<double> dutyVec;
    QVector<double> rpmVec;
    QVector<double> voltInVec;

    bool mRealtimeGraphsAdded;
    bool mDoReplot;
    bool mDoRescale;
    bool mDoFilterReplot;
    PacketInterface *mPacketInterface;
    bool keyLeft;
    bool keyRight;

    void appendDoubleAndTrunc(QVector<double> *vec, double num, int maxSize);
    void clearBuffers();
};

#endif // MAINWINDOW_H
