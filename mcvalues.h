#ifndef MCVALUES_H
#define MCVALUES_H

#include "datatypes.h"
#include "lib-qt-qml-tricks/src/qqmlhelpers.h"

#include <QObject>

class McValues : public QObject
{
    Q_OBJECT
        QML_READONLY_PROPERTY(double, power           )
        QML_READONLY_PROPERTY(double, batteryVoltage  )
        QML_READONLY_PROPERTY(double, MOSFET_Temp     )
        QML_READONLY_PROPERTY(double, motorCurrent    )
        QML_READONLY_PROPERTY(double, batteryCurrent  )
        QML_READONLY_PROPERTY(double, electricalSpeed )
        QML_READONLY_PROPERTY(double, dutyCycle       )
        QML_READONLY_PROPERTY(double, drawnCap        )
        QML_READONLY_PROPERTY(double, chargedCap      )
        QML_READONLY_PROPERTY(double, drawnEnergy     )
        QML_READONLY_PROPERTY(double, chargedEnergy   )
        QML_READONLY_PROPERTY(int,    tacho           )
        QML_READONLY_PROPERTY(int,    tachoABS        )
    QML_READONLY_PROPERTY(QString,faultString     )
public:
    explicit McValues(QObject *parent = 0);
    McValues(MC_VALUES values);
    void setValues(MC_VALUES values);

};
#endif // MCVALUES_H
