#include "bldcinterface.h"
#include <math.h>
#include <QThread>
#include <QtQml>

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

BLDCInterface::BLDCInterface(QObject *parent) :
    QObject(parent)
{
    m_mcconf = new McConfiguration(this);
    m_appconf = new AppConfiguration(this);


    refreshSerialDevices();
    set_udpIp("192.168.1.118");

    // Compatible firmwares
    mFwVersionReceived = false;
    mFwRetries = 0;
    mCompatibleFws.append(qMakePair(2, 15));

    QString supportedFWs;
    for (int i = 0;i < mCompatibleFws.size();i++) {
        QString tmp;
        tmp.sprintf("%d.%d", mCompatibleFws.at(i).first, mCompatibleFws.at(i).second);
        if (i < (mCompatibleFws.size() - 1)) {
            tmp.append("\n");
        }
        supportedFWs.append(tmp);
    }
    update_firmwareSupported(supportedFWs);

    mSerialPort = new QSerialPort(this);

    mTimer = new QTimer(this);
    mTimer->setInterval(20);
    mTimer->start();

    mSampleInt = 0;
    m_doReplot =(false );
    m_doRescale=( true);
    m_doFilterReplot = true;
    m_packetInterface = new PacketInterface(this);
    mDetectRes.updated = false;

    connect(mSerialPort, SIGNAL(readyRead()),
            this, SLOT(serialDataAvailable()));
    connect(mSerialPort, SIGNAL(error(QSerialPort::SerialPortError)),
            this, SLOT(serialPortError(QSerialPort::SerialPortError)));
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));

    connect(m_packetInterface, SIGNAL(dataToSend(QByteArray&)),
            this, SLOT(packetDataToSend(QByteArray&)));
    connect(m_packetInterface, SIGNAL(fwVersionReceived(int,int)),
            this, SLOT(fwVersionReceived(int,int)));
    connect(m_packetInterface, SIGNAL(ackReceived(QString)),
            this, SIGNAL(ackReceived(QString)));
    connect(m_packetInterface, SIGNAL(valuesReceived(MC_VALUES)),
            this, SIGNAL(mcValuesReceived(MC_VALUES)));
    connect(m_packetInterface, SIGNAL(printReceived(QString)),
            this, SIGNAL(printReceived(QString)));
    connect(m_packetInterface, SIGNAL(samplesReceived(QByteArray)),
            this, SLOT(samplesReceived(QByteArray)));
    connect(m_packetInterface, SIGNAL(rotorPosReceived(double)),
            this, SIGNAL(rotorPosReceived(double)));
    connect(m_packetInterface, SIGNAL(experimentSamplesReceived(QVector<double>)),
            this, SLOT(experimentSamplesReceived(QVector<double>)));
    connect(m_packetInterface, SIGNAL(mcconfReceived(mc_configuration)),
            this, SLOT(mcconfReceived(mc_configuration)));
    connect(m_packetInterface, SIGNAL(motorParamReceived(double,double,QVector<int>,int)),
            this, SLOT(motorParamReceived(double,double,QVector<int>,int)));
    connect(m_packetInterface, SIGNAL(motorRLReceived(double,double)),
            this, SLOT(motorRLReceived(double,double)));
    connect(m_packetInterface, SIGNAL(motorLinkageReceived(double)),
            this, SLOT(motorLinkageReceived(double)));
    connect(m_packetInterface, SIGNAL(encoderParamReceived(double,double,bool)),
            this, SLOT(encoderParamReceived(double,double,bool)));
    connect(m_packetInterface, SIGNAL(focHallTableReceived(QVector<int>,int)),
            this, SLOT(focHallTableReceived(QVector<int>,int)));
    connect(m_packetInterface, SIGNAL(appconfReceived(app_configuration)),
            this, SLOT(appconfReceived(app_configuration)));
    connect(m_packetInterface, SIGNAL(decodedPpmReceived(double,double)),
            this, SLOT(decodedPpmReceived(double,double)));
    connect(m_packetInterface, SIGNAL(decodedAdcReceived(double,double,double,double)),
            this, SLOT(decodedAdcReceived(double,double,double,double)));
    connect(m_packetInterface, SIGNAL(decodedChukReceived(double)),
            this, SLOT(decodedChukReceived(double)));

    mSerialization = new Serialization(this);

    qmlRegisterType<AppConfiguration>("bldc", 1, 0, "AppConf");
    qmlRegisterType<McConfiguration>("bldc", 1, 0, "McConf");


}

void BLDCInterface::serialDataAvailable()
{
    while (mSerialPort->bytesAvailable() > 0) {
        QByteArray data = mSerialPort->readAll();
        m_packetInterface->processData(data);
    }
}

void BLDCInterface::serialPortError(QSerialPort::SerialPortError error)
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
        message = "Serial port error: " + QString::number(error);
        break;
    }

    if(!message.isEmpty()) {
        emit statusInfoChanged(message, false);

        if(mSerialPort->isOpen()) {
            mSerialPort->close();
        }
    }
}

void BLDCInterface::timerSlot()
{
    // Update CAN fwd function
    m_packetInterface->setSendCan(m_canFwd, m_canId);

    // Read FW version if needed
    static bool sendCanBefore = false;
    static int canIdBefore = 0;
    if (mSerialPort->isOpen() || m_packetInterface->isUdpConnected()) {
        if (sendCanBefore != m_canFwd ||
                canIdBefore != m_canId) {
            mFwVersionReceived = false;
            mFwRetries = 0;
        }

        if (!mFwVersionReceived) {
            m_packetInterface->getFwVersion();
            mFwRetries++;

            // Timeout if the firmware cannot be read
            if (mFwRetries >= 100) {
                emit statusInfoChanged("No firmware read response", false);
                disconnectSerial();
            }
        }

    } else {
        mFwVersionReceived = false;
        mFwRetries = 0;
    }
    sendCanBefore = m_canFwd;
    canIdBefore = m_canId;

    // Update status label
    {
        if (mSerialPort->isOpen() || m_packetInterface->isUdpConnected()) {
            if (m_packetInterface->isLimitedMode()) {
                update_status("Connected, limited");
            } else {
                update_status("Connected");
            }
        } else {
            update_status("Not connected");
        }
    }

    // Update fw upload bar and label
    double fw_prog = m_packetInterface->getFirmwareUploadProgress();
    if (fw_prog > -0.1) {
        update_firmwareProgress( fw_prog );
        update_firmwareUploadEnabled(false);
        update_firmwareCancelEnabled(true);
    } else {
        // If the firmware upload just finished or failed
        if (!m_firmwareUploadEnabled) {
            mFwVersionReceived = false;
            mFwRetries = 0;
            if (m_packetInterface->getFirmwareUploadStatus().compare("FW Upload Done") == 0) {
                update_firmwareProgress(1.0);
            } else {
                update_firmwareProgress(0.0);
            }
        }
        update_firmwareUploadEnabled(true);
        update_firmwareCancelEnabled(false);

        // Send alive command once every 10 iterations (only while not uploading firmware)
        static int alive_cnt = 0;
        alive_cnt++;
        if (alive_cnt >= 10) {
            alive_cnt = 0;
            m_packetInterface->sendAlive();
        }
    }
    update_firmwareUploadStatus(m_packetInterface->getFirmwareUploadStatus());

    // Update MC readings
    if (m_realtimeActivate) {
        m_packetInterface->getValues();
    }

    // Update decoded servo value
    if (m_appconfUpdatePpm) {
        m_packetInterface->getDecodedPpm();
    }

    // Update decoded adc value
    if (m_appconfAdcUpdate) {
        m_packetInterface->getDecodedAdc();
    }

    // Update decoded nunchuk value
    if (m_appconfUpdateChuk) {
        m_packetInterface->getDecodedChuk();
    }

    // Enable/disable fields in the configuration page
    static int isSlIntBefore = true;
    if (m_mcconfCommInt != isSlIntBefore) {
        if (m_mcconfCommInt) {
            m_mcconf->set_sl_min_erpm                           (true);
            m_mcconf->set_sl_max_fullbreak_current_dir_change	(true);
            m_mcconf->set_sl_bemf_coupling_k            		(true);
            m_mcconf->set_sl_cycle_int_rpm_br           		(true);
            m_mcconf->set_sl_cycle_int_limit            		(true);
            m_mcconf->set_sl_phase_advance_at_br        		(true);
            m_mcconf->set_sl_min_erpm_cycle_int_limit   		(true);
        } else {
            m_mcconf->set_sl_min_erpm                           (true);
            m_mcconf->set_sl_max_fullbreak_current_dir_change	(true);
            m_mcconf->set_sl_bemf_coupling_k            		(false);
            m_mcconf->set_sl_cycle_int_rpm_br           		(true);
            m_mcconf->set_sl_cycle_int_limit            		(false);
            m_mcconf->set_sl_phase_advance_at_br        		(true);
            m_mcconf->set_sl_min_erpm_cycle_int_limit   		(false);
        }
    }
    isSlIntBefore = m_mcconfCommInt;

    // Handle key events
    static double keyPower = 0.0;
    static double lastKeyPower = 0.0;
    const double lowPower = 0.18;
    const double lowPowerRev = 0.1;
    const double highPower = 0.9;
    const double highPowerRev = 0.3;
    const double lowStep = 0.02;
    const double highStep = 0.01;

    if (m_keyRight && m_keyLeft) {
        if (keyPower >= lowPower) {
            stepTowards(keyPower, highPower, highStep);
        } else if (keyPower <= -lowPower) {
            stepTowards(keyPower, -highPowerRev, highStep);
        } else if (keyPower >= 0) {
            stepTowards(keyPower, highPower, lowStep);
        } else {
            stepTowards(keyPower, -highPowerRev, lowStep);
        }
    } else if (m_keyRight) {
        if (fabs(keyPower) > lowPower) {
            stepTowards(keyPower, lowPower, highStep);
        } else {
            stepTowards(keyPower, lowPower, lowStep);
        }
    } else if (m_keyLeft) {
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
        m_packetInterface->setDutyCycle(keyPower);
    }
    emit update();
}

void BLDCInterface::packetDataToSend(QByteArray &data)
{
    if (mSerialPort->isOpen()) {
        mSerialPort->write(data);
    }
}

void BLDCInterface::fwVersionReceived(int major, int minor)
{
    QPair<int, int> highest_supported = *std::max_element(mCompatibleFws.begin(), mCompatibleFws.end());
    QPair<int, int> fw_connected = qMakePair(major, minor);

    bool wasReceived = mFwVersionReceived;

    if (major < 0) {
        mFwVersionReceived = false;
        mFwRetries = 0;
        disconnectSerial();
        emit msgCritical( "Error", "The firmware on the connected VESC is too old. Please"
                            " update it using a programmer.");
        update_firmwareVersion("Old Firmware");
    } else if (fw_connected > highest_supported) {
        mFwVersionReceived = true;
        m_packetInterface->setLimitedMode(true);
        if (!wasReceived) {
            emit msgwarning("Warning", "The connected VESC has newer firmware than this version of"
                                                " BLDC Tool supports. It is recommended that you update BLDC "
                                                " Tool to the latest version. Alternatively, the firmware on"
                                                " the connected VESC can be downgraded in the firmware tab."
                                                " Until then, limited communication mode will be used where"
                                                " only the firmware can be changed.");
        }
    } else if (!mCompatibleFws.contains(fw_connected)) {
        if (fw_connected >= qMakePair(1, 1)) {
            mFwVersionReceived = true;
            m_packetInterface->setLimitedMode(true);
            if (!wasReceived) {
                emit msgwarning("Warning", "The connected VESC has too old firmware. Since the"
                                                    " connected VESC has firmware with bootloader support, it can be"
                                                    " updated from the Firmware tab."
                                                    " Until then, limited communication mode will be used where only the"
                                                    " firmware can be changed.");
            }
        } else {
            mFwVersionReceived = false;
            mFwRetries = 0;
            disconnectSerial();
            if (!wasReceived) {
                emit msgCritical( "Error", "The firmware on the connected VESC is too old. Please"
                                                   " update it using a programmer.");
            }
        }
    } else {
        mFwVersionReceived = true;
        if (fw_connected < highest_supported) {
            if (!wasReceived) {
                emit msgwarning("Warning", "The connected VESC has compatible, but old"
                                                    " firmware. It is recommended that you update it.");
            }
        }

        QString fwStr;
        m_packetInterface->setLimitedMode(false);
        fwStr.sprintf("VESC Firmware Version %d.%d", major, minor);
        emit statusInfoChanged(fwStr, true);
    }

    if (major >= 0) {
        QString fwText;
        fwText.sprintf("%d.%d", major, minor);
        update_firmwareVersion(fwText);
    }
}

void BLDCInterface::samplesReceived(QByteArray data)
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

        if (tmpCurr1Array.size() == (m_sampleNum * 2)) {
            curr1Array = tmpCurr1Array;
            curr2Array = tmpCurr2Array;
            ph1Array = tmpPh1Array;
            ph2Array = tmpPh2Array;
            ph3Array = tmpPh3Array;
            vZeroArray = tmpVZeroArray;
            statusArray = tmpStatusArray;
            currTotArray = tmpCurrTotArray;
            fSwArray = tmpFSwArray;
            set_doReplot(true);
            set_doFilterReplot(true);
            set_doRescale(true);
        }
    }
}

void BLDCInterface::mcconfReceived(mc_configuration mcconf)
{
    m_mcconf->setData(mcconf);
    m_mcconfLoaded = true;
    emit statusInfoChanged("MCCONF Received", true);
}

void BLDCInterface::motorParamReceived(double cycle_int_limit, double bemf_coupling_k, QVector<int> hall_table, int hall_res)
{
    if (cycle_int_limit < 0.01 && bemf_coupling_k < 0.01) {
        emit statusInfoChanged("Bad Detection Result Received", false);
        update_mcconfDetectResultBrowser("Detection failed.");
        return;
    }
    emit statusInfoChanged("Detection Result Received", true);

    mDetectRes.updated = true;
    mDetectRes.cycle_int_limit = cycle_int_limit;
    mDetectRes.bemf_coupling_k = bemf_coupling_k;
    mDetectRes.hall_table = hall_table;
    mDetectRes.hall_res = hall_res;

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

    update_mcconfDetectResultBrowser(QString().sprintf("Detection results:\n"
                                                             "Integrator limit: %.2f\n"
                                                             "BEMF Coupling: %.2f\n\n"
                                                             "%s",
                                                             cycle_int_limit,
                                                             bemf_coupling_k,
                                                             hall_str.toLocal8Bit().data()));
}

void BLDCInterface::motorRLReceived(double r, double l)
{
    if (r < 1e-9 && l < 1e-9) {
        emit statusInfoChanged("Bad Detection Result Received", false);
    } else {
        emit statusInfoChanged("Detection Result Received", true);
        set_mcconfFocDetectL(l);
        set_mcconfFocDetectR(r);
        mcconfFocCalcCC();
    }
}

void BLDCInterface::motorLinkageReceived(double flux_linkage)
{
    if (flux_linkage < 1e-9) {
        emit statusInfoChanged("Bad Detection Result Received", false);
    } else {
        emit statusInfoChanged("Detection Result Received", true);
        set_mcconfFocDetectLinkage(flux_linkage);
    }
}

void BLDCInterface::encoderParamReceived(double offset, double ratio, bool inverted)
{
    if (offset > 1000.0) {
        emit statusInfoChanged("Encoder not enabled in firmware", false);
        emit msgCritical("Error", "Encoder support is not enabled in the current firmware. Please \n"
                                           "upload firmware with encodcer support and try again.");
    } else {
        emit statusInfoChanged("Encoder Result Received", true);
        set_mcconfFocMeasureEncoderOffset(offset);
        set_mcconfFocMeasureEncoderRatio(ratio);
        set_mcconfFocMeasureEncoderInverted(inverted);
    }
}

void BLDCInterface::focHallTableReceived(QVector<int> hall_table, int res)
{
    if (res != 0) {
        emit statusInfoChanged("Bad Detection Result Received", false);
    } else {
        emit statusInfoChanged("Hall Result Received", true);
        QList<double> table;
        foreach (double val, hall_table)
            table.append(val);
        set_mcconfFocMeasureHallTable(table);
    }
}

void BLDCInterface::appconfReceived(app_configuration appconf){
    m_appconf->setData(appconf);
    update_appconfLoaded( true );
    emit statusInfoChanged("APPCONF Received", true);
}

void BLDCInterface::decodedPpmReceived(double ppm_value, double ppm_last_len)
{
    update_appconfDecodedPpm((ppm_value + 1.0) * 5.0);
    update_appconfPpmPulsewidth(ppm_last_len);
}

void BLDCInterface::decodedAdcReceived(double adc_value, double adc_voltage, double adc_value2, double adc_voltage2)
{
    update_appconfAdcDecoded(adc_value);
    update_appconfAdcVoltage(adc_voltage);

    update_appconfAdcDecoded2(adc_value2);
    update_appconfAdcVoltage2(adc_voltage2);
}

void BLDCInterface::decodedChukReceived(double chuk_value)
{
    update_appconfDecodedChuk((chuk_value + 1.0) * 0.5);
}

void BLDCInterface::mcconfFocCalcCC()
{
    double r = get_mcconfFocDetectR();
    double l = get_mcconfFocDetectL();

    if (r < 1e-10) {
        emit msgCritical("Error", "R is 0. Please measure it first.");
        return;
    }

    if (l < 1e-10) {
        emit msgCritical( "Error", "L is 0. Please measure it first.");
        return;
    }

    l /= 1e6;
    double tc = m_mcconfFocCalcCCTc;
    double bw = 1.0 / (tc * 1e-6);
    double kp = l * bw;
    double ki = kp * (r / l);

    set_mcconfFocCalcKp(kp);
    set_mcconfFocCalcKi(ki);
}

void BLDCInterface::experimentSamplesReceived(QVector<double> samples)
{
    emit experimentSamplesReceived(samples.toList());
}

void BLDCInterface::refreshSerialDevices()
{
    m_serialPortList.clear();

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    foreach(const QSerialPortInfo &port, ports) {
        QString name = port.portName();
        int index = m_serialPortList.count();
        // put STMicroelectronics device first in list and add prefix
        if(port.manufacturer() == "STMicroelectronics") {
            name.insert(0, "VESC - ");
            index = 0;
        }
        m_serialPortList.insert(index, new SerialPort(name, port.systemLocation()));
    }
    set_currentSerialPort(0);
}

void BLDCInterface::clearBuffers()
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

void BLDCInterface::disconnectSerial(){
    if (mSerialPort->isOpen()) {
        mSerialPort->close();
    }

    if (m_packetInterface->isUdpConnected()) {
        m_packetInterface->stopUdpConnection();
    }

    mFwVersionReceived = false;
    mFwRetries = 0;
}

void BLDCInterface::detect()
{
    m_packetInterface->setDetect(DISP_POS_MODE_INDUCTANCE);
}

void BLDCInterface::stopDetect()
{
    m_packetInterface->setDetect(DISP_POS_MODE_NONE);
}

void BLDCInterface::sendTerminal(QString &cmd)
{
    m_packetInterface->sendTerminalCmd(cmd);
}

void BLDCInterface::readMcconf()
{
    m_packetInterface->getMcconf();
}

void BLDCInterface::readMcconfDefault()
{
    m_packetInterface->getMcconfDefault();
}

void BLDCInterface::writeMcconf()
{
    if (!m_mcconfLoaded) {
        emit msgCritical("Error", "The configuration should be read or loaded at least once before writing it.");
        return;
    }
    m_packetInterface->setMcconf(m_mcconf->data());
}

void BLDCInterface::loadMcconfXml()
{
    mc_configuration tmp = m_mcconf->data();
    if (mSerialization->readMcconfXml(tmp)) {
        m_mcconf->setData(tmp);
    } else {
        emit statusInfoChanged("Loading MCCONF failed", false);
    }
}

void BLDCInterface::saveMcconfXml(){
    mSerialization->writeMcconfXml(m_mcconf->data());
}

void BLDCInterface::detectMotorParam(double current, double min_rpm, double low_duty){
    m_packetInterface->detectMotorParam(current, min_rpm, low_duty);
}

void BLDCInterface::readAppConf(){
    m_packetInterface->getAppConf();
}

void BLDCInterface::readAppConfDefault(){
    m_packetInterface->getAppConfDefault();
}

void BLDCInterface::writeAppConf(){
    if (!m_appconfLoaded) {
        emit msgCritical( "Error", "The configuration should be read at least once before writing it.");
        return;
    }
    m_packetInterface->setAppConf(m_appconf->data());
}

void BLDCInterface::reboot(){
    m_packetInterface->reboot();
}

void BLDCInterface::setPos(double pos){
    m_packetInterface->setPos(pos);
}

void BLDCInterface::uploadFirmware(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        emit msgCritical("Error", "Could not open file. Make sure that the path is valid.");
        return;
    }

    if (file.size() > 400000) {
        emit msgCritical("Error", "The selected file is too large to be a firmware.");
        return;
    }

    QFileInfo fileInfo(file.fileName());

    if (!(fileInfo.fileName().startsWith("BLDC_4") || fileInfo.fileName().startsWith("VESC"))
            || !fileInfo.fileName().endsWith(".bin")) {
        emit msgCritical("Error", "The selected file name seems invalid.");
        return;
    }

    QByteArray fw = file.readAll();
    m_packetInterface->startFirmwareUpload(fw);
}

void BLDCInterface::readFirmwareVersion()
{
    mFwVersionReceived = false;
    mFwRetries = 0;
    m_packetInterface->getFwVersion();
}

void BLDCInterface::detectEncoder()
{
    m_packetInterface->setDetect(DISP_POS_MODE_ENCODER);
}

void BLDCInterface::detectEncoderPosError()
{
    m_packetInterface->setDetect(DISP_POS_MODE_ENCODER_POS_ERROR);
}

void BLDCInterface::detectEncoderObserverError()
{
    m_packetInterface->setDetect(DISP_POS_MODE_ENCODER_OBSERVER_ERROR);
}

void BLDCInterface::detectObserver()
{
    m_packetInterface->setDetect(DISP_POS_MODE_OBSERVER);
}


void BLDCInterface::getSampleData(bool atStart, int sampleNum, int sampleInt)
{
    clearBuffers();
    m_packetInterface->samplePrint(atStart, sampleNum, sampleInt);
}

void BLDCInterface::connectCurrentSerial()
{
    QString serialPort = m_serialPortList.at(m_currentSerialPort)->get_systemLocation();
    connectSerial(serialPort);
}

void BLDCInterface::connectSerial(QString port)
{
    if(mSerialPort->isOpen()) {
        return;
    }
    mSerialPort->setPortName(port);
    mSerialPort->open(QIODevice::ReadWrite);

    if(!mSerialPort->isOpen()) {
        return;
    }

    mSerialPort->setBaudRate(QSerialPort::Baud115200);
    mSerialPort->setDataBits(QSerialPort::Data8);
    mSerialPort->setParity(QSerialPort::NoParity);
    mSerialPort->setStopBits(QSerialPort::OneStop);
    mSerialPort->setFlowControl(QSerialPort::NoFlowControl);

    // For nrf
    mSerialPort->setRequestToSend(true);
    mSerialPort->setDataTerminalReady(true);
    QThread::msleep(5);
    mSerialPort->setDataTerminalReady(false);
    QThread::msleep(100);

    m_packetInterface->stopUdpConnection();

}
