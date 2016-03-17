#include "bleinterface.h"
#include <QDebug>

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
    m_deviceConnected(false)
{


    m_btDeviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    connect(m_btDeviceDiscoveryAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)),
            this, SLOT(addDevice(const QBluetoothDeviceInfo&)));
    connect(m_btDeviceDiscoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
            this, SLOT(onDeviceScanError(QBluetoothDeviceDiscoveryAgent::Error)));
    connect(m_btDeviceDiscoveryAgent, SIGNAL(finished()), this, SLOT(onScanFinished()));
    m_btDeviceDiscoveryAgent->start();
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
    //! [devicediscovery-2]
    m_deviceDiscoveryAgent->start();
    //! [devicediscovery-2]
    emit statusInfoChanged("Scanning for devices...", true);
}

void BLEInterface::write(const QByteArray &data)
{
    QLowEnergyCharacteristic characteristic  =  m_service->characteristic(QBluetoothUuid(CHARACTARISTIC_UUID));
    if(characteristic.isValid() &&
            (characteristic.properties() & QLowEnergyCharacteristic::WriteNoResponse) ){
        m_service->writeCharacteristic(characteristic, data);
    }
    else {
        qWarning() << "Write is not supported.";
    }
}

//! [devicediscovery-3]
void BLEInterface::addDevice(const QBluetoothDeviceInfo &device)
{
    if (device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        qWarning() << "Discovered LE Device name: " << device.name() << " Address: "
                   << device.address().toString();
//! [devicediscovery-3]
        m_devicesNames.append(device.name());
        DeviceInfo *dev = new DeviceInfo(device);
        m_devices.append(dev);
        emit devicesNamesChanged(m_devicesNames);
        emit statusInfoChanged("Low Energy device found. Scanning for more...", true);
//! [devicediscovery-4]
    }
    //...
}
//! [devicediscovery-4]

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
    //! [Connect signals]
    m_control = new QLowEnergyController(m_devices[ m_currentDevice]->getDevice(), this);
    connect(m_control, SIGNAL(onServiceDiscovered(QBluetoothUuid)),
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
    //! [Connect signals]
}

//! [Connecting to service]

void BLEInterface::onDeviceConnected()
{
    update_deviceConnected(true);
    m_control->discoverServices();
}

void BLEInterface::onDeviceDisconnected()
{
    update_deviceConnected(false);
    emit statusInfoChanged("Service disconnected", false);
    qWarning() << "Remote device disconnected";
}

//! [Connecting to service]

//! [Filter BLDCSLAVE service 1]
void BLEInterface::onServiceDiscovered(const QBluetoothUuid &gatt)
{
    if (gatt == QBluetoothUuid(SERVICE_UUID)) {
        emit statusInfoChanged("Service discovered. Waiting for service scan to be done...", true);
        m_foundService = true;
    }
}
//! [Filter BLDCSLAVE service 1]

void BLEInterface::onServiceScanDone()
{
    delete m_service;
    m_service = 0;

    //! [Filter BLDCSLAVE service 2]
    if (m_foundService) {
        emit statusInfoChanged("Connecting to service...", true);
        m_service = m_control->createServiceObject(
                    QBluetoothUuid(SERVICE_UUID), this);
    }

    if (!m_service) {
        emit statusInfoChanged("Service not found.", false);
        return;
    }

    connect(m_service, SIGNAL(stateChanged(QLowEnergyService::ServiceState)),
            this, SLOT(onServiceStateChanged(QLowEnergyService::ServiceState)));
    connect(m_service, SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),
            this, SLOT(onCharacteristicChanged(QLowEnergyCharacteristic,QByteArray)));

    m_service->discoverDetails();
    //! [Filter BLDCSLAVE service 2]
}

void BLEInterface::disconnectDevice()
{
    m_foundService = false;

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

//! [Error handling]
void BLEInterface::onControllerError(QLowEnergyController::Error error)
{
    emit statusInfoChanged("Cannot connect to remote device.", false);
    qWarning() << "Controller Error:" << error;
}
//! [Error handling]


//! [Reading value 1]
void BLEInterface::onCharacteristicChanged(const QLowEnergyCharacteristic &c,
                                     const QByteArray &value)
{
    // ignore any other characteristic change -> shouldn't really happen though
    if (c.uuid() != QBluetoothUuid(CHARACTARISTIC_UUID))
        return;
    emit dataReceived(value);
}

void BLEInterface::onServiceStateChanged(QLowEnergyService::ServiceState s)
{
    qDebug() << "serviceStateChanged, state: " << s;
        //nothing for now

}
void BLEInterface::serviceError(QLowEnergyService::ServiceError e)
{
    qWarning() << "Service error:" << e;

}


