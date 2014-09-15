#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <QObject>
#include "packetinterface.h"

class Serialization : public QObject
{
    Q_OBJECT
public:
    explicit Serialization(QObject *parent = 0);
    bool writeMcconfXml(const PacketInterface::mc_configuration &mcconf, QWidget *parent = 0);
    bool readMcconfXml(PacketInterface::mc_configuration &mcconf, QWidget *parent = 0);

signals:

public slots:

};

#endif // SERIALIZATION_H
