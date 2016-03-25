#ifndef BLDCINTERFACE_H
#define BLDCINTERFACE_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QtSerialPort/QSerialPortInfo>
#include <QFile>
#include <QFileInfo>
#include <QUrl>

#include "packetinterface.h"
#include "serialization.h"
#include "mcconfiguration.h"
#include "appconfiguration.h"
#include "lib-qt-qml-tricks/src/qqmlhelpers.h"
#include "mcvalues.h"
#include "bleinterface.h"

typedef struct {
    bool updated;
    double cycle_int_limit;
    double bemf_coupling_k;
    QList<int> hall_table;
    int hall_res;
} detect_res_t;


QML_ENUM_CLASS(OS,Unkown=0,Windows,Linux,OSX,Android,IOS)

class BLDCInterface : public QObject
{
    Q_OBJECT

    QML_CONSTANT_PROPERTY(OS::Type, os)

    QML_READONLY_PROPERTY(PacketInterface*, packetInterface)
    QML_READONLY_PROPERTY(McConfiguration*, mcconf )
    QML_READONLY_PROPERTY(AppConfiguration*, appconf)
    QML_READONLY_PROPERTY(McValues*, mcValues)
    QML_READONLY_PROPERTY(BLEInterface*, bleInterface)

    QML_WRITABLE_PROPERTY(QString, udpIp)
    QML_WRITABLE_PROPERTY(int, udpPort)
    QML_WRITABLE_PROPERTY(int, currentSerialPort)

    QML_READONLY_PROPERTY(QStringList, serialPortNames)
    QML_READONLY_PROPERTY(QString, firmwareSupported)
    QML_READONLY_PROPERTY(QString, firmware)
    QML_READONLY_PROPERTY(QString, mcconfDetectResult)
    QML_READONLY_PROPERTY(QString, firmwareVersion)
    QML_READONLY_PROPERTY(int, firmwareProgress)
    QML_READONLY_PROPERTY(bool, firmwareUploadEnabled)
    QML_READONLY_PROPERTY(bool, firmwareCancelEnabled)
    QML_READONLY_PROPERTY(bool, mcconfSlMinErpmEnabled      )
    QML_READONLY_PROPERTY(bool, mcconfSlMaxFbCurrEnabled    )
    QML_READONLY_PROPERTY(bool, mcconfSlBemfKEnabled        )
    QML_READONLY_PROPERTY(bool, mcconfSlBrErpmEnabled       )
    QML_READONLY_PROPERTY(bool, mcconfSlIntLimEnabled       )
    QML_READONLY_PROPERTY(bool, mcconfSlIntLimScaleBrEnabled)
    QML_READONLY_PROPERTY(bool, mcconfSlMinErpmIlEnabled    )
    QML_READONLY_PROPERTY(QString, firmwareUploadStatus)
    QML_READONLY_PROPERTY(double, rotorPos)
    QML_READONLY_PROPERTY(QString, status)
    QML_READONLY_PROPERTY(bool, mcconfLoaded )
    QML_READONLY_PROPERTY(bool, appconfLoaded)

    QML_WRITEONLY_PROPERTY(int, sampleNum)
    QML_WRITEONLY_PROPERTY(int, mcconfFocCalcCCTc)
    QML_WRITEONLY_PROPERTY(int, canId)
    QML_WRITEONLY_PROPERTY(bool, canFwd)
    QML_WRITEONLY_PROPERTY(bool, realtimeActivate)
    QML_WRITEONLY_PROPERTY(bool, appconfUpdatePpm)
    QML_WRITEONLY_PROPERTY(bool, appconfAdcUpdate)
    QML_WRITEONLY_PROPERTY(bool, appconfUpdateChuk)
    QML_WRITEONLY_PROPERTY(bool, overrideKb)
    QML_WRITEONLY_PROPERTY(double, sampleFreq)
    QML_WRITEONLY_PROPERTY(bool, horizontalZoom)
    QML_WRITEONLY_PROPERTY(bool, verticalZoom)
    QML_WRITEONLY_PROPERTY(bool, keyLeft )
    QML_WRITEONLY_PROPERTY(bool, keyRight)

    QML_WRITABLE_PROPERTY(bool, mcconfCommInt)
    QML_WRITABLE_PROPERTY(int, mcconfFocCalcKp)
    QML_WRITABLE_PROPERTY(int, mcconfFocCalcKi)
    QML_WRITABLE_PROPERTY(double, mcconfFocDetectR		          )
    QML_WRITABLE_PROPERTY(double, mcconfFocDetectL		          )
    QML_WRITABLE_PROPERTY(double, mcconfFocDetectLinkage	      )
    QML_WRITABLE_PROPERTY(double, mcconfDetectCurrent		      )
    QML_WRITABLE_PROPERTY(double, mcconfDetectErpm		          )
    QML_WRITABLE_PROPERTY(double, mcconfDetectLowDuty		      )
    QML_WRITABLE_PROPERTY(double, mcconfFocDetectCurrent	      )
    QML_WRITABLE_PROPERTY(double, mcconfFocDetectMinRpm	          )
    QML_WRITABLE_PROPERTY(double, mcconfFocDetectDuty		      )
    QML_WRITABLE_PROPERTY(double, mcconfFocMeasureEncoderOffset	  )
    QML_WRITABLE_PROPERTY(double, mcconfFocMeasureEncoderRatio	  )
    QML_WRITABLE_PROPERTY(double, mcconfFocMeasureEncoderInverted )
    QML_WRITABLE_PROPERTY(QList<int>, mcconfFocMeasureHallTable)
    QML_READONLY_PROPERTY(double, appconfDecodedChuk)
    QML_READONLY_PROPERTY(double, appconfAdcDecoded )
    QML_READONLY_PROPERTY(double, appconfAdcDecoded2)
    QML_READONLY_PROPERTY(double, appconfAdcVoltage)
    QML_READONLY_PROPERTY(double, appconfAdcVoltage2)
    QML_READONLY_PROPERTY(int, appconfDecodedPpm)
    QML_READONLY_PROPERTY(double, appconfPpmPulsewidth)

public:
    explicit BLDCInterface(QObject *parent = 0);
    const detect_res_t& get_detectRes() const{
        return m_detectRes;
    }
    bool isConnected(){
        return mSerialPort->isOpen() || m_bleInterface->isConnected() || m_packetInterface->isUdpConnected();
    }

signals:
    void statusInfoChanged(QString info, bool isGood);
    void msgCritical(QString title, QString text);
    void msgwarning(QString title, QString text);
    void mcValuesReceived(MC_VALUES values);
    void updatePlots();

public:
    Q_INVOKABLE static QString localFilePath(QUrl path){
        return path.toLocalFile();
    }

    static void stepTowards(double &value, double goal, double step);
public slots:

    void connectCurrentSerial();
    void connectSerial(QString port);
    void serialDataAvailable();
    void serialPortError(QSerialPort::SerialPortError error);
    void packetDataToSend(QByteArray &data);
    void disconnectSerial();
    void detect();
    void stopDetect();
    void writeMcconf();
    bool loadMcconfXml(QString xmlfile);
    void writeAppConf();
    void uploadFirmware(QString fileName);
    void readFirmwareVersion();
    void connectCurrentBleDevice();
    void disconnectBle();
    void connectUdb();
    void mcconfFocCalcCC();
    void refreshSerialDevices();
    bool saveMcconfXml(QString xmlfile);
private slots:

    void timerSlot();
    void fwVersionReceived(int major, int minor);
    void mcconfReceived(mc_configuration mcconf);
    void motorParamReceived(double cycle_int_limit, double bemf_coupling_k, QVector<int> hall_table, int hall_res);
    void motorRLReceived(double r, double l);
    void motorLinkageReceived(double flux_linkage);
    void encoderParamReceived(double offset, double ratio, bool inverted);
    void focHallTableReceived(QVector<int> hall_table, int res);
    void appconfReceived(app_configuration appconf);
    void decodedPpmReceived(double ppm_value, double ppm_last_len);
    void decodedAdcReceived(double adc_value, double adc_voltage, double adc_value2, double adc_voltage2);
    void decodedChukReceived(double chuk_value);
private:

    void clearBuffers();

    QSerialPort *mSerialPort;
    QStringList m_serialPortsLocations;
    QTimer *mTimer;
    int mStatusInfoTime;
    bool mFwVersionReceived;
    int mFwRetries;
    QList<QPair<int, int> > mCompatibleFws;
    Serialization *mSerialization;
    detect_res_t m_detectRes;
};

#endif // BLDCINTERFACE_H
