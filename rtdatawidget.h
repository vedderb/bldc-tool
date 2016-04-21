/*
    Copyright 2012-2014 Benjamin Vedder	benjamin@vedder.se

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#ifndef RTDATAWIDGET_H
#define RTDATAWIDGET_H

#include <QWidget>
#include "packetinterface.h"

class RtDataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RtDataWidget(QWidget *parent = 0);
    void setValues(MC_VALUES &values);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    MC_VALUES mValues;

};

#endif // RTDATAWIDGET_H
