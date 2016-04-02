#include "bldcinterface.h"
#include <math.h>
#include <QThread>
#ifdef QML
#include <QtQml>
#endif
void BLDCInterface::stepTowards(double &value, double goal, double step) {
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


BLDCInterface::BLDCInterface(QObject *parent) :
    QObject(parent),
    #if  defined(Q_OS_WIN)
        m_os(OS::Windows)
    #elif defined(Q_OS_ANDROID)
        m_os(OS::Android)
    #elif defined(Q_OS_LINUX)
        m_os(OS::Linux)
    #elif defined(Q_OS_OSX)
        m_os(OS::OSX)
    #elif defined(Q_OS_IOS)
        m_os(OS::IOS)
    #else
        m_os(OS::Unkown)
    #endif
    ,m_udpPort(27800)
    ,m_realtimeActivate(false)
    ,m_keyLeft(false)
    ,m_keyRight(false)
    ,m_detectRes()
{
    m_mcconf = new McConfiguration(this);
    m_appconf = new AppConfiguration(this);
    m_mcValues = new McValues(this);
    m_bleInterface = new BLEInterface(this);

#ifndef NO_SERIAL_PORT
    mSerialPort = new QSerialPort(this);
    refreshSerialDevices();
    connect(mSerialPort, SIGNAL(readyRead()),
            this, SLOT(serialDataAvailable()));
    connect(mSerialPort, SIGNAL(error(QSerialPort::SerialPortError)),
            this, SLOT(serialPortError(QSerialPort::SerialPortError)));
#endif

    // Compatible firmwares
    mFwVersionReceived = false;
    mFwRetries = 0;
    mCompatibleFws.append(qMakePair(2, 16));

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
    mTimer = new QTimer(this);
    mTimer->setInterval(TIMER_INTERVAL_UDB);
    mTimer->start();
    m_packetInterface = new PacketInterface(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    connect(m_packetInterface, SIGNAL(dataToSend(QByteArray&)),
            this, SLOT(packetDataToSend(QByteArray&)));
    connect(m_packetInterface, SIGNAL(fwVersionReceived(int,int)),
            this, SLOT(fwVersionReceived(int,int)));
    connect(m_packetInterface, SIGNAL(valuesReceived(MC_VALUES)),
            m_mcValues, SLOT(setValues(MC_VALUES)));
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
    connect(m_bleInterface, SIGNAL(statusInfoChanged(QString,bool)),
            this, SIGNAL(statusInfoChanged(QString,bool)));
    connect(m_bleInterface, SIGNAL(dataReceived(QByteArray)),
            m_packetInterface, SLOT(processData(QByteArray)));

    mSerialization = new Serialization(this);
#ifdef QML
    qmlRegisterType<AppConfiguration>("bldc", 1, 0, "AppConf");
    qmlRegisterType<McConfiguration>("bldc", 1, 0, "McConf");
    qmlRegisterType<McValues>("bldc", 1, 0, "McValues");
    qmlRegisterType<PacketInterface>();
    qmlRegisterType<BLEInterface>();
    qmlRegisterUncreatableType<OS>("bldc", 1, 0, "OS", "Read Only" );
#endif
}

void BLDCInterface::timerSlot()
{
    // Update CAN fwd function
    m_packetInterface->setSendCan(m_canFwd, m_canId);

    // Read FW version if needed
    static bool sendCanBefore = false;
    static int canIdBefore = 0;
    if (isConnected()) {
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
#ifndef NO_SERIAL_PORT
                disconnectSerial();
#endif
                m_packetInterface->stopUdpConnection();
                disconnectBle();
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
        if (isConnected()) {
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
        update_firmwareProgress( fw_prog * 100);
        update_firmwareUploadEnabled(false);
        update_firmwareCancelEnabled(true);
    } else {
        // If the firmware upload just finished or failed
        if (!m_firmwareUploadEnabled) {
            mFwVersionReceived = false;
            mFwRetries = 0;
            if (m_packetInterface->getFirmwareUploadStatus().compare("FW Upload Done") == 0) {
                update_firmwareProgress(100);
            } else {
                update_firmwareProgress(0);
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
            update_mcconfSlMinErpmEnabled      (true);
            update_mcconfSlMaxFbCurrEnabled    (true);
            update_mcconfSlBemfKEnabled        (true);
            update_mcconfSlBrErpmEnabled       (true);
            update_mcconfSlIntLimEnabled       (true);
            update_mcconfSlIntLimScaleBrEnabled(true);
            update_mcconfSlMinErpmIlEnabled    (true);
        } else {
            update_mcconfSlMinErpmEnabled      (true);
            update_mcconfSlMaxFbCurrEnabled    (true);
            update_mcconfSlBemfKEnabled        (false);
            update_mcconfSlBrErpmEnabled       (true);
            update_mcconfSlIntLimEnabled       (false);
            update_mcconfSlIntLimScaleBrEnabled(true);
            update_mcconfSlMinErpmIlEnabled    (false);
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
    emit updatePlots();
}

void BLDCInterface::packetDataToSend(QByteArray &data)
{
#ifndef NO_SERIAL_PORT
    if (mSerialPort->isOpen()) {
        mSerialPort->write(data);
    }
    else
#endif
        if (m_bleInterface->isConnected()) {
            m_bleInterface->write(data);
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
#ifndef NO_SERIAL_PORT
        disconnectSerial();
#endif
        m_packetInterface->stopUdpConnection();
        disconnectBle();
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
#ifndef NO_SERIAL_PORT
            disconnectSerial();
#endif
            m_packetInterface->stopUdpConnection();
            disconnectBle();
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
        update_mcconfDetectResult("Detection failed.");
        return;
    }
    emit statusInfoChanged("Detection Result Received", true);

    m_detectRes.updated = true;
    m_detectRes.cycle_int_limit = cycle_int_limit;
    m_detectRes.bemf_coupling_k = bemf_coupling_k;
    m_detectRes.hall_table = hall_table.toList();
    m_detectRes.hall_res = hall_res;

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

    update_mcconfDetectResult(QString().sprintf("Detection results:\n"
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
        set_mcconfFocMeasureEncoderOffset   (offset);
        set_mcconfFocMeasureEncoderRatio    (ratio);
        set_mcconfFocMeasureEncoderInverted (inverted);
    }
}

void BLDCInterface::focHallTableReceived(QVector<int> hall_table, int res)
{
    if (res != 0) {
        emit statusInfoChanged("Bad Detection Result Received", false);
    } else {
        emit statusInfoChanged("Hall Result Received", true);
        QList<int> table;
        foreach (auto val, hall_table)
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
    update_appconfDecodedPpm((ppm_value + 1.0) * 500.0);
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

void BLDCInterface::disconnectBle(){
    if (m_bleInterface->isConnected()) {
        m_bleInterface->disconnectDevice();
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

void BLDCInterface::writeMcconf()
{
    if (!m_mcconfLoaded) {
        emit msgCritical("Error", "The configuration should be read or loaded at least once before writing it.");
        return;
    }
    m_packetInterface->setMcconf(m_mcconf->data());
}

bool BLDCInterface::loadMcconfXml(QString xmlfile)
{
    mc_configuration tmp = m_mcconf->data();
    if (mSerialization->readMcconfXml(xmlfile, tmp)) {
        m_mcconf->setData(tmp);
        return true;
    } else {
        emit statusInfoChanged("Loading MCCONF failed", false);
        return false;
    }
}

bool BLDCInterface::saveMcconfXml(QString xmlfile)
{
    if (mSerialization->writeMcconfXml(xmlfile, m_mcconf->data())) {
        return true;
    } else {
        emit statusInfoChanged("Saving MCCONF failed", false);
        return false;
    }
}

void BLDCInterface::writeAppConf(){
    if (!m_appconfLoaded) {
        emit msgCritical( "Error", "The configuration should be read at least once before writing it.");
        return;
    }
    m_packetInterface->setAppConf(m_appconf->data());
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

void BLDCInterface::connectUdb()
{
    QHostAddress ip;

    if (ip.setAddress(m_udpIp.trimmed())) {
#ifndef NO_SERIAL_PORT
        disconnectSerial();
#endif
        disconnectBle();
        mTimer->start(TIMER_INTERVAL_UDB);
        m_packetInterface->startUdpConnection(ip, m_udpPort);
    } else {
        emit statusInfoChanged("Invalid IP address", false);
    }
}

void BLDCInterface::connectCurrentBleDevice(){

#ifndef NO_SERIAL_PORT
    if(mSerialPort->isOpen()) {
        emit statusInfoChanged("Serial port connected.", false);
        return;
    }
#endif
    mTimer->start(TIMER_INTERVAL_BLE);
    m_bleInterface->connectCurrentDevice();
}

#ifndef NO_SERIAL_PORT
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

void BLDCInterface::connectCurrentSerial()
{
    if(m_currentSerialPort >= 0 &&
            m_serialPortsLocations.count() > m_currentSerialPort){
        connectSerial(m_serialPortsLocations.at(m_currentSerialPort));
    }
}

void BLDCInterface::connectSerial(QString port)
{
    if(mSerialPort->isOpen()) {
        return;
    }
    if(m_bleInterface->isConnected()){
        emit statusInfoChanged("Bluetooth low energy connected.", false);
        return;
    }
    mSerialPort->setPortName(port);
    mSerialPort->open(QIODevice::ReadWrite);

    if(!mSerialPort->isOpen()) {
        return;
    }

    mSerialPort->setBaudRate(QSerialPort::Baud9600);
    mSerialPort->setDataBits(QSerialPort::Data8);
    mSerialPort->setParity(QSerialPort::NoParity);
    mSerialPort->setStopBits(QSerialPort::OneStop);
    mSerialPort->setFlowControl(QSerialPort::NoFlowControl);
    mTimer->start(TIMER_INTERVAL_SERIAL);

    // For nrf
    mSerialPort->setRequestToSend(true);
    mSerialPort->setDataTerminalReady(true);
    QThread::msleep(5);
    mSerialPort->setDataTerminalReady(false);
    QThread::msleep(100);

    m_packetInterface->stopUdpConnection();
}


void BLDCInterface::refreshSerialDevices()
{
    m_serialPortsLocations.clear();
    QStringList serialPortNames;

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    foreach(const QSerialPortInfo &port, ports) {
        QString name = port.portName();
        // put STMicroelectronics device first in list and add prefix
        if(port.manufacturer() == "STMicroelectronics") {
            name.insert(0, "VESC - ");
            serialPortNames.insert(0, name);
            m_serialPortsLocations.insert(0, port.systemLocation());
        }
        else{
            serialPortNames.append(name);
            m_serialPortsLocations.append(port.systemLocation());
        }
    }
    if(m_serialPortNames.isEmpty())
        set_currentSerialPort(-1);
    else
        set_currentSerialPort(0);
    update_serialPortNames(serialPortNames);
}

void BLDCInterface::disconnectSerial(){
    if (mSerialPort->isOpen()) {
        mSerialPort->close();
    }

    mFwVersionReceived = false;
    mFwRetries = 0;
}
#endif
