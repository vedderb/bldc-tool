#ifndef BLEINTERFACE_H
#define BLEINTERFACE_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QLowEnergyService>

#include "lib-qt-qml-tricks/src/qqmlhelpers.h"

static quint16 SERVICE_UUID = 0xFFE2;
static quint16 CHARACTARISTIC_UUID = 0xFFE1;

class DeviceInfo: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString deviceName READ getName NOTIFY deviceChanged)
    Q_PROPERTY(QString deviceAddress READ getAddress NOTIFY deviceChanged)
public:
    DeviceInfo(const QBluetoothDeviceInfo &device);
    void setDevice(const QBluetoothDeviceInfo &device);
    QString getName() const { return m_device.name(); }
    QString getAddress() const;
    QBluetoothDeviceInfo getDevice() const;

signals:
    void deviceChanged();

private:
    QBluetoothDeviceInfo m_device;
};

class BLEInterface : public QObject
{
    Q_OBJECT

    QML_WRITABLE_PROPERTY(int, currentDevice)
    QML_READONLY_PROPERTY(QStringList, devicesNames)
    QML_READONLY_PROPERTY(bool, deviceConnected)
public:
    explicit BLEInterface(QObject *parent = 0);
    ~BLEInterface();

    void connectCurrentDevice();
    void disconnectDevice();
    Q_INVOKABLE void scanDevices();
    void write(const QByteArray& data);

signals:
    void statusInfoChanged(QString info, bool isGood);
    void dataReceived(const QByteArray &data);
private slots:
    //QBluetothDeviceDiscoveryAgent
    void addDevice(const QBluetoothDeviceInfo&);
    void onScanFinished();
    void onDeviceScanError(QBluetoothDeviceDiscoveryAgent::Error);

    //QLowEnergyController
    void onServiceDiscovered(const QBluetoothUuid &);
    void onServiceScanDone();
    void onControllerError(QLowEnergyController::Error);
    void onDeviceConnected();
    void onDeviceDisconnected();


    //QLowEnergyService
    void onServiceStateChanged(QLowEnergyService::ServiceState s);
    void onCharacteristicChanged(const QLowEnergyCharacteristic &c,
                              const QByteArray &value);
    void serviceError(QLowEnergyService::ServiceError e);

private:
    QBluetoothDeviceDiscoveryAgent *m_deviceDiscoveryAgent;
    QLowEnergyDescriptor m_notificationDesc;
    QLowEnergyController *m_control;
    QLowEnergyService *m_service;
    QList<DeviceInfo*> m_devices;
    bool m_foundService;
};

#endif // BLEINTERFACE_H
