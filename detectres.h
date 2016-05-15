#ifndef DETECTRES_H
#define DETECTRES_H

#include <QObject>
#include <QList>

#include "lib-qt-qml-tricks/src/qqmlhelpers.h"

class DetectRes : public QObject
{
    Q_OBJECT
    QML_WRITABLE_PROPERTY(bool, updated)
    QML_WRITABLE_PROPERTY(double, cycle_int_limit)
    QML_WRITABLE_PROPERTY(double, bemf_coupling_k)
    QML_WRITABLE_PROPERTY(int, hall_table1)
    QML_WRITABLE_PROPERTY(int, hall_table2)
    QML_WRITABLE_PROPERTY(int, hall_table3)
    QML_WRITABLE_PROPERTY(int, hall_table4)
    QML_WRITABLE_PROPERTY(int, hall_table5)
    QML_WRITABLE_PROPERTY(int, hall_table6)
    QML_WRITABLE_PROPERTY(int, hall_table7)
    QML_WRITABLE_PROPERTY(int, hall_table8)
    QML_WRITABLE_PROPERTY(int, hall_res)

public:
    explicit DetectRes(QObject *parent = 0);

signals:

public slots:
};

#endif // DETECTRES_H
