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
#include "serialization.h"

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
    PacketInterface::mc_configuration getMcconfGui();
    void setMcconfGui(const PacketInterface::mc_configuration &mcconf);

private slots:
    void serialDataAvailable();
    void timerSlot();
    void packetDataToSend(QByteArray &data);
    void mcValuesReceived(PacketInterface::MC_VALUES values);
    void printReceived(QString str);
    void samplesReceived(QByteArray data);
    void rotorPosReceived(double pos);
    void experimentSamplesReceived(QVector<double> samples);
    void mcconfReceived(PacketInterface::mc_configuration mcconf);
    void motorParamReceived(double cycle_int_limit, double bemf_coupling_k);
    void appconfReceived(PacketInterface::app_configuration appconf);
    void decodedPpmReceived(double ppm_value, double ppm_last_len);
    void decodedChukReceived(double chuk_value);

    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_getDataButton_clicked();
    void on_getDataStartButton_clicked();
    void on_dutyButton_clicked();
    void on_rpmButton_clicked();
    void on_currentButton_clicked();
    void on_offButton_clicked();
    void on_offBrakeButton_clicked();
    void on_replotButton_clicked();
    void on_rescaleButton_clicked();
    void on_horizontalZoomBox_clicked();
    void on_verticalZoomBox_clicked();
    void on_filterLogScaleBox_clicked(bool checked);
    void on_filterLogScaleBox2_clicked(bool checked);
    void on_detectButton_clicked();
    void on_clearTerminalButton_clicked();
    void on_sendTerminalButton_clicked();
    void on_stopDetectButton_clicked();
    void on_experimentClearSamplesButton_clicked();
    void on_experimentSaveSamplesButton_clicked();
    void on_mcconfReadButton_clicked();
    void on_mcconfWriteButton_clicked();
    void on_currentBrakeButton_clicked();
    void on_mcconfLoadXmlButton_clicked();
    void on_mcconfSaveXmlButton_clicked();
    void on_mcconfDetectMotorParamButton_clicked();
    void on_appconfReadButton_clicked();
    void on_appconfWriteButton_clicked();
    void on_appconfRebootButton_clicked();
    void on_posCtrlButton_clicked();

private:
    Ui::MainWindow *ui;
    SerialPort *mPort;
    QTimer *mTimer;
    QLabel *mStatusLabel;
    Serialization *mSerialization;
    int mSampleInt;
    QByteArray curr1Array;
    QByteArray curr2Array;
    QByteArray ph1Array;
    QByteArray ph2Array;
    QByteArray ph3Array;
    QByteArray vZeroArray;
    QByteArray statusArray;
    QByteArray currTotArray;
    QByteArray fSwArray;
    QByteArray tmpCurr1Array;
    QByteArray tmpCurr2Array;
    QByteArray tmpPh1Array;
    QByteArray tmpPh2Array;
    QByteArray tmpPh3Array;
    QByteArray tmpVZeroArray;
    QByteArray tmpStatusArray;
    QByteArray tmpCurrTotArray;
    QByteArray tmpFSwArray;

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
    QVector<double> positionVec;

    bool mRealtimeGraphsAdded;
    bool mDoReplot;
    bool mDoReplotPos;
    bool mDoRescale;
    bool mDoFilterReplot;
    PacketInterface *mPacketInterface;
    bool keyLeft;
    bool keyRight;
    bool mcconfLoaded;
    bool appconfLoaded;

    QVector<QVector<double> > mExperimentSamples;

    void appendDoubleAndTrunc(QVector<double> *vec, double num, int maxSize);
    void clearBuffers();
    void saveExperimentSamplesToFile(QString path);

};

#endif // MAINWINDOW_H
