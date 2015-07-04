/*
    Copyright 2012-2015 Benjamin Vedder	benjamin@vedder.se

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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <string.h>
#include <cmath>
#include <QMessageBox>
#include "digitalfiltering.h"

namespace {
void stepTowards(double &value, double goal, double step) {
    if (value < goal) {
        if ((value + step) < goal) {
            value += step;
        } else {
            value = goal;
        }
    } else if (value > goal) {
        if ((value - step) > goal) {
            value -= step;
        } else {
            value = goal;
        }
    }
}
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    ui->serialDeviceEdit->setText("COM3");
#elif defined(Q_OS_MAC)
    ui->serialDeviceEdit->setText("/dev/tty.usbmodem261");
#else
    ui->serialDeviceEdit->setText("/dev/ttyACM0");
#endif

    // Compatible firmwares
    mFwVersionReceived = false;
    mCompatibleFws.append(qMakePair(1, 5));
    mCompatibleFws.append(qMakePair(1, 6));
    mCompatibleFws.append(qMakePair(1, 7));

    QString supportedFWs;
    for (int i = 0;i < mCompatibleFws.size();i++) {
        QString tmp;
        tmp.sprintf("%d.%d", mCompatibleFws.at(i).first, mCompatibleFws.at(i).second);
        if (i < (mCompatibleFws.size() - 1)) {
            tmp.append("\n");
        }
        supportedFWs.append(tmp);
    }
    ui->firmwareSupportedLabel->setText(supportedFWs);

    mPort = new QSerialPort(this);

    mTimer = new QTimer(this);
    mTimer->setInterval(20);
    mTimer->start();

    mStatusLabel = new QLabel(this);
    ui->statusBar->addPermanentWidget(mStatusLabel);
    mSampleInt = 0;
    mDoReplot = false;
    mDoReplotPos = false;
    mDoRescale = true;
    mDoFilterReplot = true;
    mPacketInterface = new PacketInterface(this);
    mRealtimeGraphsAdded = false;
    keyLeft = false;
    keyRight = false;
    mMcconfLoaded = false;
    mAppconfLoaded = false;
    mStatusInfoTime = 0;

    connect(mPort, SIGNAL(readyRead()),
            this, SLOT(serialDataAvailable()));
    connect(mPort, SIGNAL(error(QSerialPort::SerialPortError)),
            this, SLOT(serialPortError(QSerialPort::SerialPortError)));
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));

    connect(mPacketInterface, SIGNAL(dataToSend(QByteArray&)),
            this, SLOT(packetDataToSend(QByteArray&)));
    connect(mPacketInterface, SIGNAL(fwVersionReceived(int,int)),
            this, SLOT(fwVersionReceived(int,int)));
    connect(mPacketInterface, SIGNAL(ackReceived(QString)),
            this, SLOT(ackReceived(QString)));
    connect(mPacketInterface, SIGNAL(valuesReceived(PacketInterface::MC_VALUES)),
            this, SLOT(mcValuesReceived(PacketInterface::MC_VALUES)));
    connect(mPacketInterface, SIGNAL(printReceived(QString)),
            this, SLOT(printReceived(QString)));
    connect(mPacketInterface, SIGNAL(samplesReceived(QByteArray)),
            this, SLOT(samplesReceived(QByteArray)));
    connect(mPacketInterface, SIGNAL(rotorPosReceived(double)),
            this, SLOT(rotorPosReceived(double)));
    connect(mPacketInterface, SIGNAL(experimentSamplesReceived(QVector<double>)),
            this, SLOT(experimentSamplesReceived(QVector<double>)));
    connect(mPacketInterface, SIGNAL(mcconfReceived(PacketInterface::mc_configuration)),
            this, SLOT(mcconfReceived(PacketInterface::mc_configuration)));
    connect(mPacketInterface, SIGNAL(motorParamReceived(double,double,QVector<int>,int)),
            this, SLOT(motorParamReceived(double,double,QVector<int>,int)));
    connect(mPacketInterface, SIGNAL(appconfReceived(PacketInterface::app_configuration)),
            this, SLOT(appconfReceived(PacketInterface::app_configuration)));
    connect(mPacketInterface, SIGNAL(decodedPpmReceived(double,double)),
            this, SLOT(decodedPpmReceived(double,double)));
    connect(mPacketInterface, SIGNAL(decodedAdcReceived(double,double)),
            this, SLOT(decodedAdcReceived(double,double)));
    connect(mPacketInterface, SIGNAL(decodedChukReceived(double)),
            this, SLOT(decodedChukReceived(double)));

    mSerialization = new Serialization(this);

    ui->currentPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->voltagePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->filterPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->filterResponsePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->filterPlot2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->filterResponsePlot2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->realtimePlotTemperature->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->realtimePlotRest->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->realtimePlotPosition->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    QFont legendFont = font();
    legendFont.setPointSize(9);
    ui->realtimePlotPosition->addGraph();
    ui->realtimePlotPosition->graph(0)->setPen(QPen(Qt::blue));
    ui->realtimePlotPosition->graph(0)->setName("Position");
    ui->realtimePlotPosition->legend->setVisible(true);
    ui->realtimePlotPosition->legend->setFont(legendFont);
    ui->realtimePlotPosition->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom);
    ui->realtimePlotPosition->legend->setBrush(QBrush(QColor(255,255,255,230)));
    ui->realtimePlotPosition->xAxis->setLabel("Sample");
    ui->realtimePlotPosition->yAxis->setLabel("Deg");

    qApp->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *object, QEvent *e)
{
    Q_UNUSED(object);

    if (!mPort->isOpen() || !ui->overrideKbBox->isChecked()) {
        return false;
    }

    if (e->type() == QEvent::KeyPress || e->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
        bool isPress = e->type() == QEvent::KeyPress;

        switch(keyEvent->key()) {
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_PageDown:
            break;

        default:
            return false;
        }

        if(keyEvent->isAutoRepeat()) {
            return true;
        }

        switch(keyEvent->key()) {
        case Qt::Key_Up:
            if (isPress) {
                mPacketInterface->setCurrent(ui->arrowCurrentBox->value());
            } else {
                mPacketInterface->setCurrent(0.0);
            }
            break;

        case Qt::Key_Down:
            if (isPress) {
                mPacketInterface->setCurrent(-ui->arrowCurrentBox->value());
            } else {
                mPacketInterface->setCurrent(0.0);
            }
            break;

        case Qt::Key_Left:
            if (isPress) {
                keyLeft = true;
            } else {
                keyLeft = false;
            }
            break;

        case Qt::Key_Right:
            if (isPress) {
                keyRight = true;
            } else {
                keyRight = false;
            }
            break;

        case Qt::Key_PageDown:
            if (isPress) {
                mPacketInterface->setCurrentBrake(-ui->arrowCurrentBox->value());
            } else {
                mPacketInterface->setCurrentBrake(0.0);
            }
            break;

        default:
            break;
        }

        return true;
    }

    return false;
}

PacketInterface::mc_configuration MainWindow::getMcconfGui()
{
    PacketInterface::mc_configuration mcconf;

    if (ui->mcconfPwmModeSyncButton->isChecked()) {
        mcconf.pwm_mode = PacketInterface::PWM_MODE_SYNCHRONOUS;
    } else if (ui->mcconfPwmModeBipolarButton->isChecked()) {
        mcconf.pwm_mode = PacketInterface::PWM_MODE_BIPOLAR;
    } else if (ui->mcconfPwmModeNonsyncHiswButton->isChecked()) {
        mcconf.pwm_mode = PacketInterface::PWM_MODE_NONSYNCHRONOUS_HISW;
    }

    if (ui->mcconfCommIntButton->isChecked()) {
        mcconf.comm_mode = PacketInterface::COMM_MODE_INTEGRATE;
    } else if (ui->mcconfCommDelayButton->isChecked()) {
        mcconf.comm_mode = PacketInterface::COMM_MODE_DELAY;
    }

    if (ui->mcconfTypeBldcButton->isChecked()) {
        mcconf.motor_type = PacketInterface::MOTOR_TYPE_BLDC;
    } else if (ui->mcconfTypeDcButton->isChecked()) {
        mcconf.motor_type = PacketInterface::MOTOR_TYPE_DC;
    }

    if (ui->mcconfSensorModeSensorlessButton->isChecked()) {
        mcconf.sensor_mode = PacketInterface::SENSOR_MODE_SENSORLESS;
    } else if (ui->mcconfSensorModeSensoredButton->isChecked()) {
        mcconf.sensor_mode = PacketInterface::SENSOR_MODE_SENSORED;
    } else if (ui->mcconfSensorModeHybridButton->isChecked()) {
        mcconf.sensor_mode = PacketInterface::SENSOR_MODE_HYBRID;
    }

    mcconf.l_current_max = ui->mcconfLimCurrentMaxBox->value();
    mcconf.l_current_min = ui->mcconfLimCurrentMinBox->value();
    mcconf.l_in_current_max = ui->mcconfLimCurrentInMaxBox->value();
    mcconf.l_in_current_min = ui->mcconfLimCurrentInMinBox->value();
    mcconf.l_abs_current_max = ui->mcconfLimCurrentAbsMaxBox->value();
    mcconf.l_slow_abs_current = ui->mcconfLimCurrentSlowAbsMaxBox->isChecked();
    mcconf.l_max_erpm = ui->mcconfLimMaxErpmBox->value();
    mcconf.l_min_erpm = ui->mcconfLimMinErpmBox->value();
    mcconf.l_max_erpm_fbrake = ui->mcconfLimMaxErpmFbrakeBox->value();
    mcconf.l_max_erpm_fbrake_cc = ui->mcconfLimMaxErpmFbrakeCcBox->value();
    mcconf.l_rpm_lim_neg_torque = ui->mcconfLimErpmLimitNegTorqueBox->isChecked();
    mcconf.l_min_vin = ui->mcconfLimMinVinBox->value();
    mcconf.l_max_vin = ui->mcconfLimMaxVinBox->value();
    mcconf.l_temp_fet_start = ui->mcconfLimTempFetStartBox->value();
    mcconf.l_temp_fet_end = ui->mcconfLimTempFetEndBox->value();
    mcconf.l_temp_motor_start = ui->mcconfLimTempMotorStartBox->value();
    mcconf.l_temp_motor_end = ui->mcconfLimTempMotorEndBox->value();
    mcconf.l_min_duty = ui->mcconfLimMinDutyBox->value();
    mcconf.l_max_duty = ui->mcconfLimMaxDutyBox->value();

    mcconf.sl_min_erpm = ui->mcconfSlMinErpmBox->value();
    mcconf.sl_max_fullbreak_current_dir_change = ui->mcconfSlMaxFbCurrBox->value();
    mcconf.sl_min_erpm_cycle_int_limit = ui->mcconfSlMinErpmIlBox->value();
    mcconf.sl_cycle_int_limit = ui->mcconfSlIntLimBox->value();
    mcconf.sl_phase_advance_at_br = ui->mcconfSlIntLimScaleBrBox->value();
    mcconf.sl_cycle_int_rpm_br = ui->mcconfSlBrErpmBox->value();
    mcconf.sl_bemf_coupling_k = ui->mcconfSlBemfKBox->value();

    mcconf.hall_table[0] = ui->mcconfHallTab0Box->value();
    mcconf.hall_table[1] = ui->mcconfHallTab1Box->value();
    mcconf.hall_table[2] = ui->mcconfHallTab2Box->value();
    mcconf.hall_table[3] = ui->mcconfHallTab3Box->value();
    mcconf.hall_table[4] = ui->mcconfHallTab4Box->value();
    mcconf.hall_table[5] = ui->mcconfHallTab5Box->value();
    mcconf.hall_table[6] = ui->mcconfHallTab6Box->value();
    mcconf.hall_table[7] = ui->mcconfHallTab7Box->value();
    mcconf.hall_sl_erpm = ui->mcconfHallSlErpmBox->value();

    mcconf.s_pid_kp = ui->mcconfSpidKpBox->value();
    mcconf.s_pid_ki = ui->mcconfSpidKiBox->value();
    mcconf.s_pid_kd = ui->mcconfSpidKdBox->value();
    mcconf.s_pid_min_rpm = ui->mcconfSpidMinRpmBox->value();

    mcconf.p_pid_kp = ui->mcconfPpidKpBox->value();
    mcconf.p_pid_ki = ui->mcconfPpidKiBox->value();
    mcconf.p_pid_kd = ui->mcconfPpidKdBox->value();

    mcconf.cc_startup_boost_duty = ui->mcconfCcBoostBox->value();
    mcconf.cc_min_current = ui->mcconfCcMinBox->value();
    mcconf.cc_gain = ui->mcconfCcGainBox->value();
    mcconf.cc_ramp_step_max = ui->mcconfCcMaxRampStepBox->value();

    mcconf.m_fault_stop_time_ms = ui->mcconfMFaultStopTimeBox->value();

    mcconf.meta_description = ui->mcconfDescEdit->toHtml();

    return mcconf;
}

void MainWindow::setMcconfGui(const PacketInterface::mc_configuration &mcconf)
{
    switch (mcconf.pwm_mode) {
    case PacketInterface::PWM_MODE_SYNCHRONOUS:
        ui->mcconfPwmModeSyncButton->setChecked(true);
        break;

    case PacketInterface::PWM_MODE_BIPOLAR:
        ui->mcconfPwmModeBipolarButton->setChecked(true);
        break;

    case PacketInterface::PWM_MODE_NONSYNCHRONOUS_HISW:
        ui->mcconfPwmModeNonsyncHiswButton->setChecked(true);
        break;

    default:
        break;
    }

    switch (mcconf.comm_mode) {
    case PacketInterface::COMM_MODE_INTEGRATE:
        ui->mcconfCommIntButton->setChecked(true);
        break;

    case PacketInterface::COMM_MODE_DELAY:
        ui->mcconfCommDelayButton->setChecked(true);
        break;

    default:
        break;

    }

    switch (mcconf.motor_type) {
    case PacketInterface::MOTOR_TYPE_BLDC:
        ui->mcconfTypeBldcButton->setChecked(true);
        break;

    case PacketInterface::MOTOR_TYPE_DC:
        ui->mcconfTypeDcButton->setChecked(true);
        break;

    default:
        break;
    }

    switch (mcconf.sensor_mode) {
    case PacketInterface::SENSOR_MODE_SENSORLESS:
        ui->mcconfSensorModeSensorlessButton->setChecked(true);
        break;

    case PacketInterface::SENSOR_MODE_SENSORED:
        ui->mcconfSensorModeSensoredButton->setChecked(true);
        break;

    case PacketInterface::SENSOR_MODE_HYBRID:
        ui->mcconfSensorModeHybridButton->setChecked(true);
        break;

    default:
        break;
    }

    ui->mcconfLimCurrentMaxBox->setValue(mcconf.l_current_max);
    ui->mcconfLimCurrentMinBox->setValue(mcconf.l_current_min);
    ui->mcconfLimCurrentInMaxBox->setValue(mcconf.l_in_current_max);
    ui->mcconfLimCurrentInMinBox->setValue(mcconf.l_in_current_min);
    ui->mcconfLimCurrentAbsMaxBox->setValue(mcconf.l_abs_current_max);
    ui->mcconfLimCurrentSlowAbsMaxBox->setChecked(mcconf.l_slow_abs_current);
    ui->mcconfLimMaxErpmBox->setValue(mcconf.l_max_erpm);
    ui->mcconfLimMinErpmBox->setValue(mcconf.l_min_erpm);
    ui->mcconfLimMaxErpmFbrakeBox->setValue(mcconf.l_max_erpm_fbrake);
    ui->mcconfLimMaxErpmFbrakeCcBox->setValue(mcconf.l_max_erpm_fbrake_cc);
    ui->mcconfLimErpmLimitNegTorqueBox->setChecked(mcconf.l_rpm_lim_neg_torque);
    ui->mcconfLimMinVinBox->setValue(mcconf.l_min_vin);
    ui->mcconfLimMaxVinBox->setValue(mcconf.l_max_vin);
    ui->mcconfLimTempFetStartBox->setValue(mcconf.l_temp_fet_start);
    ui->mcconfLimTempFetEndBox->setValue(mcconf.l_temp_fet_end);
    ui->mcconfLimTempMotorStartBox->setValue(mcconf.l_temp_motor_start);
    ui->mcconfLimTempMotorEndBox->setValue(mcconf.l_temp_motor_end);
    ui->mcconfLimMinDutyBox->setValue(mcconf.l_min_duty);
    ui->mcconfLimMaxDutyBox->setValue(mcconf.l_max_duty);

    ui->mcconfSlMinErpmBox->setValue(mcconf.sl_min_erpm);
    ui->mcconfSlMaxFbCurrBox->setValue(mcconf.sl_max_fullbreak_current_dir_change);
    ui->mcconfSlMinErpmIlBox->setValue(mcconf.sl_min_erpm_cycle_int_limit);
    ui->mcconfSlIntLimBox->setValue(mcconf.sl_cycle_int_limit);
    ui->mcconfSlIntLimScaleBrBox->setValue(mcconf.sl_phase_advance_at_br);
    ui->mcconfSlBrErpmBox->setValue(mcconf.sl_cycle_int_rpm_br);
    ui->mcconfSlBemfKBox->setValue(mcconf.sl_bemf_coupling_k);

    ui->mcconfHallTab0Box->setValue(mcconf.hall_table[0]);
    ui->mcconfHallTab1Box->setValue(mcconf.hall_table[1]);
    ui->mcconfHallTab2Box->setValue(mcconf.hall_table[2]);
    ui->mcconfHallTab3Box->setValue(mcconf.hall_table[3]);
    ui->mcconfHallTab4Box->setValue(mcconf.hall_table[4]);
    ui->mcconfHallTab5Box->setValue(mcconf.hall_table[5]);
    ui->mcconfHallTab6Box->setValue(mcconf.hall_table[6]);
    ui->mcconfHallTab7Box->setValue(mcconf.hall_table[7]);
    ui->mcconfHallSlErpmBox->setValue(mcconf.hall_sl_erpm);

    ui->mcconfSpidKpBox->setValue(mcconf.s_pid_kp);
    ui->mcconfSpidKiBox->setValue(mcconf.s_pid_ki);
    ui->mcconfSpidKdBox->setValue(mcconf.s_pid_kd);
    ui->mcconfSpidMinRpmBox->setValue(mcconf.s_pid_min_rpm);

    ui->mcconfPpidKpBox->setValue(mcconf.p_pid_kp);
    ui->mcconfPpidKiBox->setValue(mcconf.p_pid_ki);
    ui->mcconfPpidKdBox->setValue(mcconf.p_pid_kd);

    ui->mcconfCcBoostBox->setValue(mcconf.cc_startup_boost_duty);
    ui->mcconfCcMinBox->setValue(mcconf.cc_min_current);
    ui->mcconfCcGainBox->setValue(mcconf.cc_gain);
    ui->mcconfCcMaxRampStepBox->setValue(mcconf.cc_ramp_step_max);

    ui->mcconfMFaultStopTimeBox->setValue(mcconf.m_fault_stop_time_ms);

    ui->mcconfDescEdit->document()->setHtml(mcconf.meta_description);

    mMcconfLoaded = true;
}

void MainWindow::showStatusInfo(QString info, bool isGood)
{
    if (isGood) {
        mStatusLabel->setStyleSheet("QLabel { background-color : lightgreen; color : black; }");
    } else {
        mStatusLabel->setStyleSheet("QLabel { background-color : red; color : black; }");
    }

    mStatusInfoTime = 80;
    mStatusLabel->setText(info);
}

void MainWindow::serialDataAvailable()
{
    while (mPort->bytesAvailable() > 0) {
        QByteArray data = mPort->readAll();
        mPacketInterface->processData(data);
    }
}

void MainWindow::serialPortError(QSerialPort::SerialPortError error)
{
    QString message;
    switch (error) {
    case QSerialPort::NoError:
        break;
    case QSerialPort::DeviceNotFoundError:
        message = tr("Device not found");
        break;
    case QSerialPort::OpenError:
        message = tr("Can't open device");
        break;
    case QSerialPort::NotOpenError:
        message = tr("Not open error");
        break;
    case QSerialPort::ResourceError:
        message = tr("Port disconnected");
        break;
    case QSerialPort::UnknownError:
        message = tr("Unknown error");
        break;
    default:
        message = QString::number(error);
        break;
    }

    if(!message.isEmpty()) {
        showStatusInfo(message, false);

        if(mPort->isOpen()) {
            mPort->close();
        }
    }
}


void MainWindow::timerSlot()
{
    // Update CAN fwd function
    mPacketInterface->setSendCan(ui->canFwdBox->isChecked(), ui->canIdBox->value());

    // Read FW version if needed
    static bool sendCanBefore = false;
    static int canIdBefore = 0;
    if (mPort->isOpen()) {
        if (sendCanBefore != ui->canFwdBox->isChecked() ||
                canIdBefore != ui->canIdBox->value()) {
            mFwVersionReceived = false;
        }

        if (!mFwVersionReceived) {
            mPacketInterface->getFwVersion();
        }

    } else {
        mFwVersionReceived = false;
    }
    sendCanBefore = ui->canFwdBox->isChecked();
    canIdBefore = ui->canIdBox->value();

    // Update status label
    if (mStatusInfoTime) {
        mStatusInfoTime--;
        if (!mStatusInfoTime) {
            mStatusLabel->setStyleSheet(qApp->styleSheet());
        }
    } else {
        if (mPort->isOpen()) {
            if (mPacketInterface->isLimitedMode()) {
                mStatusLabel->setText("Connected, limited");
            } else {
                mStatusLabel->setText("Connected");
            }
        } else {
            mStatusLabel->setText("Not connected");
        }
    }

    // Update fw upload bar and label
    double fw_prog = mPacketInterface->getFirmwareUploadProgress();
    if (fw_prog > -0.1) {
        ui->firmwareBar->setValue(fw_prog * 1000);
        ui->firmwareUploadButton->setEnabled(false);
    } else {
        ui->firmwareUploadButton->setEnabled(true);
    }
    ui->firmwareUploadStatusLabel->setText(mPacketInterface->getFirmwareUploadStatus());

    // Send alive command
    mPacketInterface->sendAlive();

    // Update MC readings
    if (ui->realtimeActivateBox->isChecked()) {
        mPacketInterface->getValues();
    }

    // Update decoded servo value
    if (ui->appconfUpdatePpmBox->isChecked()) {
        mPacketInterface->getDecodedPpm();
    }

    // Update decoded adc value
    if (ui->appconfAdcUpdateBox->isChecked()) {
        mPacketInterface->getDecodedAdc();
    }

    // Update decoded nunchuk value
    if (ui->appconfUpdateChukBox->isChecked()) {
        mPacketInterface->getDecodedChuk();
    }

    // Enable/disable fields in the configuration page
    static int isSlIntBefore = true;
    if (ui->mcconfCommIntButton->isChecked() != isSlIntBefore) {
        if (ui->mcconfCommIntButton->isChecked()) {
            ui->mcconfSlMinErpmBox->setEnabled(true);
            ui->mcconfSlMaxFbCurrBox->setEnabled(true);
            ui->mcconfSlBemfKBox->setEnabled(true);
            ui->mcconfSlBrErpmBox->setEnabled(true);
            ui->mcconfSlIntLimBox->setEnabled(true);
            ui->mcconfSlIntLimScaleBrBox->setEnabled(true);
            ui->mcconfSlMinErpmIlBox->setEnabled(true);
        } else {
            ui->mcconfSlMinErpmBox->setEnabled(true);
            ui->mcconfSlMaxFbCurrBox->setEnabled(true);
            ui->mcconfSlBemfKBox->setEnabled(false);
            ui->mcconfSlBrErpmBox->setEnabled(true);
            ui->mcconfSlIntLimBox->setEnabled(false);
            ui->mcconfSlIntLimScaleBrBox->setEnabled(true);
            ui->mcconfSlMinErpmIlBox->setEnabled(false);
        }
    }
    isSlIntBefore = ui->mcconfCommIntButton->isChecked();

    // Handle key events
    static double keyPower = 0.0;
    static double lastKeyPower = 0.0;
    const double lowPower = 0.18;
    const double lowPowerRev = 0.1;
    const double highPower = 0.9;
    const double highPowerRev = 0.3;
    const double lowStep = 0.02;
    const double highStep = 0.01;

    if (keyRight && keyLeft) {
        if (keyPower >= lowPower) {
            stepTowards(keyPower, highPower, highStep);
        } else if (keyPower <= -lowPower) {
            stepTowards(keyPower, -highPowerRev, highStep);
        } else if (keyPower >= 0) {
            stepTowards(keyPower, highPower, lowStep);
        } else {
            stepTowards(keyPower, -highPowerRev, lowStep);
        }
    } else if (keyRight) {
        if (fabs(keyPower) > lowPower) {
            stepTowards(keyPower, lowPower, highStep);
        } else {
            stepTowards(keyPower, lowPower, lowStep);
        }
    } else if (keyLeft) {
        if (fabs(keyPower) > lowPower) {
            stepTowards(keyPower, -lowPowerRev, highStep);
        } else {
            stepTowards(keyPower, -lowPowerRev, lowStep);
        }
    } else {
        stepTowards(keyPower, 0.0, lowStep * 3);
    }

    if (keyPower != lastKeyPower) {
        lastKeyPower = keyPower;
        mPacketInterface->setDutyCycle(keyPower);
    }

    // Update plots
    static QVector<double> filter;
    static QVector<double> filter2;
    DigitalFiltering f;
    QFont legendFont = font();
    legendFont.setPointSize(9);

    // Plot filters
    if (mDoFilterReplot) {
        if (ui->meanRadioButton->isChecked()) {
            filter.clear();
            int fLen = (1 << ui->currentFilterTapBox->value());
            for (int i = 0;i < fLen;i++) {
                filter.append(1.0 / (double)fLen);
            }
        } else {
            filter = f.generateFirFilter(ui->currentFilterFreqBox->value(),
                                         ui->currentFilterTapBox->value(), ui->hammingBox->isChecked());
        }

        if (ui->meanRadioButton2->isChecked()) {
            filter2.clear();
            int fLen2 = (1 << ui->currentFilterTapBox2->value());
            for (int i = 0;i < fLen2;i++) {
                filter2.append(1.0 / (double)fLen2);
            }
        } else {
            filter2 = f.generateFirFilter(ui->currentFilterFreqBox2->value(),
                                          ui->currentFilterTapBox2->value(), ui->hammingBox2->isChecked());
        }

        static int last_len = 0;
        bool len_diff = last_len != ui->currentFilterTapBox->value();
        last_len = ui->currentFilterTapBox->value();

        static int last_len2 = 0;
        bool len_diff2 = last_len2 != ui->currentFilterTapBox2->value();
        last_len2 = ui->currentFilterTapBox2->value();

        static int last_decimation = 0;
        bool decimation_diff = last_decimation != ui->decimationSpinBox->value();
        last_decimation = ui->decimationSpinBox->value();

        // Plot filter
        QVector<double> filterIndex;
        for(int i = 0;i < filter.size();i++) {
            filterIndex.append((double)i);
        }

        QVector<double> filterIndex2;
        for(int i = 0;i < filter2.size();i++) {
            filterIndex2.append((double)i);
        }

        ui->filterPlot->clearGraphs();
        ui->filterResponsePlot->clearGraphs();
        ui->filterPlot2->clearGraphs();
        ui->filterResponsePlot2->clearGraphs();

        ui->filterPlot->addGraph();
        ui->filterPlot->graph(0)->setData(filterIndex, filter);
        ui->filterPlot->graph(0)->setName("Filter");

        ui->filterPlot2->addGraph();
        ui->filterPlot2->graph(0)->setData(filterIndex2, filter2);
        ui->filterPlot2->graph(0)->setName("Filter");

        if (ui->filterScatterBox->isChecked()) {
            ui->filterPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
            ui->filterPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
        }

        if (ui->filterScatterBox2->isChecked()) {
            ui->filterPlot2->graph(0)->setLineStyle(QCPGraph::lsLine);
            ui->filterPlot2->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
        }

        // Plot response
        QVector<double> response = f.fftWithShift(filter, ui->currentFilterTapBox->value() + 4);
        QVector<double> response2 = f.fftWithShift(filter2, ui->currentFilterTapBox2->value() + 4);

        // Remove positive half
        response.resize(response.size() / 2);
        response2.resize(response2.size() / 2);

        filterIndex.clear();
        for(int i = 0;i < response.size();i++) {
            filterIndex.append(((double)i / (double)response.size()) * (double)ui->sampleFreqBox->value() / 2);
        }

        filterIndex2.clear();
        for(int i = 0;i < response2.size();i++) {
            filterIndex2.append(((double)i / (double)response2.size()) * (double)ui->sampleFreqBox->value() / (double)(2 * ui->decimationSpinBox->value()));
        }

        ui->filterResponsePlot->addGraph();
        ui->filterResponsePlot->graph(0)->setData(filterIndex, response);
        ui->filterResponsePlot->graph(0)->setName("Filter Response");
        ui->filterResponsePlot->graph(0)->setPen(QPen(Qt::red));

        ui->filterResponsePlot2->addGraph();
        ui->filterResponsePlot2->graph(0)->setData(filterIndex2, response2);
        ui->filterResponsePlot2->graph(0)->setName("Filter Response");
        ui->filterResponsePlot2->graph(0)->setPen(QPen(Qt::red));

        ui->filterPlot->legend->setVisible(true);
        ui->filterPlot->legend->setFont(legendFont);
        ui->filterPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom);
        ui->filterPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
        ui->filterPlot->xAxis->setLabel("Index");
        ui->filterPlot->yAxis->setLabel("Value");

        ui->filterPlot2->legend->setVisible(true);
        ui->filterPlot2->legend->setFont(legendFont);
        ui->filterPlot2->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom);
        ui->filterPlot2->legend->setBrush(QBrush(QColor(255,255,255,230)));
        ui->filterPlot2->xAxis->setLabel("Index");
        ui->filterPlot2->yAxis->setLabel("Value");

        ui->filterResponsePlot->legend->setVisible(true);
        ui->filterResponsePlot->legend->setFont(legendFont);
        ui->filterResponsePlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom);
        ui->filterResponsePlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
        ui->filterResponsePlot->xAxis->setLabel("Frequency (Hz)");
        ui->filterResponsePlot->yAxis->setLabel("Gain");

        ui->filterResponsePlot2->legend->setVisible(true);
        ui->filterResponsePlot2->legend->setFont(legendFont);
        ui->filterResponsePlot2->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom);
        ui->filterResponsePlot2->legend->setBrush(QBrush(QColor(255,255,255,230)));
        ui->filterResponsePlot2->xAxis->setLabel("Frequency (Hz)");
        ui->filterResponsePlot2->yAxis->setLabel("Gain");

        if (len_diff) {
            ui->filterPlot->rescaleAxes();
            ui->filterResponsePlot->rescaleAxes();
        }

        if (len_diff2) {
            ui->filterPlot2->rescaleAxes();
            ui->filterResponsePlot2->rescaleAxes();
        }

        if (decimation_diff) {
            ui->filterResponsePlot2->rescaleAxes();
        }

        ui->filterPlot->replot();
        ui->filterResponsePlot->replot();

        ui->filterPlot2->replot();
        ui->filterResponsePlot2->replot();
        mDoFilterReplot = false;
    }

    if (mDoReplot) {
        const double f_samp = (ui->sampleFreqBox->value() / ui->sampleIntBox->value());

        int size = curr1Array.size();

        QVector<double> curr1(size/2);
        for (int i=0; i<(size); i+=2) {
            curr1[i/2] = (double)((qint16)(((unsigned char)curr1Array[i] << 8) | (unsigned char)curr1Array[i + 1]));
        }

        QVector<double> curr2(size/2);
        for (int i=0; i<(size); i+=2) {
            curr2[i/2] = (double)((qint16)(((unsigned char)curr2Array[i] << 8) | (unsigned char)curr2Array[i + 1]));
        }

        QVector<double> ph1(size/2);
        for (int i=0; i<(size); i+=2) {
            ph1[i/2] = (double)((qint16)(((unsigned char)ph1Array[i] << 8) | (unsigned char)ph1Array[i + 1]));
        }

        QVector<double> ph2(size/2);
        for (int i=0; i<(size); i+=2) {
            ph2[i/2] = (double)((qint16)(((unsigned char)ph2Array[i] << 8) | (unsigned char)ph2Array[i + 1]));
        }

        QVector<double> ph3(size/2);
        for (int i=0; i<(size); i+=2) {
            ph3[i/2] = (double)((qint16)(((unsigned char)ph3Array[i] << 8) | (unsigned char)ph3Array[i + 1]));
        }

        QVector<double> vZero(size/2);
        for (int i=0; i<(size); i+=2) {
            vZero[i/2] = (double)((qint16)(((unsigned char)vZeroArray[i] << 8) | (unsigned char)vZeroArray[i + 1]));
        }

        QVector<double> position(size/2);
        for (int i=0;i < (size / 2);i++) {
            position[i] = (double)((quint8)statusArray.at(i) & 7);
        }

        QVector<double> position_hall(size/2);
        for (int i=0;i < (size / 2);i++) {
            position_hall[i] = (double)((quint8)(statusArray.at(i) >> 3) & 7) / 1.0;
        }

        QVector<double> totCurrentMc(size/2);
        for (int i=0; i<(size); i+=2) {
            totCurrentMc[i/2] = (double)((qint16)(((unsigned char)currTotArray[i] << 8) | (unsigned char)currTotArray[i + 1]));
            totCurrentMc[i/2] /= 100;
        }

        QVector<double> fSw(size/2);
        for (int i=0; i<(size); i+=2) {
            fSw[i/2] = (double)((qint16)(((unsigned char)fSwArray[i] << 8) | (unsigned char)fSwArray[i + 1]));
            fSw[i/2] *= 10.0;
            fSw[i/2] /= (double)ui->sampleIntBox->value();
        }

        // Calculate current on phases and voltages
        QVector<double> curr3(curr2.size());
        QVector<double> totCurrent(curr2.size());

        for (int i=0;i < curr2.size(); i++) {
            curr1[i] *= (3.3 / 4095.0) / (0.001 * 10.0);
            curr2[i] *= (3.3 / 4095.0) / (0.001 * 10.0);
            curr3[i] = -(curr1[i] + curr2[i]);

            const double v_fact = (3.3 / 4095.0) * ((33000.0 + 2200.0) / 2200.0);
            ph1[i] *= v_fact;
            ph2[i] *= v_fact;
            ph3[i] *= v_fact;
            vZero[i] *= v_fact;

            if (ui->truncateBox->isChecked()) {
                if (!(position[i] == 1 || position[i] == 4)) {
                    ph1[i] = 0;
                }

                if (!(position[i] == 2 || position[i] == 5)) {
                    ph2[i] = 0;
                }

                if (!(position[i] == 3 || position[i] == 6)) {
                    ph3[i] = 0;
                }
            }

            int direction = 1;
            switch ((int)position[i]) {
            case 1:
            case 6:
                if (direction) {
                    totCurrent[i] = curr3[i];
                } else {
                    totCurrent[i] = curr2[i];
                }
                break;

            case 2:
            case 3:
                totCurrent[i] = curr1[i];
                break;

            case 4:
            case 5:
                if (direction) {
                    totCurrent[i] = curr2[i];
                } else {
                    totCurrent[i] = curr3[i];
                }
                break;
            }
        }

        // Filter currents
        if (ui->currentFilterActiveBox->isChecked()) {
            curr1 = f.filterSignal(curr1, filter, ui->compDelayBox->isChecked());
            curr2 = f.filterSignal(curr2, filter, ui->compDelayBox->isChecked());
            curr3 = f.filterSignal(curr3, filter, ui->compDelayBox->isChecked());
            totCurrent = f.filterSignal(totCurrent, filter, ui->compDelayBox->isChecked());
        }

        // Apply second filter
        int decimation = 1;
        if (ui->currentFilterActiveBox2->isChecked()) {
            decimation = ui->decimationSpinBox->value();

            QVector<double> currDec1, currDec2, currDec3, totCurrentDec;
            for (int i = 0;i < curr1.size();i++) {
                if (i % decimation == 0) {
                    currDec1.append(curr1[i]);
                    currDec2.append(curr2[i]);
                    currDec3.append(curr3[i]);
                    totCurrentDec.append(totCurrent[i]);
                }
            }

            curr1 = f.filterSignal(currDec1, filter2, ui->compDelayBox->isChecked());
            curr2 = f.filterSignal(currDec2, filter2, ui->compDelayBox->isChecked());
            curr3 = f.filterSignal(currDec3, filter2, ui->compDelayBox->isChecked());
            totCurrent = f.filterSignal(totCurrentDec, filter2, ui->compDelayBox->isChecked());
        }

        static bool lastSpectrum = false;
        bool spectrumChanged = ui->currentSpectrumButton->isChecked() != lastSpectrum;
        lastSpectrum = ui->currentSpectrumButton->isChecked();

        // Filtered x-axis vector for currents
        QVector<double> xAxisCurrDec;
        QVector<double> xAxisCurr;

        // Use DFT
        // TODO: The transform only makes sense with a constant sampling frequency right now. Some
        // weird scaling should be implemented.
        if (ui->currentSpectrumButton->isChecked()) {
            int fftBits = 16;

            curr1 = f.fftWithShift(curr1, fftBits, true);
            curr2 = f.fftWithShift(curr2, fftBits, true);
            curr3 = f.fftWithShift(curr3, fftBits, true);
            totCurrent = f.fftWithShift(totCurrent, fftBits, true);
            totCurrentMc = f.fftWithShift(totCurrentMc, fftBits, true);

            curr1.resize(curr1.size() / 2);
            curr2.resize(curr2.size() / 2);
            curr3.resize(curr3.size() / 2);
            totCurrent.resize(totCurrent.size() / 2);
            totCurrentMc.resize(totCurrentMc.size() / 2);

            // Resize x-axis
            xAxisCurrDec.resize(curr1.size());
            xAxisCurr.resize(totCurrentMc.size());

            // Generate Filtered X-axis
            for (int i = 0;i < xAxisCurrDec.size();i++) {
                xAxisCurrDec[i] = ((double)i / (double)xAxisCurrDec.size()) * (f_samp / (2 * decimation));
            }

            for (int i = 0;i < xAxisCurr.size();i++) {
                xAxisCurr[i] = ((double)i / (double)xAxisCurr.size()) * (f_samp / 2);
            }
        } else {
            // Resize x-axis
            xAxisCurrDec.resize(curr1.size());
            xAxisCurr.resize(totCurrentMc.size());

            // Generate X axis
            double prev_x = 0.0;
            double rat = (double)fSw.size() / (double)xAxisCurrDec.size();
            for (int i = 0;i < xAxisCurrDec.size();i++) {
                xAxisCurrDec[i] = prev_x;
                prev_x += (double)decimation / fSw[(int)((double)i * rat)];
            }

            prev_x = 0.0;
            rat = (double)fSw.size() / (double)xAxisCurr.size();
            for (int i = 0;i < xAxisCurr.size();i++) {
                xAxisCurr[i] = prev_x;
                prev_x += 1.0 / fSw[(int)((double)i * rat)];
            }
        }

        QVector<double> xAxisVolt(ph1.size());
        double prev_x = 0.0;
        for (int i = 0;i < xAxisVolt.size();i++) {
            xAxisVolt[i] = prev_x;
            prev_x += 1.0 / fSw[i];
        }

        ui->currentPlot->clearGraphs();
        ui->voltagePlot->clearGraphs();

        QPen phasePen;
        phasePen.setStyle(Qt::DotLine);
        phasePen.setColor(Qt::blue);

        QPen phasePen2;
        phasePen2.setStyle(Qt::DotLine);
        phasePen2.setColor(Qt::red);

        int graphIndex = 0;

        if (ui->showCurrent1Box->isChecked()) {
            ui->currentPlot->addGraph();
            ui->currentPlot->graph(graphIndex)->setPen(QPen(Qt::magenta));
            ui->currentPlot->graph(graphIndex)->setData(xAxisCurrDec, curr1);
            ui->currentPlot->graph(graphIndex)->setName("Phase 1 Current");
            graphIndex++;
        }

        if (ui->showCurrent2Box->isChecked()) {
            ui->currentPlot->addGraph();
            ui->currentPlot->graph(graphIndex)->setData(xAxisCurrDec, curr2);
            ui->currentPlot->graph(graphIndex)->setPen(QPen(Qt::red));
            ui->currentPlot->graph(graphIndex)->setName("Phase 2 Current");
            graphIndex++;
        }

        if (ui->showCurrent3Box->isChecked()) {
            ui->currentPlot->addGraph();
            ui->currentPlot->graph(graphIndex)->setData(xAxisCurrDec, curr3);
            ui->currentPlot->graph(graphIndex)->setPen(QPen(Qt::green));
            ui->currentPlot->graph(graphIndex)->setName("Phase 3 Current");
            graphIndex++;
        }

        if (ui->showTotalCurrentBox->isChecked()) {
            ui->currentPlot->addGraph();
            ui->currentPlot->graph(graphIndex)->setData(xAxisCurrDec, totCurrent);
            ui->currentPlot->graph(graphIndex)->setPen(QPen(Qt::darkCyan));
            ui->currentPlot->graph(graphIndex)->setName("Total current");
            graphIndex++;
        }

        if (ui->showMcTotalCurrentBox->isChecked()) {
            ui->currentPlot->addGraph();
            ui->currentPlot->graph(graphIndex)->setData(xAxisCurr, totCurrentMc);
            ui->currentPlot->graph(graphIndex)->setPen(QPen(Qt::blue));
            ui->currentPlot->graph(graphIndex)->setName("Total current filtered by MC");
            graphIndex++;
        }

        if (ui->showPosCurrentBox->isChecked() && !ui->currentSpectrumButton->isChecked()) {
            ui->currentPlot->addGraph();
            ui->currentPlot->graph(graphIndex)->setData(xAxisCurr, position);
            ui->currentPlot->graph(graphIndex)->setPen(phasePen);
            ui->currentPlot->graph(graphIndex)->setName("Current position");
            graphIndex++;
        }

        graphIndex = 0;

        if (ui->showPh1Box->isChecked()) {
            ui->voltagePlot->addGraph();
            ui->voltagePlot->graph(graphIndex)->setData(xAxisVolt, ph1);
            ui->voltagePlot->graph(graphIndex)->setPen(QPen(Qt::blue));
            ui->voltagePlot->graph(graphIndex)->setName("Phase 1 voltage");
            graphIndex++;
        }

        if (ui->showPh2Box->isChecked()) {
            ui->voltagePlot->addGraph();
            ui->voltagePlot->graph(graphIndex)->setData(xAxisVolt, ph2);
            ui->voltagePlot->graph(graphIndex)->setPen(QPen(Qt::red));
            ui->voltagePlot->graph(graphIndex)->setName("Phase 2 voltage");
            graphIndex++;
        }

        if (ui->showPh3Box->isChecked()) {
            ui->voltagePlot->addGraph();
            ui->voltagePlot->graph(graphIndex)->setData(xAxisVolt, ph3);
            ui->voltagePlot->graph(graphIndex)->setPen(QPen(Qt::green));
            ui->voltagePlot->graph(graphIndex)->setName("Phase 3 voltage");
            graphIndex++;
        }

        if (ui->showVirtualGndBox->isChecked()) {
            ui->voltagePlot->addGraph();
            ui->voltagePlot->graph(graphIndex)->setData(xAxisVolt, vZero);
            ui->voltagePlot->graph(graphIndex)->setPen(QPen(Qt::magenta));
            ui->voltagePlot->graph(graphIndex)->setName("Virtual ground");
            graphIndex++;
        }

        if (ui->showPosVoltageBox->isChecked()) {
            ui->voltagePlot->addGraph();
            ui->voltagePlot->graph(graphIndex)->setData(xAxisVolt, position);
            ui->voltagePlot->graph(graphIndex)->setPen(phasePen);
            ui->voltagePlot->graph(graphIndex)->setName("Current position");
            graphIndex++;

            ui->voltagePlot->addGraph();
            ui->voltagePlot->graph(graphIndex)->setData(xAxisVolt, position_hall);
            ui->voltagePlot->graph(graphIndex)->setPen(phasePen2);
            ui->voltagePlot->graph(graphIndex)->setName("Hall position");
            graphIndex++;
        }

        // Plot settings
        ui->currentPlot->legend->setVisible(true);
        ui->currentPlot->legend->setFont(legendFont);
        ui->currentPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom);
        ui->currentPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
        if (ui->currentSpectrumButton->isChecked()) {
            ui->currentPlot->xAxis->setLabel("Frequency (Hz)");
            ui->currentPlot->yAxis->setLabel("Amplitude");
        } else {
            ui->currentPlot->xAxis->setLabel("Seconds (s)");
            ui->currentPlot->yAxis->setLabel("Amperes (A)");
        }

        ui->voltagePlot->legend->setVisible(true);
        ui->voltagePlot->legend->setFont(legendFont);
        ui->voltagePlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom);
        ui->voltagePlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
        ui->voltagePlot->xAxis->setLabel("Seconds (s)");
        ui->voltagePlot->yAxis->setLabel("Volts (V)");

        if (mDoRescale || spectrumChanged) {
            ui->currentPlot->rescaleAxes();
            ui->voltagePlot->rescaleAxes();
        }

        ui->currentPlot->replot();
        ui->voltagePlot->replot();

        mDoReplot = false;
        mDoRescale = false;
    }

    if (mDoReplotPos) {
        QVector<double> xAxis(positionVec.size());
        for (int i = 0;i < positionVec.size();i++) {
            xAxis[i] = (double)i;
        }

        ui->rotorPosBar->setValue((int)positionVec.last());
        ui->realtimePlotPosition->graph(0)->setData(xAxis, positionVec);
        ui->realtimePlotPosition->rescaleAxes();
        ui->realtimePlotPosition->replot();

        mDoReplotPos = false;
    }
}

void MainWindow::packetDataToSend(QByteArray &data)
{
    if (mPort->isOpen()) {
        mPort->write(data);
    }
}

void MainWindow::fwVersionReceived(int major, int minor)
{
    QPair<int, int> highest_supported = *std::max_element(mCompatibleFws.begin(), mCompatibleFws.end());
    QPair<int, int> fw_connected = qMakePair(major, minor);

    bool wasReceived = mFwVersionReceived;

    if (major < 0) {
        mFwVersionReceived = false;
        mPort->close();
        QMessageBox messageBox;
        messageBox.critical(this, "Error", "The firmware on the connected VESC is too old. Please"
                            " update it using a programmer.");
        ui->firmwareVersionLabel->setText("Old Firmware");
    } else if (fw_connected > highest_supported) {
        mFwVersionReceived = true;
        mPacketInterface->setLimitedMode(true);
        if (!wasReceived) {
            QMessageBox messageBox;
            messageBox.warning(this, "Warning", "The connected VESC has newer firmware than this version of"
                                                " BLDC Tool supports. It is recommended that you update BLDC "
                                                " Tool to the latest version. Alternatively, the firmware on"
                                                " the connected VESC can be downgraded in the firmware tab."
                                                " Until then, limited communication mode will be used where"
                                                " only the firmware can be changed.");
        }
    } else if (!mCompatibleFws.contains(fw_connected)) {
        if (fw_connected >= qMakePair(1, 1)) {
            mFwVersionReceived = true;
            mPacketInterface->setLimitedMode(true);
            if (!wasReceived) {
                QMessageBox messageBox;
                messageBox.warning(this, "Warning", "The connected VESC has too old firmware. Since the"
                                                    " connected VESC has firmware with bootloader support, it can be"
                                                    " updated from the Firmware tab."
                                                    " Until then, limited communication mode will be used where only the"
                                                    " firmware can be changed.");
            }
        } else {
            mFwVersionReceived = false;
            mPort->close();
            if (!wasReceived) {
                QMessageBox messageBox;
                messageBox.critical(this, "Error", "The firmware on the connected VESC is too old. Please"
                                                   " update it using a programmer.");
            }
        }
    } else {
        mFwVersionReceived = true;
        if (fw_connected < highest_supported) {
            if (!wasReceived) {
                QMessageBox messageBox;
                messageBox.warning(this, "Warning", "The connected VESC has compatible, but old"
                                                    " firmware. It is recommended that you update it.");
            }
        }

        QString fwStr;
        mPacketInterface->setLimitedMode(false);
        fwStr.sprintf("VESC Firmware Version %d.%d", major, minor);
        showStatusInfo(fwStr, true);
    }

    if (major >= 0) {
        QString fwText;
        fwText.sprintf("%d.%d", major, minor);
        ui->firmwareVersionLabel->setText(fwText);
    }
}

void MainWindow::ackReceived(QString ackType)
{
    showStatusInfo(ackType, true);
}

void MainWindow::mcValuesReceived(PacketInterface::MC_VALUES values)
{
    const int maxS = 500;

    ui->rtDataWidget->setValues(values);

    appendDoubleAndTrunc(&tempMos1Vec, values.temp_mos1, maxS);
    appendDoubleAndTrunc(&tempMos2Vec, values.temp_mos2, maxS);
    appendDoubleAndTrunc(&tempMos3Vec, values.temp_mos3, maxS);
    appendDoubleAndTrunc(&tempMos4Vec, values.temp_mos4, maxS);
    appendDoubleAndTrunc(&tempMos5Vec, values.temp_mos5, maxS);
    appendDoubleAndTrunc(&tempMos6Vec, values.temp_mos6, maxS);
    appendDoubleAndTrunc(&tempPcbVec, values.temp_pcb, maxS);
    appendDoubleAndTrunc(&currInVec, values.current_in, maxS);
    appendDoubleAndTrunc(&currMotorVec, values.current_motor, maxS);
    appendDoubleAndTrunc(&dutyVec, values.duty_now, maxS);
    appendDoubleAndTrunc(&rpmVec, values.rpm, maxS);
    appendDoubleAndTrunc(&voltInVec, values.v_in, maxS);

    QPen dotPen;
    dotPen.setStyle(Qt::DotLine);
    dotPen.setColor(Qt::blue);

    QFont legendFont = font();
    legendFont.setPointSize(9);

    int dataSize = tempMos1Vec.size();
    float fsamp = 10.0;

    QVector<double> xAxis(dataSize);
    for (int i = 0;i < tempMos1Vec.size();i++) {
        xAxis[i] = (double)i / fsamp;
    }

    int graphIndex = 0;

    if (!mRealtimeGraphsAdded) {
        ui->realtimePlotTemperature->addGraph();
        ui->realtimePlotTemperature->graph(graphIndex)->setPen(QPen(Qt::blue));
        ui->realtimePlotTemperature->graph(graphIndex)->setName("Temperature MOSFET 1");
        graphIndex++;

        ui->realtimePlotTemperature->addGraph();
        dotPen.setColor(Qt::blue);
        ui->realtimePlotTemperature->graph(graphIndex)->setPen(dotPen);
        ui->realtimePlotTemperature->graph(graphIndex)->setName("Temperature MOSFET 2");
        graphIndex++;

        ui->realtimePlotTemperature->addGraph();
        ui->realtimePlotTemperature->graph(graphIndex)->setPen(QPen(Qt::red));
        ui->realtimePlotTemperature->graph(graphIndex)->setName("Temperature MOSFET 3");
        graphIndex++;

        ui->realtimePlotTemperature->addGraph();
        dotPen.setColor(Qt::red);
        ui->realtimePlotTemperature->graph(graphIndex)->setPen(dotPen);
        ui->realtimePlotTemperature->graph(graphIndex)->setName("Temperature MOSFET 4");
        graphIndex++;

        ui->realtimePlotTemperature->addGraph();
        ui->realtimePlotTemperature->graph(graphIndex)->setPen(QPen(Qt::green));
        ui->realtimePlotTemperature->graph(graphIndex)->setName("Temperature MOSFET 5");
        graphIndex++;

        ui->realtimePlotTemperature->addGraph();
        dotPen.setColor(Qt::green);
        ui->realtimePlotTemperature->graph(graphIndex)->setPen(dotPen);
        ui->realtimePlotTemperature->graph(graphIndex)->setName("Temperature MOSFET 6");
        graphIndex++;

        ui->realtimePlotTemperature->addGraph();
        ui->realtimePlotTemperature->graph(graphIndex)->setPen(QPen(Qt::magenta));
        ui->realtimePlotTemperature->graph(graphIndex)->setName("Temperature PCB");
        graphIndex++;

        // Next plot
        graphIndex = 0;
        ui->realtimePlotRest->addGraph();
        ui->realtimePlotRest->graph(graphIndex)->setPen(QPen(Qt::blue));
        ui->realtimePlotRest->graph(graphIndex)->setName("Current in");
        graphIndex++;

        ui->realtimePlotRest->addGraph();
        ui->realtimePlotRest->graph(graphIndex)->setPen(QPen(Qt::red));
        ui->realtimePlotRest->graph(graphIndex)->setName("Current motor");
        graphIndex++;

        ui->realtimePlotRest->addGraph(ui->realtimePlotRest->xAxis, ui->realtimePlotRest->yAxis2);
        ui->realtimePlotRest->graph(graphIndex)->setPen(QPen(Qt::green));
        ui->realtimePlotRest->graph(graphIndex)->setName("Duty cycle");
        graphIndex++;

        ui->realtimePlotTemperature->legend->setVisible(true);
        ui->realtimePlotTemperature->legend->setFont(legendFont);
        ui->realtimePlotTemperature->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom);
        ui->realtimePlotTemperature->legend->setBrush(QBrush(QColor(255,255,255,230)));
        ui->realtimePlotTemperature->xAxis->setLabel("Seconds (s)");
        ui->realtimePlotTemperature->yAxis->setLabel("Deg C");

        ui->realtimePlotRest->legend->setVisible(true);
        ui->realtimePlotRest->legend->setFont(legendFont);
        ui->realtimePlotRest->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom);
        ui->realtimePlotRest->legend->setBrush(QBrush(QColor(255,255,255,230)));
        ui->realtimePlotRest->xAxis->setLabel("Seconds (s)");
        ui->realtimePlotRest->yAxis->setLabel("Ampere (A)");
        ui->realtimePlotRest->yAxis2->setLabel("Duty Cycle");

        ui->realtimePlotTemperature->yAxis->setRange(0, 120);
        ui->realtimePlotRest->yAxis->setRange(-20, 130);
        ui->realtimePlotRest->yAxis2->setRange(-0.2, 1.3);
        ui->realtimePlotRest->yAxis2->setVisible(true);

        mRealtimeGraphsAdded = true;
    }

    // Temperature plot
    graphIndex = 0;
    ui->realtimePlotTemperature->graph(graphIndex++)->setData(xAxis, tempMos1Vec);
    ui->realtimePlotTemperature->graph(graphIndex++)->setData(xAxis, tempMos2Vec);
    ui->realtimePlotTemperature->graph(graphIndex++)->setData(xAxis, tempMos3Vec);
    ui->realtimePlotTemperature->graph(graphIndex++)->setData(xAxis, tempMos4Vec);
    ui->realtimePlotTemperature->graph(graphIndex++)->setData(xAxis, tempMos5Vec);
    ui->realtimePlotTemperature->graph(graphIndex++)->setData(xAxis, tempMos6Vec);
    ui->realtimePlotTemperature->graph(graphIndex++)->setData(xAxis, tempPcbVec);

    // Current and duty-plot
    graphIndex = 0;
    ui->realtimePlotRest->graph(graphIndex++)->setData(xAxis, currInVec);
    ui->realtimePlotRest->graph(graphIndex++)->setData(xAxis, currMotorVec);
    ui->realtimePlotRest->graph(graphIndex++)->setData(xAxis, dutyVec);

    if (ui->realtimeAutoScaleBox->isChecked()) {
        ui->realtimePlotTemperature->rescaleAxes();
        ui->realtimePlotRest->rescaleAxes();
    }

    if (dataSize < maxS) {
        ui->realtimePlotTemperature->xAxis->setRange(0, dataSize / fsamp);
    }

    if (dataSize < maxS) {
        ui->realtimePlotRest->xAxis->setRange(0, dataSize / fsamp);
    }

    ui->realtimePlotTemperature->replot();
    ui->realtimePlotRest->replot();
}

void MainWindow::printReceived(QString str)
{
    ui->terminalBrowser->append(str);
}

void MainWindow::samplesReceived(QByteArray data)
{
    for (int i = 0;i < data.size();i++) {
        if (mSampleInt == 0 || mSampleInt == 1) {
            tmpCurr1Array.append(data[i]);
        } else if (mSampleInt == 2 || mSampleInt == 3) {
            tmpCurr2Array.append(data[i]);
        } else if (mSampleInt == 4 || mSampleInt == 5) {
            tmpPh1Array.append(data[i]);
        } else if (mSampleInt == 6 || mSampleInt == 7) {
            tmpPh2Array.append(data[i]);
        } else if (mSampleInt == 8 || mSampleInt == 9) {
            tmpPh3Array.append(data[i]);
        } else if (mSampleInt == 10 || mSampleInt == 11) {
            tmpVZeroArray.append(data[i]);
        } else if (mSampleInt == 12) {
            tmpStatusArray.append(data[i]);
        } else if (mSampleInt == 13 || mSampleInt == 14) {
            tmpCurrTotArray.append(data[i]);
        } else if (mSampleInt == 15 || mSampleInt == 16) {
            tmpFSwArray.append(data[i]);
        }

        mSampleInt++;
        if (mSampleInt == 17) {
            mSampleInt = 0;
        }

        if (tmpCurr1Array.size() == (ui->sampleNumBox->value() * 2)) {
            curr1Array = tmpCurr1Array;
            curr2Array = tmpCurr2Array;
            ph1Array = tmpPh1Array;
            ph2Array = tmpPh2Array;
            ph3Array = tmpPh3Array;
            vZeroArray = tmpVZeroArray;
            statusArray = tmpStatusArray;
            currTotArray = tmpCurrTotArray;
            fSwArray = tmpFSwArray;
            mDoReplot = true;
            mDoFilterReplot = true;
            mDoRescale = true;
        }
    }
}

void MainWindow::rotorPosReceived(double pos)
{
    appendDoubleAndTrunc(&positionVec, pos, 1500);
    mDoReplotPos = true;
}

void MainWindow::experimentSamplesReceived(QVector<double> samples)
{
    QString row;

    if (samples.size() > 0) {
        row.append(QString().sprintf("%d", (int)samples.at(0)));
    }
    for (int i = 1; i < samples.size();i++) {
        row.append(QString().sprintf("\t%.3f", samples.at(i)));
    }

    ui->experimentBrowser->append(row);
    mExperimentSamples.append(samples);
    ui->experimentSampleLabel->setText(QString().sprintf("Samples: %d", mExperimentSamples.size()));

    if (ui->experimentAutosaveBox->isChecked() &&
            (mExperimentSamples.size() % ui->experimentAutosaveIntervalBox->value()) == 0) {
        saveExperimentSamplesToFile(ui->experimentPathEdit->text());
    }
}

void MainWindow::mcconfReceived(PacketInterface::mc_configuration mcconf)
{
    setMcconfGui(mcconf);
    showStatusInfo("MCCONF Received", true);
}

void MainWindow::motorParamReceived(double cycle_int_limit, double bemf_coupling_k, QVector<int> hall_table, int hall_res)
{
    if (cycle_int_limit < 0.01 && bemf_coupling_k < 0.01) {
        showStatusInfo("Bad Detection Result Received", false);
        ui->mcconfDetectResultBrowser->setText("Detection failed.");
    } else {
        showStatusInfo("Detection Result Received", true);

        QString hall_str;
        if (hall_res == 0) {
            hall_str.sprintf("Detected hall sensor table:\n"
                             "%i, %i, %i, %i, %i, %i, %i, %i\n",
                             hall_table.at(0), hall_table.at(1),
                             hall_table.at(2), hall_table.at(3),
                             hall_table.at(4), hall_table.at(5),
                             hall_table.at(6), hall_table.at(7));
        } else if (hall_res == -1) {
            hall_str.sprintf("Hall sensor detection failed:\n"
                             "%i, %i, %i, %i, %i, %i, %i, %i\n",
                             hall_table.at(0), hall_table.at(1),
                             hall_table.at(2), hall_table.at(3),
                             hall_table.at(4), hall_table.at(5),
                             hall_table.at(6), hall_table.at(7));
        } else if (hall_res == -2) {
            hall_str.sprintf("WS2811 enabled. Hall sensors cannot be used.\n");
        } else if (hall_res == -3) {
            hall_str.sprintf("Encoder enabled. Hall sensors cannot be used.\n");
        } else {
            hall_str.sprintf("Unknown hall error: %d\n", hall_res);
        }

        ui->mcconfDetectResultBrowser->setText(QString().sprintf("Detection results:\n"
                                                                 "Integrator limit: %.2f\n"
                                                                 "BEMF Coupling: %.2f\n\n"
                                                                 "%s",
                                                                 cycle_int_limit,
                                                                 bemf_coupling_k,
                                                                 hall_str.toLocal8Bit().data()));
    }
}

void MainWindow::appconfReceived(PacketInterface::app_configuration appconf)
{
    ui->appconfControllerIdBox->setValue(appconf.controller_id);
    ui->appconfTimeoutBox->setValue(appconf.timeout_msec);
    ui->appconfTimeoutBrakeCurrentBox->setValue(appconf.timeout_brake_current);
    ui->appconfSendCanStatusBox->setChecked(appconf.send_can_status);
    ui->appconfSendCanStatusRateBox->setValue(appconf.send_can_status_rate_hz);

    switch (appconf.app_to_use) {
    case PacketInterface::APP_NONE:
        ui->appconfUseNoAppButton->setChecked(true);
        break;

    case PacketInterface::APP_PPM:
        ui->appconfUsePpmButton->setChecked(true);
        break;

    case PacketInterface::APP_ADC:
        ui->appconfUseAdcButton->setChecked(true);
        break;

    case PacketInterface::APP_UART:
        ui->appconfUseUartButton->setChecked(true);
        break;

    case PacketInterface::APP_PPM_UART:
        ui->appconfUsePpmUartButton->setChecked(true);
        break;

    case PacketInterface::APP_ADC_UART:
        ui->appconfUseAdcUartButton->setChecked(true);
        break;

    case PacketInterface::APP_NUNCHUK:
        ui->appconfUseNunchukButton->setChecked(true);
        break;

    case PacketInterface::APP_NRF:
        ui->appconfUseNrfButton->setChecked(true);
        break;

    case PacketInterface::APP_CUSTOM:
        ui->appconfUseCustomButton->setChecked(true);
        break;

    default:
        break;
    }

    // PPM
    switch (appconf.app_ppm_conf.ctrl_type) {
    case PacketInterface::PPM_CTRL_TYPE_NONE:
        ui->appconfPpmDisabledButton->setChecked(true);
        break;

    case PacketInterface::PPM_CTRL_TYPE_CURRENT:
        ui->appconfPpmCurrentButton->setChecked(true);
        break;

    case PacketInterface::PPM_CTRL_TYPE_CURRENT_NOREV:
        ui->appconfPpmCurrentNorevButton->setChecked(true);
        break;

    case PacketInterface::PPM_CTRL_TYPE_CURRENT_NOREV_BRAKE:
        ui->appconfPpmCurrentNorevBrakeButton->setChecked(true);
        break;

    case PacketInterface::PPM_CTRL_TYPE_DUTY:
        ui->appconfPpmDutyButton->setChecked(true);
        break;

    case PacketInterface::PPM_CTRL_TYPE_DUTY_NOREV:
        ui->appconfPpmDutyNorevButton->setChecked(true);
        break;

    case PacketInterface::PPM_CTRL_TYPE_PID:
        ui->appconfPpmPidButton->setChecked(true);
        break;

    case PacketInterface::PPM_CTRL_TYPE_PID_NOREV:
        ui->appconfPpmPidNorevButton->setChecked(true);
        break;

    default:
        break;
    }

    ui->appconfPpmPidMaxErpmBox->setValue(appconf.app_ppm_conf.pid_max_erpm);
    ui->appconfPpmHystBox->setValue(appconf.app_ppm_conf.hyst);
    ui->appconfPpmPulseStartBox->setValue(appconf.app_ppm_conf.pulse_start);
    ui->appconfPpmPulseWidthBox->setValue(appconf.app_ppm_conf.pulse_end);
    ui->appconfPpmMedianFilterBox->setChecked(appconf.app_ppm_conf.median_filter);
    ui->appconfPpmSafeStartBox->setChecked(appconf.app_ppm_conf.safe_start);

    if (appconf.app_ppm_conf.rpm_lim_end >= 200000.0) {
        ui->appconfPpmRpmLimBox->setChecked(false);
    } else {
        ui->appconfPpmRpmLimStartBox->setValue(appconf.app_ppm_conf.rpm_lim_start);
        ui->appconfPpmRpmLimEndBox->setValue(appconf.app_ppm_conf.rpm_lim_end);
    }

    ui->appconfPpmMultiGroup->setChecked(appconf.app_ppm_conf.multi_esc);
    ui->appconfPpmTcBox->setChecked(appconf.app_ppm_conf.tc);
    ui->appconfPpmTcErpmBox->setValue(appconf.app_ppm_conf.tc_max_diff);

    // ADC
    switch (appconf.app_adc_conf.ctrl_type) {
    case PacketInterface::ADC_CTRL_TYPE_NONE:
        ui->appconfAdcDisabledButton->setChecked(true);
        break;

    case PacketInterface::ADC_CTRL_TYPE_CURRENT:
        ui->appconfAdcCurrentButton->setChecked(true);
        break;

    case PacketInterface::ADC_CTRL_TYPE_CURRENT_REV_CENTER:
        ui->appconfAdcCurrentCenterButton->setChecked(true);
        break;

    case PacketInterface::ADC_CTRL_TYPE_CURRENT_REV_BUTTON:
        ui->appconfAdcCurrentButtonButton->setChecked(true);
        break;

    case PacketInterface::ADC_CTRL_TYPE_CURRENT_NOREV_BRAKE_CENTER:
        ui->appconfAdcCurrentNorevCenterButton->setChecked(true);
        break;

    case PacketInterface::ADC_CTRL_TYPE_CURRENT_NOREV_BRAKE_BUTTON:
        ui->appconfAdcCurrentNorevButtonButton->setChecked(true);
        break;

    case PacketInterface::ADC_CTRL_TYPE_DUTY:
        ui->appconfAdcDutyCycleButton->setChecked(true);
        break;

    case PacketInterface::ADC_CTRL_TYPE_DUTY_REV_CENTER:
        ui->appconfAdcDutyCycleCenterButton->setChecked(true);
        break;

    case PacketInterface::ADC_CTRL_TYPE_DUTY_REV_BUTTON:
        ui->appconfAdcDutyCycleButtonButton->setChecked(true);
        break;

    default:
        break;
    }

    ui->appconfAdcUpdateRateBox->setValue(appconf.app_adc_conf.update_rate_hz);
    ui->appconfAdcHystBox->setValue(appconf.app_adc_conf.hyst);
    ui->appconfAdcVoltageStartBox->setValue(appconf.app_adc_conf.voltage_start);
    ui->appconfAdcVoltageEndBox->setValue(appconf.app_adc_conf.voltage_end);
    ui->appconfAdcFilterBox->setChecked(appconf.app_adc_conf.use_filter);
    ui->appconfAdcSafeStartBox->setChecked(appconf.app_adc_conf.safe_start);
    ui->appconfAdcInvertButtonBox->setChecked(appconf.app_adc_conf.button_inverted);
    ui->appconfAdcInvertVoltageBox->setChecked(appconf.app_adc_conf.voltage_inverted);

    if (appconf.app_adc_conf.rpm_lim_end >= 200000.0) {
        ui->appconfAdcRpmLimBox->setChecked(false);
    } else {
        ui->appconfAdcRpmLimStartBox->setValue(appconf.app_adc_conf.rpm_lim_start);
        ui->appconfAdcRpmLimEndBox->setValue(appconf.app_adc_conf.rpm_lim_end);
    }

    ui->appconfAdcMultiGroup->setChecked(appconf.app_adc_conf.multi_esc);
    ui->appconfAdcTcBox->setChecked(appconf.app_adc_conf.tc);
    ui->appconfAdcTcErpmBox->setValue(appconf.app_adc_conf.tc_max_diff);

    // UART
    ui->appconfUartBaudBox->setValue(appconf.app_uart_baudrate);

    // Nunchuk
    switch (appconf.app_chuk_conf.ctrl_type) {
    case PacketInterface::CHUK_CTRL_TYPE_NONE:
        ui->appconfChukDisabledButton->setChecked(true);
        break;

    case PacketInterface::CHUK_CTRL_TYPE_CURRENT:
        ui->appconfChukCurrentButton->setChecked(true);
        break;

    case PacketInterface::CHUK_CTRL_TYPE_CURRENT_NOREV:
        ui->appconfChukCurrentNorevButton->setChecked(true);
        break;

    default:
        break;
    }

    ui->appconfChukHystBox->setValue(appconf.app_chuk_conf.hyst);
    ui->appconfChukRpmLimStartBox->setValue(appconf.app_chuk_conf.rpm_lim_start);
    ui->appconfChukRpmLimEndBox->setValue(appconf.app_chuk_conf.rpm_lim_end);
    ui->appconfChukRampTimePosBox->setValue(appconf.app_chuk_conf.ramp_time_pos);
    ui->appconfChukRampTimeNegBox->setValue(appconf.app_chuk_conf.ramp_time_neg);

    ui->appconfChukMultiGroup->setChecked(appconf.app_chuk_conf.multi_esc);
    ui->appconfChukTcBox->setChecked(appconf.app_chuk_conf.tc);
    ui->appconfChukTcErpmBox->setValue(appconf.app_chuk_conf.tc_max_diff);

    mAppconfLoaded = true;
    showStatusInfo("APPCONF Received", true);
}

void MainWindow::decodedPpmReceived(double ppm_value, double ppm_last_len)
{
    ui->appconfDecodedPpmBar->setValue((ppm_value + 1.0) * 500.0);
    ui->appconfPpmPulsewidthNumber->display(ppm_last_len);
}

void MainWindow::decodedAdcReceived(double adc_value, double adc_voltage)
{
    ui->appconfAdcDecodedBar->setValue(adc_value * 1000.0);
    ui->appconfAdcVoltageNumber->display(adc_voltage);
}

void MainWindow::decodedChukReceived(double chuk_value)
{
    ui->appconfDecodedChukBar->setValue((chuk_value + 1.0) * 500.0);
}

void MainWindow::on_connectButton_clicked()
{
    if(mPort->isOpen()) {
        return;
    }

    mPort->setPortName(ui->serialDeviceEdit->text().trimmed());
    mPort->open(QIODevice::ReadWrite);

    if(!mPort->isOpen()) {
        return;
    }

    mPort->setBaudRate(QSerialPort::Baud115200);
    mPort->setDataBits(QSerialPort::Data8);   //8 bits
    mPort->setParity(QSerialPort::NoParity);   //no parity
    mPort->setStopBits(QSerialPort::OneStop);   //1 stop bit
    mPort->setFlowControl(QSerialPort::NoFlowControl);  //no flow control
}

void MainWindow::on_disconnectButton_clicked()
{
    mPort->close();
}

void MainWindow::on_getDataButton_clicked()
{
    clearBuffers();
    mPacketInterface->samplePrint(false, ui->sampleNumBox->value(), ui->sampleIntBox->value());
}

void MainWindow::on_getDataStartButton_clicked()
{
    clearBuffers();
    mPacketInterface->samplePrint(true, ui->sampleNumBox->value(), ui->sampleIntBox->value());
}

void MainWindow::on_dutyButton_clicked()
{
    mPacketInterface->setDutyCycle(ui->dutyBox->value());
}

void MainWindow::on_rpmButton_clicked()
{
    mPacketInterface->setRpm(ui->rpmBox->value());
}

void MainWindow::on_currentButton_clicked()
{
    mPacketInterface->setCurrent(ui->currentBox->value());
}

void MainWindow::on_offButton_clicked()
{
    mPacketInterface->setCurrent(0.0);
}

void MainWindow::on_offBrakeButton_clicked()
{
    mPacketInterface->setDutyCycle(0.0);
}

void MainWindow::clearBuffers()
{
    mSampleInt = 0;
    tmpCurr1Array.clear();
    tmpCurr2Array.clear();
    tmpPh1Array.clear();
    tmpPh2Array.clear();
    tmpPh3Array.clear();
    tmpVZeroArray.clear();
    tmpStatusArray.clear();
    tmpCurrTotArray.clear();
    tmpFSwArray.clear();
}

void MainWindow::saveExperimentSamplesToFile(QString path)
{
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not open" << path;
        return;
    }

    QTextStream out(&file);

    QVectorIterator<QVector<double> > i(mExperimentSamples);
    while (i.hasNext()) {
        const QVector<double> &element = i.next();
        QString row;

        if (element.size() > 0) {
            row.append(QString().sprintf("%d", (int)element.at(0)));
        }
        for (int i = 1; i < element.size();i++) {
            row.append(QString().sprintf(";%.3f", element.at(i)));
        }

        out << row << "\n";
    }

    file.close();
}

void MainWindow::on_replotButton_clicked()
{
    mDoReplot = true;
    mDoFilterReplot = true;
}

void MainWindow::on_rescaleButton_clicked()
{
    ui->currentPlot->rescaleAxes();
    ui->currentPlot->replot();

    ui->voltagePlot->rescaleAxes();
    ui->voltagePlot->replot();

    ui->filterPlot->rescaleAxes();
    ui->filterPlot->replot();

    ui->filterResponsePlot->rescaleAxes();
    ui->filterResponsePlot->replot();

    ui->filterResponsePlot2->rescaleAxes();
    ui->filterResponsePlot2->replot();

    ui->realtimePlotTemperature->rescaleAxes();
    ui->realtimePlotTemperature->replot();

    ui->realtimePlotRest->rescaleAxes();
    ui->realtimePlotRest->replot();

    ui->realtimePlotPosition->rescaleAxes();
    ui->realtimePlotPosition->replot();
}

void MainWindow::on_horizontalZoomBox_clicked()
{
    Qt::Orientations plotOrientations = (Qt::Orientations)
            ((ui->horizontalZoomBox->isChecked() ? Qt::Horizontal : 0) |
            (ui->verticalZoomBox->isChecked() ? Qt::Vertical : 0));

    ui->currentPlot->axisRect()->setRangeZoom(plotOrientations);
    ui->voltagePlot->axisRect()->setRangeZoom(plotOrientations);
    ui->filterPlot->axisRect()->setRangeZoom(plotOrientations);
    ui->filterResponsePlot->axisRect()->setRangeZoom(plotOrientations);
    ui->filterPlot2->axisRect()->setRangeZoom(plotOrientations);
    ui->filterResponsePlot2->axisRect()->setRangeZoom(plotOrientations);
    ui->realtimePlotTemperature->axisRect()->setRangeZoom(plotOrientations);
    ui->realtimePlotRest->axisRect()->setRangeZoom(plotOrientations);
    ui->realtimePlotPosition->axisRect()->setRangeZoom(plotOrientations);
}

void MainWindow::on_verticalZoomBox_clicked()
{
    on_horizontalZoomBox_clicked();
}

void MainWindow::on_filterLogScaleBox_clicked(bool checked)
{
    if (checked) {
        ui->filterResponsePlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    } else {
        ui->filterResponsePlot->yAxis->setScaleType(QCPAxis::stLinear);
    }

    ui->filterResponsePlot->rescaleAxes();
    ui->filterResponsePlot->replot();
}

void MainWindow::on_filterLogScaleBox2_clicked(bool checked)
{
    if (checked) {
        ui->filterResponsePlot2->yAxis->setScaleType(QCPAxis::stLogarithmic);
    } else {
        ui->filterResponsePlot2->yAxis->setScaleType(QCPAxis::stLinear);
    }

    ui->filterResponsePlot2->rescaleAxes();
    ui->filterResponsePlot2->replot();
}

void MainWindow::on_detectButton_clicked()
{
    mPacketInterface->setDetect();
}

void MainWindow::appendDoubleAndTrunc(QVector<double> *vec, double num, int maxSize)
{
    vec->append(num);

    if(vec->size() > maxSize) {
        vec->remove(0, vec->size() - maxSize);
    }
}

void MainWindow::on_clearTerminalButton_clicked()
{
    ui->terminalBrowser->clear();
}

void MainWindow::on_sendTerminalButton_clicked()
{
    mPacketInterface->sendTerminalCmd(ui->terminalEdit->text());
    ui->terminalEdit->clear();
}

void MainWindow::on_stopDetectButton_clicked()
{
    mPacketInterface->setCurrent(0.0);
}

void MainWindow::on_experimentClearSamplesButton_clicked()
{
    ui->experimentBrowser->clear();
    mExperimentSamples.clear();
    ui->experimentSampleLabel->setText("Samples: 0");
}

void MainWindow::on_experimentSaveSamplesButton_clicked()
{
    QString path;
    path = QFileDialog::getSaveFileName(this, tr("Choose where to save the magnetometer samples"));
    if (path.isNull()) {
        return;
    }

    ui->experimentPathEdit->setText(path);
    saveExperimentSamplesToFile(path);
}

void MainWindow::on_mcconfReadButton_clicked()
{
    mPacketInterface->getMcconf();
}

void MainWindow::on_mcconfWriteButton_clicked()
{
    if (!mMcconfLoaded) {
        QMessageBox messageBox;
        messageBox.critical(this, "Error", "The configuration should be read or loaded at least once before writing it.");
        return;
    }

    mPacketInterface->setMcconf(getMcconfGui());
}

void MainWindow::on_currentBrakeButton_clicked()
{
    mPacketInterface->setCurrentBrake(ui->currentBrakeBox->value());
}

void MainWindow::on_mcconfLoadXmlButton_clicked()
{
    PacketInterface::mc_configuration mcconf = getMcconfGui();
    if (mSerialization->readMcconfXml(mcconf, this)) {
        setMcconfGui(mcconf);
    } else {
        showStatusInfo("Loading MCCONF failed", false);
    }
}

void MainWindow::on_mcconfSaveXmlButton_clicked()
{
    PacketInterface::mc_configuration mcconf = getMcconfGui();
    mSerialization->writeMcconfXml(mcconf, this);
}

void MainWindow::on_mcconfDetectMotorParamButton_clicked()
{
    mPacketInterface->detectMotorParam(ui->mcconfDetectCurrentBox->value(),
                                       ui->mcconfDetectErpmBox->value(),
                                       ui->mcconfDetectLowDutyBox->value());
}

void MainWindow::on_appconfReadButton_clicked()
{
    mPacketInterface->getAppConf();
}

void MainWindow::on_appconfWriteButton_clicked()
{
    if (!mAppconfLoaded) {
        QMessageBox messageBox;
        messageBox.critical(this, "Error", "The configuration should be read at least once before writing it.");
        return;
    }

    PacketInterface::app_configuration appconf;

    appconf.controller_id = ui->appconfControllerIdBox->value();
    appconf.timeout_msec = ui->appconfTimeoutBox->value();
    appconf.timeout_brake_current = ui->appconfTimeoutBrakeCurrentBox->value();
    appconf.send_can_status = ui->appconfSendCanStatusBox->isChecked();
    appconf.send_can_status_rate_hz = ui->appconfSendCanStatusRateBox->value();

    if (ui->appconfUseNoAppButton->isChecked()) {
        appconf.app_to_use = PacketInterface::APP_NONE;
    } else if (ui->appconfUsePpmButton->isChecked()) {
        appconf.app_to_use = PacketInterface::APP_PPM;
    } else if (ui->appconfUseAdcButton->isChecked()) {
        appconf.app_to_use = PacketInterface::APP_ADC;
    } else if (ui->appconfUseUartButton->isChecked()) {
        appconf.app_to_use = PacketInterface::APP_UART;
    } else if (ui->appconfUsePpmUartButton->isChecked()) {
        appconf.app_to_use = PacketInterface::APP_PPM_UART;
    } else if (ui->appconfUseAdcUartButton->isChecked()) {
        appconf.app_to_use = PacketInterface::APP_ADC_UART;
    } else if (ui->appconfUseNunchukButton->isChecked()) {
        appconf.app_to_use = PacketInterface::APP_NUNCHUK;
    } else if (ui->appconfUseNrfButton->isChecked()) {
        appconf.app_to_use = PacketInterface::APP_NRF;
    } else if (ui->appconfUseCustomButton->isChecked()) {
        appconf.app_to_use = PacketInterface::APP_CUSTOM;
    }

    // PPM
    if (ui->appconfPpmDisabledButton->isChecked()) {
        appconf.app_ppm_conf.ctrl_type = PacketInterface::PPM_CTRL_TYPE_NONE;
    } else if (ui->appconfPpmCurrentButton->isChecked()) {
        appconf.app_ppm_conf.ctrl_type = PacketInterface::PPM_CTRL_TYPE_CURRENT;
    } else if (ui->appconfPpmCurrentNorevButton->isChecked()) {
        appconf.app_ppm_conf.ctrl_type = PacketInterface::PPM_CTRL_TYPE_CURRENT_NOREV;
    } else if (ui->appconfPpmCurrentNorevBrakeButton->isChecked()) {
        appconf.app_ppm_conf.ctrl_type = PacketInterface::PPM_CTRL_TYPE_CURRENT_NOREV_BRAKE;
    } else if (ui->appconfPpmDutyButton->isChecked()) {
        appconf.app_ppm_conf.ctrl_type = PacketInterface::PPM_CTRL_TYPE_DUTY;
    } else if (ui->appconfPpmDutyNorevButton->isChecked()) {
        appconf.app_ppm_conf.ctrl_type = PacketInterface::PPM_CTRL_TYPE_DUTY_NOREV;
    } else if (ui->appconfPpmPidButton->isChecked()) {
        appconf.app_ppm_conf.ctrl_type = PacketInterface::PPM_CTRL_TYPE_PID;
    } else if (ui->appconfPpmPidNorevButton->isChecked()) {
        appconf.app_ppm_conf.ctrl_type = PacketInterface::PPM_CTRL_TYPE_PID_NOREV;
    }

    appconf.app_ppm_conf.pid_max_erpm = ui->appconfPpmPidMaxErpmBox->value();
    appconf.app_ppm_conf.hyst = ui->appconfPpmHystBox->value();
    appconf.app_ppm_conf.pulse_start = ui->appconfPpmPulseStartBox->value();
    appconf.app_ppm_conf.pulse_end = ui->appconfPpmPulseWidthBox->value();
    appconf.app_ppm_conf.median_filter = ui->appconfPpmMedianFilterBox->isChecked();
    appconf.app_ppm_conf.safe_start = ui->appconfPpmSafeStartBox->isChecked();

    if (ui->appconfPpmRpmLimBox->isChecked()) {
        appconf.app_ppm_conf.rpm_lim_start = ui->appconfPpmRpmLimStartBox->value();
        appconf.app_ppm_conf.rpm_lim_end = ui->appconfPpmRpmLimEndBox->value();
    } else {
        appconf.app_ppm_conf.rpm_lim_start = 200000.0;
        appconf.app_ppm_conf.rpm_lim_end = 250000.0;
    }

    appconf.app_ppm_conf.multi_esc = ui->appconfPpmMultiGroup->isChecked();
    appconf.app_ppm_conf.tc = ui->appconfPpmTcBox->isChecked();
    appconf.app_ppm_conf.tc_max_diff = ui->appconfPpmTcErpmBox->value();

    // ADC
    if (ui->appconfAdcDisabledButton->isChecked()) {
        appconf.app_adc_conf.ctrl_type = PacketInterface::ADC_CTRL_TYPE_NONE;
    } else if (ui->appconfAdcCurrentButton->isChecked()) {
        appconf.app_adc_conf.ctrl_type = PacketInterface::ADC_CTRL_TYPE_CURRENT;
    } else if (ui->appconfAdcCurrentCenterButton->isChecked()) {
        appconf.app_adc_conf.ctrl_type = PacketInterface::ADC_CTRL_TYPE_CURRENT_REV_CENTER;
    } else if (ui->appconfAdcCurrentButtonButton->isChecked()) {
        appconf.app_adc_conf.ctrl_type = PacketInterface::ADC_CTRL_TYPE_CURRENT_REV_BUTTON;
    } else if (ui->appconfAdcCurrentNorevCenterButton->isChecked()) {
        appconf.app_adc_conf.ctrl_type = PacketInterface::ADC_CTRL_TYPE_CURRENT_NOREV_BRAKE_CENTER;
    } else if (ui->appconfAdcCurrentNorevButtonButton->isChecked()) {
        appconf.app_adc_conf.ctrl_type = PacketInterface::ADC_CTRL_TYPE_CURRENT_NOREV_BRAKE_BUTTON;
    } else if (ui->appconfAdcDutyCycleButton->isChecked()) {
        appconf.app_adc_conf.ctrl_type = PacketInterface::ADC_CTRL_TYPE_DUTY;
    } else if (ui->appconfAdcDutyCycleCenterButton->isChecked()) {
        appconf.app_adc_conf.ctrl_type = PacketInterface::ADC_CTRL_TYPE_DUTY_REV_CENTER;
    } else if (ui->appconfAdcDutyCycleButtonButton->isChecked()) {
        appconf.app_adc_conf.ctrl_type = PacketInterface::ADC_CTRL_TYPE_DUTY_REV_BUTTON;
    }

    appconf.app_adc_conf.update_rate_hz = ui->appconfAdcUpdateRateBox->value();
    appconf.app_adc_conf.hyst = ui->appconfAdcHystBox->value();
    appconf.app_adc_conf.voltage_start = ui->appconfAdcVoltageStartBox->value();
    appconf.app_adc_conf.voltage_end = ui->appconfAdcVoltageEndBox->value();
    appconf.app_adc_conf.use_filter = ui->appconfAdcFilterBox->isChecked();
    appconf.app_adc_conf.safe_start = ui->appconfAdcSafeStartBox->isChecked();
    appconf.app_adc_conf.button_inverted = ui->appconfAdcInvertButtonBox->isChecked();
    appconf.app_adc_conf.voltage_inverted = ui->appconfAdcInvertVoltageBox->isChecked();

    if (ui->appconfAdcRpmLimBox->isChecked()) {
        appconf.app_adc_conf.rpm_lim_start = ui->appconfAdcRpmLimStartBox->value();
        appconf.app_adc_conf.rpm_lim_end = ui->appconfAdcRpmLimEndBox->value();
    } else {
        appconf.app_adc_conf.rpm_lim_start = 200000.0;
        appconf.app_adc_conf.rpm_lim_end = 250000.0;
    }

    appconf.app_adc_conf.multi_esc = ui->appconfAdcMultiGroup->isChecked();
    appconf.app_adc_conf.tc = ui->appconfAdcTcBox->isChecked();
    appconf.app_adc_conf.tc_max_diff = ui->appconfAdcTcErpmBox->value();

    // UART
    appconf.app_uart_baudrate = ui->appconfUartBaudBox->value();

    // Nunchuk
    if (ui->appconfChukDisabledButton->isChecked()) {
        appconf.app_chuk_conf.ctrl_type = PacketInterface::CHUK_CTRL_TYPE_NONE;
    } else if (ui->appconfChukCurrentButton->isChecked()) {
        appconf.app_chuk_conf.ctrl_type = PacketInterface::CHUK_CTRL_TYPE_CURRENT;
    } else if (ui->appconfChukCurrentNorevButton->isChecked()) {
        appconf.app_chuk_conf.ctrl_type = PacketInterface::CHUK_CTRL_TYPE_CURRENT_NOREV;
    }

    appconf.app_chuk_conf.hyst = ui->appconfChukHystBox->value();
    appconf.app_chuk_conf.rpm_lim_start = ui->appconfChukRpmLimStartBox->value();
    appconf.app_chuk_conf.rpm_lim_end = ui->appconfChukRpmLimEndBox->value();
    appconf.app_chuk_conf.ramp_time_pos = ui->appconfChukRampTimePosBox->value();
    appconf.app_chuk_conf.ramp_time_neg = ui->appconfChukRampTimeNegBox->value();

    appconf.app_chuk_conf.multi_esc = ui->appconfChukMultiGroup->isChecked();
    appconf.app_chuk_conf.tc = ui->appconfChukTcBox->isChecked();
    appconf.app_chuk_conf.tc_max_diff = ui->appconfChukTcErpmBox->value();

    mPacketInterface->setAppConf(appconf);
}

void MainWindow::on_appconfRebootButton_clicked()
{
    mPacketInterface->reboot();
}

void MainWindow::on_posCtrlButton_clicked()
{
    mPacketInterface->setPos(ui->posCtrlBox->value());
}

void MainWindow::on_firmwareChooseButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Choose Firmware File"), ".",
                                                    tr("Binary files (*.bin)"));

    if (filename.isNull()) {
        return;
    }

    ui->firmwareEdit->setText(filename);
}

void MainWindow::on_firmwareUploadButton_clicked()
{
    QFile file(ui->firmwareEdit->text());
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox messageBox;
        messageBox.critical(this, "Error", "Could not open file. Make sure that the path is valid.");
        return;
    }

    if (file.size() > 400000) {
        QMessageBox messageBox;
        messageBox.critical(this, "Error", "The selected file is too large to be a firmware.");
        return;
    }

    QFileInfo fileInfo(file.fileName());

    if (!(fileInfo.fileName().startsWith("BLDC_4") || fileInfo.fileName().startsWith("VESC"))
            || !fileInfo.fileName().endsWith(".bin")) {
        QMessageBox messageBox;
        messageBox.critical(this, "Error", "The selected file name seems invalid.");
        return;
    }

    QByteArray fw = file.readAll();
    mPacketInterface->startFirmwareUpload(fw);
}

void MainWindow::on_firmwareVersionReadButton_clicked()
{
    mPacketInterface->getFwVersion();
}
