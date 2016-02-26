#ifndef SERIALPORTINTERFACE_H
#define BLDCINTERFACE_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QQmlListProperty>
#include <QtSerialPort/QSerialPortInfo>

#include "packetinterface.h"
#include "serialization.h"
#include "mcconfiguration.h"
#include "appconfiguration.h"
#include "lib-qt-qml-tricks/src/qqmlhelpers.h"

class SerialPort : public QObject
{
    Q_OBJECT
    QML_READONLY_PROPERTY(QString, portName)
    QML_READONLY_PROPERTY(QString, systemLocation)
public:
    SerialPort(QString name, QString location) :
    m_portName(name),
    m_systemLocation(location)
    {}
    virtual ~SerialPort() {}
};

typedef struct {
    bool updated;
    double cycle_int_limit;
    double bemf_coupling_k;
    QVector<int> hall_table;
    int hall_res;
} detect_res_t;

class BLDCInterface : public QObject
{
    Q_OBJECT

    QML_WRITABLE_PROPERTY(QString, udpIp)
    QML_LIST_PROPERTY(BLDCInterface, serialPortList, SerialPort)// @@ToDo: add notify signal
    QML_READONLY_PROPERTY(QString, firmwareSupported)
    Q_PROPERTY(McConfiguration* mcconf READ mcconf NOTIFY mcconfChanged)
    QML_READONLY_PROPERTY(QString, mcconfDetectResultBrowser)
    QML_READONLY_PROPERTY(QString, firmwareVersion)
    QML_READONLY_PROPERTY(double, firmwareProgress)
    QML_READONLY_PROPERTY(bool, firmwareUploadEnabled)
    QML_READONLY_PROPERTY(bool, firmwareCancelEnabled)
    QML_READONLY_PROPERTY(QString, firmwareUploadStatus)
    QML_READONLY_PROPERTY(double, rotorPos)
    QML_READONLY_PROPERTY(QString, status)

    QML_WRITEONLY_PROPERTY(int, sampleNum)
    QML_WRITEONLY_PROPERTY(int, mcconfFocCalcCCTc)
    QML_WRITEONLY_PROPERTY(int, mcconfFocCalcKp)
    QML_WRITEONLY_PROPERTY(int, mcconfFocCalcKi)
    QML_WRITEONLY_PROPERTY(bool, canFwd)
    QML_WRITEONLY_PROPERTY(bool, canId)
    QML_WRITEONLY_PROPERTY(bool, realtimeActivate)
    QML_WRITEONLY_PROPERTY(bool, appconfUpdatePpm       )
    QML_WRITEONLY_PROPERTY(bool, appconfAdcUpdate       )
    QML_WRITEONLY_PROPERTY(bool, appconfUpdateChuk      )
    QML_WRITABLE_PROPERTY(bool, mcconfCommInt           )

    QML_WRITEONLY_PROPERTY(bool, keyLeft )
    QML_WRITEONLY_PROPERTY(bool, keyRight)

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
    QML_WRITABLE_PROPERTY(double, mcconfFocMeasureEncoderCurrent  )
    QML_WRITABLE_PROPERTY(double, mcconfFocMeasureHallCurrent	  )
    QML_WRITABLE_PROPERTY(QList<double>, mcconfDetectHallTable	  )
    QML_WRITABLE_PROPERTY(QList<double>, mcconfFocMeasureHallTable)
    QML_READONLY_PROPERTY(double, appconfDecodedChuk)
    QML_READONLY_PROPERTY(double, appconfAdcDecoded )
    QML_READONLY_PROPERTY(double, appconfAdcDecoded2)
    QML_READONLY_PROPERTY(double, appconfAdcVoltage)
    QML_READONLY_PROPERTY(double, appconfAdcVoltage2)
    QML_READONLY_PROPERTY(double, appconfDecodedPpm)
    QML_READONLY_PROPERTY(double, appconfPpmPulsewidth)
    Q_PROPERTY(AppConfiguration *appconf READ appconf NOTIFY appconfChanged)



public:
    explicit BLDCInterface(QObject *parent = 0);

    AppConfiguration * appconf() const
    {
        return m_appconf;
    }

    McConfiguration* mcconf() const
    {
        return m_mcconf;
    }

signals:
    void statusInfoChanged(QString info, bool isGood);
    void msgCritical(QString title, QString text);
    void msgwarning(QString title, QString text);
    void ackReceived(QString ackType);
    void mcValuesReceived(MC_VALUES values);
    void printReceived(QString str);
    void experimentSamplesReceived(QVector<double> samples);
    void serialPortsInfoChanged(QQmlListProperty<QSerialPortInfo> serialPortsInfo);
    void rotorPosReceived(double pos);
    void appconfChanged(AppConfiguration * appconf);
    void mcconfChanged(McConfiguration* mcconf);

private slots:

    void serialDataAvailable();
    void serialPortError(QSerialPort::SerialPortError error);

    void timerSlot();
    void packetDataToSend(QByteArray &data);
    void fwVersionReceived(int major, int minor);
    void disconnect();

    void samplesReceived(QByteArray data);
    void mcconfReceived(mc_configuration &mcconf);
    void motorParamReceived(double cycle_int_limit, double bemf_coupling_k, QVector<int> hall_table, int hall_res);
    void motorRLReceived(double r, double l);
    void motorLinkageReceived(double flux_linkage);
    void encoderParamReceived(double offset, double ratio, bool inverted);
    void focHallTableReceived(QVector<int> hall_table, int res);
    void appconfReceived(app_configuration appconf);
    void decodedPpmReceived(double ppm_value, double ppm_last_len);
    void decodedAdcReceived(double adc_value, double adc_voltage, double adc_value2, double adc_voltage2);
    void decodedChukReceived(double chuk_value);
    void mcconfFocCalcCC();
private:

    void refreshSerialDevices();

    QSerialPort *mSerialPort;
    QTimer *mTimer;
    int mStatusInfoTime;
    bool mFwVersionReceived;
    int mFwRetries;
    QList<QPair<int, int> > mCompatibleFws;
    PacketInterface *mPacketInterface;
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

    detect_res_t mDetectRes;

    bool mDoReplot;
    bool mDoReplotPos;
    bool mDoRescale;
    bool mDoFilterReplot;
    bool mMcconfLoaded;
    bool mAppconfLoaded;

    AppConfiguration * m_appconf;
    McConfiguration* m_mcconf;
};

#endif // BLDCINTERFACE_H
