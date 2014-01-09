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

#ifndef LOCPOINT_H
#define LOCPOINT_H

#include <QPointF>

class LocPoint
{
public:
    LocPoint(double x = 0, double y = 0, double alpha = 0,
             double speed = 0.5, double radius = 0, double sigma = 0);

    double getX();
    double getY();
    double getAlpha();
    double getSpeed();
    QPointF getPoint();
    double getRadius();
    double getSigma();

    void setX(double x);
    void setY(double y);
    void setXY(double x, double y);
    void scaleXY(double scalefactor);
    void setAlpha(double alpha);
    void setSpeed(double speed);
    void setRadius(double radius);
    void setSigma(double sigma);

    // Operators
    LocPoint& operator=(const LocPoint& point);
    bool operator==(const LocPoint& point);
    bool operator!=(const LocPoint& point);

private:
    double mX;
    double mY;
    double mAlpha;
    double mSpeed;
    double mRadius;
    double mSigma;
};

#endif // LOCPOINT_H
