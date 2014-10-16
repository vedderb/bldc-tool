#ifndef RTDATAWIDGET_H
#define RTDATAWIDGET_H

#include <QWidget>
#include "packetinterface.h"

class RtDataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RtDataWidget(QWidget *parent = 0);
    void setValues(PacketInterface::MC_VALUES &values);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    PacketInterface::MC_VALUES mValues;

};

#endif // RTDATAWIDGET_H
