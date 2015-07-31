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

#include "rtdatawidget.h"
#include <QPainter>
#include <QPaintEvent>

RtDataWidget::RtDataWidget(QWidget *parent) :
    QWidget(parent)
{
}

void RtDataWidget::setValues(MC_VALUES &values)
{
    mValues = values;
    mValues.fault_str.remove(0, 11);
    repaint();
}

void RtDataWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Paint begins here
    painter.fillRect(event->rect(), QBrush(Qt::transparent));

    const double fs = 12;
    const double box1_w = 400;
    const double box2_w = 400;
    const double box_h = 140;
    const double vidw = event->rect().width();

    QString str;
    QPen pen;
    QFont font;

    // Upper left info box
    str.sprintf("Power            : %.1f W\n"
                "Duty cycle       : %.2f %%\n"
                "Electrical speed : %.1f rpm\n"
                "Battery current  : %.2f A\n"
                "Motor current    : %.2f A\n"
                "MOSFET Temp      : %.2f Deg C\n"
                "Fault code       : %s",
                mValues.v_in * mValues.current_in,
                mValues.duty_now * 100.0,
                mValues.rpm,
                mValues.current_in,
                mValues.current_motor,
                mValues.temp_mos1,
                mValues.fault_str.toLocal8Bit().data());

    painter.setOpacity(0.7);
    painter.fillRect(0, 0, box1_w, box_h, Qt::black);
    painter.setOpacity(1.0);

    pen.setColor(Qt::white);
    painter.setPen(pen);
    font.setPointSize(fs);
    font.setFamily("Monospace");
    painter.setFont(font);
    painter.drawText(QRectF(5, 5, box1_w, box_h), Qt::AlignLeft, str);

    // Upper right info box
    str.sprintf("Drawn cap       : %.1f mAh\n"
                "Charged cap     : %.1f mAh\n"
                "Drawn energy    : %.2f Wh\n"
                "Charged energy  : %.2f Wh\n"
                "Tacho           : %i counts\n"
                "Tacho ABS       : %i counts\n"
                "Battery voltage : %.1f V",
                mValues.amp_hours * 1000.0,
                mValues.amp_hours_charged * 1000.0,
                mValues.watt_hours,
                mValues.watt_hours_charged,
                mValues.tachometer,
                mValues.tachometer_abs,
                mValues.v_in);

    painter.setOpacity(0.7);
    painter.fillRect(vidw - box2_w, 0, box2_w, box_h, Qt::black);
    painter.setOpacity(1.0);

    pen.setColor(Qt::white);
    painter.setPen(pen);
    font.setPointSize(fs);
    font.setFamily("Monospace");
    painter.setFont(font);
    painter.drawText(QRectF(vidw -box2_w + 5, 5, box2_w, box_h), Qt::AlignLeft, str);
}
