#include "bleinterface.h"
#include <QDebug>
#include <QEventLoop>

DeviceInfo::DeviceInfo(const QBluetoothDeviceInfo &info):
    QObject(), m_device(info)
{
}

QBluetoothDeviceInfo DeviceInfo::getDevice() const
{
    return m_device;
}

QString DeviceInfo::getAddress() const
{
#ifdef Q_OS_MAC
    // workaround for Core Bluetooth:
    return m_device.deviceUuid().toString();
#else
    return m_device.address().toString();
#endif
}

void DeviceInfo::setDevice(const QBluetoothDeviceInfo &device)
{
    m_device = device;
    emit deviceChanged();
}

BLEInterface::BLEInterface(QObject *parent) : QObject(parent),
    m_currentDevice(0),
    m_control(0),
    m_service(0),
    m_readTimer(0),
    m_connected(false){
    m_deviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    
    connect(m_deviceDiscoveryAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)),
            this, SLOT(addDevice(const QBluetoothDeviceInfo&)));
    connect(m_deviceDiscoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
            this, SLOT(onDeviceScanError(QBluetoothDeviceDiscoveryAgent::Error)));
    connect(m_deviceDiscoveryAgent, SIGNAL(finished()), this, SLOT(onScanFinished()));
}

BLEInterface::~BLEInterface()
{
    qDeleteAll(m_devices);
    m_devices.clear();
}

void BLEInterface::scanDevices()
{
    m_devicesNames.clear();
    qDeleteAll(m_devices);
    m_devices.clear();
    emit devicesNamesChanged(m_devicesNames);
    m_deviceDiscoveryAgent->start();
    emit statusInfoChanged("Scanning for devices...", true);
}

void BLEInterface::read(){
    if(m_service && m_readCharacteristic.isValid())
        m_service->readCharacteristic(m_readCharacteristic);
}

void BLEInterface::waitForWrite(){
    QEventLoop pause;
    connect(m_service, &QLowEnergyService::characteristicWritten,
            &pause, &QEventLoop::quit);
    connect(m_service, SIGNAL(error(QLowEnergyService::ServiceError)),
            &pause, SLOT(quit()));
    pause.exec();
}
void BLEInterface::write(const QByteArray &data)
{
    qDebug() << "BLEInterface::write: " << data;
    if(m_service && m_writeCharacteristic.isValid()){
        if(data.length() > CHUNK_SIZE){
            int sentBytes = 0;
            while (sentBytes < data.length()) {
                m_service->writeCharacteristic(m_writeCharacteristic,
                                               data.mid(sentBytes, CHUNK_SIZE),
                                               m_writeMode);
                sentBytes += CHUNK_SIZE;
                if(m_writeMode == QLowEnergyService::WriteWithResponse){
                    waitForWrite();
                    if(m_service->error() != QLowEnergyService::NoError)
                        break;
                }
            }
        }
        else
            m_service->writeCharacteristic(m_writeCharacteristic, data, m_writeMode);
    }
}

void BLEInterface::addDevice(const QBluetoothDeviceInfo &device)
{
    if (device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        qWarning() << "Discovered LE Device name: " << device.name() << " Address: "
                   << device.address().toString();
        m_devicesNames.append(device.name());
        DeviceInfo *dev = new DeviceInfo(device);
        m_devices.append(dev);
        emit devicesNamesChanged(m_devicesNames);
        emit statusInfoChanged("Low Energy device found", true);
        if(AUTOCONNECT_DEVICES.contains(device.name())){
            m_deviceDiscoveryAgent->stop();
            set_currentDevice(m_devices.length()-1);
            connectCurrentDevice();
        }
    }
    //...
}

void BLEInterface::onScanFinished()
{
    if (m_devicesNames.size() == 0)
        emit statusInfoChanged("No Low Energy devices found", false);
}

void BLEInterface::onDeviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        emit statusInfoChanged("The Bluetooth adaptor is powered off, power it on before doing discovery.", false);
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
        emit statusInfoChanged("Writing or reading from the device resulted in an error.", false);
    else
        emit statusInfoChanged("An unknown error has occurred.", false);
}



void BLEInterface::connectCurrentDevice()
{
    if(m_devices.isEmpty())
        return;
    
    if (m_control) {
        m_control->disconnectFromDevice();
        delete m_control;
        m_control = 0;
        
    }
    m_control = new QLowEnergyController(m_devices[ m_currentDevice]->getDevice(), this);
    connect(m_control, SIGNAL(serviceDiscovered(QBluetoothUuid)),
            this, SLOT(onServiceDiscovered(QBluetoothUuid)));
    connect(m_control, SIGNAL(discoveryFinished()),
            this, SLOT(onServiceScanDone()));
    connect(m_control, SIGNAL(error(QLowEnergyController::Error)),
            this, SLOT(onControllerError(QLowEnergyController::Error)));
    connect(m_control, SIGNAL(connected()),
            this, SLOT(onDeviceConnected()));
    connect(m_control, SIGNAL(disconnected()),
            this, SLOT(onDeviceDisconnected()));
    
    m_control->connectToDevice();
}

void BLEInterface::onDeviceConnected()
{
    m_control->discoverServices();
}

void BLEInterface::onDeviceDisconnected()
{
    update_connected(false);
    emit statusInfoChanged("Service disconnected", false);
    qWarning() << "Remote device disconnected";
}

void BLEInterface::onServiceDiscovered(const QBluetoothUuid &gatt)
{

    foreach (auto uuid, SERVICE_UUIDs) {
        m_serviceUuid = QBluetoothUuid(uuid);
        if(gatt == m_serviceUuid){
            emit statusInfoChanged("Service discovered. Waiting for service scan to be done...", true);
            m_foundService = true;
            break;
        }
    }
}

void BLEInterface::onServiceScanDone()
{
    delete m_service;
    m_service = 0;
    
    if (m_foundService) {
        m_service = m_control->createServiceObject(
                    m_serviceUuid, this);
    }
    
    if (!m_service) {
        emit statusInfoChanged("Service not found.", false);
        return;
    }
    
    connect(m_service, SIGNAL(stateChanged(QLowEnergyService::ServiceState)),
            this, SLOT(onServiceStateChanged(QLowEnergyService::ServiceState)));
    connect(m_service, SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),
            this, SLOT(onCharacteristicChanged(QLowEnergyCharacteristic,QByteArray)));
    connect(m_service, SIGNAL(characteristicRead(QLowEnergyCharacteristic,QByteArray)),
            this, SLOT(onCharacteristicRead(QLowEnergyCharacteristic,QByteArray)));
    connect(m_service, SIGNAL(characteristicWritten(QLowEnergyCharacteristic,QByteArray)),
            this, SLOT(onCharacteristicWrite(QLowEnergyCharacteristic,QByteArray)));
    connect(m_service, SIGNAL(error(QLowEnergyService::ServiceError)),
            this, SLOT(serviceError(QLowEnergyService::ServiceError)));
    
    if(m_service->state() == QLowEnergyService::DiscoveryRequired) {
        emit statusInfoChanged("Connecting to service...", true);
        m_service->discoverDetails();
    }
    else
        searchCharacteristic();
    
}

void BLEInterface::disconnectDevice()
{
    m_foundService = false;
    m_readTimer->deleteLater();
    m_readTimer = NULL;
    
    if (m_devices.isEmpty()) {
        return;
    }
    
    //disable notifications
    if (m_notificationDesc.isValid() && m_service) {
        m_service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0000"));
    } else {
        m_control->disconnectFromDevice();
        delete m_service;
        m_service = 0;
    }
}

void BLEInterface::onControllerError(QLowEnergyController::Error error)
{
    emit statusInfoChanged("Cannot connect to remote device.", false);
    qWarning() << "Controller Error:" << error;
}

void BLEInterface::onCharacteristicChanged(const QLowEnergyCharacteristic &c,
                                           const QByteArray &value)
{
    Q_UNUSED(c)
    qDebug() << "Characteristic Changed: " << value;
    emit dataReceived(value);
}
void BLEInterface::onCharacteristicWrite(const QLowEnergyCharacteristic &c,
                                         const QByteArray &value)
{
    Q_UNUSED(c)
    qDebug() << "Characteristic Written: " << value;
}
void BLEInterface::onCharacteristicRead(const QLowEnergyCharacteristic &c,
                                        const QByteArray &value){
    Q_UNUSED(c)
    qDebug() << "Characteristic Read: " << value;
    emit dataReceived(value);
}

void BLEInterface::searchCharacteristic(){
    if(m_service){
        QMap<QLowEnergyCharacteristic::PropertyTypes, QLowEnergyCharacteristic> properties;
        foreach (QLowEnergyCharacteristic c, m_service->characteristics()) {
            if(c.isValid()){
                if (c.properties() & QLowEnergyCharacteristic::WriteNoResponse){
                    properties[QLowEnergyCharacteristic::WriteNoResponse] = c;
                }
                if(c.properties() & QLowEnergyCharacteristic::Write) {
                    properties[QLowEnergyCharacteristic::Write] = c;
                }
                if (c.properties() & QLowEnergyCharacteristic::Read) {
                    properties[QLowEnergyCharacteristic::Read] = c;
                }
                if (c.properties() & QLowEnergyCharacteristic::Notify) {
                    properties[QLowEnergyCharacteristic::Notify] = c;
                    m_notificationDesc = c.descriptor(
                                QBluetoothUuid::ClientCharacteristicConfiguration);
                    if (m_notificationDesc.isValid()) {
                        m_service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));
                    }
                }
            }
        }

        bool valid = ( properties.contains(QLowEnergyCharacteristic::WriteNoResponse) ||
                       properties.contains(QLowEnergyCharacteristic::Write)) &&
                (properties.contains(QLowEnergyCharacteristic::Notify) ||
                 properties.contains(QLowEnergyCharacteristic::Read));
        if(valid){
            if(!properties.contains(QLowEnergyCharacteristic::Notify) &&
                    properties.contains(QLowEnergyCharacteristic::Read)){
                m_readCharacteristic = properties[QLowEnergyCharacteristic::Read];
                if(!m_readTimer){
                    m_readTimer = new QTimer(this);
                    connect(m_readTimer, &QTimer::timeout, this, &BLEInterface::read);
                    m_readTimer->start(READ_INTERVAL_MS);
                }
            }
            if(properties.contains(QLowEnergyCharacteristic::Write)){
                m_writeCharacteristic = properties[QLowEnergyCharacteristic::Write];
                m_writeMode = QLowEnergyService::WriteWithResponse;
            }
            else if(properties.contains(QLowEnergyCharacteristic::WriteNoResponse)){
                m_writeCharacteristic = properties[QLowEnergyCharacteristic::WriteNoResponse];
                m_writeMode = QLowEnergyService::WriteWithoutResponse;
            }
            update_connected(true);
        }
    }
}


void BLEInterface::onServiceStateChanged(QLowEnergyService::ServiceState s)
{
    qDebug() << "serviceStateChanged, state: " << s;
    if (s == QLowEnergyService::ServiceDiscovered) {
        searchCharacteristic();
    }
}
void BLEInterface::serviceError(QLowEnergyService::ServiceError e)
{
    qWarning() << "Service error:" << e;
}
