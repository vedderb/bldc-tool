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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <string.h>
#include <cmath>
#include "digitalfiltering.h"

namespace {
void stepTowards(double &value, double goal, double step) {
    if (value < goal) {
        if ((value + step) < goal) {
            value += step;
        } else {
            value = goal;
        }
    } else if (value > goal) {
        if ((value - step) > goal) {
            value -= step;
        } else {
            value = goal;
        }
    }
}
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mPort = new SerialPort(this);

    mTimer = new QTimer(this);
    mTimer->setInterval(20);
    mTimer->start();

    mStatusLabel = new QLabel(this);
    ui->statusBar->addPermanentWidget(mStatusLabel);
    mSampleInt = 0;
    mDoReplot = false;
    mDoRescale = true;
    mDoFilterReplot = true;
    mPacketInterface = new PacketInterface(this);
    mRealtimeGraphsAdded = false;
    keyLeft = false;
    keyRight = false;

    connect(mPort, SIGNAL(serial_data_available()),
            this, SLOT(serialDataAvailable()));
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));

    connect(mPacketInterface, SIGNAL(dataToSend(QByteArray&)),
            this, SLOT(packetDataToSend(QByteArray&)));
    connect(mPacketInterface, SIGNAL(carValuesReceived(PacketInterface::MC_VALUES)),
            this, SLOT(mcValuesReceived(PacketInterface::MC_VALUES)));
    connect(mPacketInterface, SIGNAL(carPrintReceived(QString)),
            this, SLOT(printReceived(QString)));
    connect(mPacketInterface, SIGNAL(carSamplesReceived(QByteArray)),
            this, SLOT(samplesReceived(QByteArray)));

    ui->currentPlot->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    ui->currentPlot->setRangeZoom(Qt::Horizontal | Qt::Vertical);

    ui->voltagePlot->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    ui->voltagePlot->setRangeZoom(Qt::Horizontal | Qt::Vertical);

    ui->filterPlot->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    ui->filterPlot->setRangeZoom(Qt::Horizontal | Qt::Vertical);

    ui->filterResponsePlot->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    ui->filterResponsePlot->setRangeZoom(Qt::Horizontal | Qt::Vertical);

    ui->filterPlot2->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    ui->filterPlot2->setRangeZoom(Qt::Horizontal | Qt::Vertical);

    ui->filterResponsePlot2->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    ui->filterResponsePlot2->setRangeZoom(Qt::Horizontal | Qt::Vertical);

    ui->realtimePlotTemperature->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    ui->realtimePlotTemperature->setRangeZoom(Qt::Horizontal | Qt::Vertical);

    ui->realtimePlotRest->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    ui->realtimePlotRest->setRangeZoom(Qt::Horizontal | Qt::Vertical);

    qApp->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *object, QEvent *e)
{
    Q_UNUSED(object);

    if (e->type() == QEvent::KeyPress || e->type() == QEvent::KeyRelease)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
        bool isPress = e->type() == QEvent::KeyPress;

        switch(keyEvent->key()) {
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Left:
        case Qt::Key_Right:
            break;

        default:
            return false;
        }

        if(keyEvent->isAutoRepeat()) {
            return true;
        }

        switch(keyEvent->key()) {
        case Qt::Key_Up:
            // Nothing
            break;

        case Qt::Key_Down:
            // Nothing
            break;

        case Qt::Key_Left:
            if (isPress) {
                keyLeft = true;
            } else {
                keyLeft = false;
            }
            break;

        case Qt::Key_Right:
            if (isPress) {
                keyRight = true;
            } else {
                keyRight = false;
            }
            break;

        default:
            break;
        }

        return true;
    }

    return false;
}

bool MainWindow::setDytyCycle(double duty)
{
    QByteArray data;
    data.append((char)0);
    data.append((char)2);
    qint16 value = (qint16)(duty * 1000.0);
    data.append((char)(value >> 8));
    data.append((char)value);
    return mPacketInterface->sendPacket(data);
}

void MainWindow::serialDataAvailable()
{
    while (mPort->bytesAvailable() > 0) {
        QByteArray data = mPort->readAll();
        mPacketInterface->processData(data);
    }
}

void MainWindow::timerSlot()
{
    // Update status label
    if (mPort->isOpen()) {
        mStatusLabel->setText("Connected");
    } else {
        mStatusLabel->setText("Not connected");
    }

    // Update MC readings
    if (ui->realtimeActivateBox->isChecked() && mPort->isOpen()) {
        static int div_cnt = 0;
        div_cnt++;

        if (div_cnt >= 1) {
            div_cnt = 0;
            mPacketInterface->readValues();
        }
    }

    // Handle key events
    static double keyPower = 0.0;
    static double lastKeyPower = 0.0;
    const double lowPower = 0.18;
    const double lowPowerRev = 0.1;
    const double highPower = 0.9;
    const double highPowerRev = 0.3;
    const double lowStep = 0.02;
    const double highStep = 0.01;

    if (keyRight && keyLeft) {
        if (keyPower >= lowPower) {
            stepTowards(keyPower, highPower, highStep);
        } else if (keyPower <= -lowPower) {
            stepTowards(keyPower, -highPowerRev, highStep);
        } else if (keyPower >= 0) {
            stepTowards(keyPower, highPower, lowStep);
        } else {
            stepTowards(keyPower, -highPowerRev, lowStep);
        }
    } else if (keyRight) {
        if (fabs(keyPower) > lowPower) {
            stepTowards(keyPower, lowPower, highStep);
        } else {
            stepTowards(keyPower, lowPower, lowStep);
        }
    } else if (keyLeft) {
        if (fabs(keyPower) > lowPower) {
            stepTowards(keyPower, -lowPowerRev, highStep);
        } else {
            stepTowards(keyPower, -lowPowerRev, lowStep);
        }
    } else {
        stepTowards(keyPower, 0.0, lowStep * 3);
    }

    if (keyPower != lastKeyPower) {
        lastKeyPower = keyPower;
        setDytyCycle(keyPower);
        qDebug() << keyPower;
    }

    // Update plots
    static QVector<double> filter;
    static QVector<double> filter2;
    DigitalFiltering f;
    QFont legendFont = font();
    legendFont.setPointSize(9);

    // Plot filters
    if (mDoFilterReplot) {
        if (ui->meanRadioButton->isChecked()) {
            filter.clear();
            int fLen = (1 << ui->currentFilterTapBox->value());
            for (int i = 0;i < fLen;i++) {
                filter.append(1.0 / (double)fLen);
            }
        } else {
            filter = f.generateFirFilter(ui->currentFilterFreqBox->value(),
                                         ui->currentFilterTapBox->value(), ui->hammingBox->isChecked());
        }

        if (ui->meanRadioButton2->isChecked()) {
            filter2.clear();
            int fLen2 = (1 << ui->currentFilterTapBox2->value());
            for (int i = 0;i < fLen2;i++) {
                filter2.append(1.0 / (double)fLen2);
            }
        } else {
            filter2 = f.generateFirFilter(ui->currentFilterFreqBox2->value(),
                                          ui->currentFilterTapBox2->value(), ui->hammingBox2->isChecked());
        }

        static int last_len = 0;
        bool len_diff = last_len != ui->currentFilterTapBox->value();
        last_len = ui->currentFilterTapBox->value();

        static int last_len2 = 0;
        bool len_diff2 = last_len2 != ui->currentFilterTapBox2->value();
        last_len2 = ui->currentFilterTapBox2->value();

        static int last_decimation = 0;
        bool decimation_diff = last_decimation != ui->decimationSpinBox->value();
        last_decimation = ui->decimationSpinBox->value();

        // Plot filter
        QVector<double> filterIndex;
        for(int i = 0;i < filter.size();i++) {
            filterIndex.append((double)i);
        }

        QVector<double> filterIndex2;
        for(int i = 0;i < filter2.size();i++) {
            filterIndex2.append((double)i);
        }

        ui->filterPlot->clearGraphs();
        ui->filterResponsePlot->clearGraphs();
        ui->filterPlot2->clearGraphs();
        ui->filterResponsePlot2->clearGraphs();

        ui->filterPlot->addGraph();
        ui->filterPlot->graph(0)->setData(filterIndex, filter);
        ui->filterPlot->graph(0)->setName("Filter");

        ui->filterPlot2->addGraph();
        ui->filterPlot2->graph(0)->setData(filterIndex2, filter2);
        ui->filterPlot2->graph(0)->setName("Filter");

        if (ui->filterScatterBox->isChecked()) {
            ui->filterPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
            ui->filterPlot->graph(0)->setScatterStyle(QCP::ssCircle);
            ui->filterPlot->graph(0)->setScatterSize(3);
        }

        if (ui->filterScatterBox2->isChecked()) {
            ui->filterPlot2->graph(0)->setLineStyle(QCPGraph::lsLine);
            ui->filterPlot2->graph(0)->setScatterStyle(QCP::ssCircle);
            ui->filterPlot2->graph(0)->setScatterSize(3);
        }

        // Plot response
        QVector<double> response = f.fftWithShift(filter, ui->currentFilterTapBox->value() + 4);
        QVector<double> response2 = f.fftWithShift(filter2, ui->currentFilterTapBox2->value() + 4);

        // Remove positive half
        response.resize(response.size() / 2);
        response2.resize(response2.size() / 2);

        filterIndex.clear();
        for(int i = 0;i < response.size();i++) {
            filterIndex.append(((double)i / (double)response.size()) * (double)ui->sampleFreqBox->value() / 2);
        }

        filterIndex2.clear();
        for(int i = 0;i < response2.size();i++) {
            filterIndex2.append(((double)i / (double)response2.size()) * (double)ui->sampleFreqBox->value() / (double)(2 * ui->decimationSpinBox->value()));
        }

        ui->filterResponsePlot->addGraph();
        ui->filterResponsePlot->graph(0)->setData(filterIndex, response);
        ui->filterResponsePlot->graph(0)->setName("Filter Response");
        ui->filterResponsePlot->graph(0)->setPen(QPen(Qt::red));

        ui->filterResponsePlot2->addGraph();
        ui->filterResponsePlot2->graph(0)->setData(filterIndex2, response2);
        ui->filterResponsePlot2->graph(0)->setName("Filter Response");
        ui->filterResponsePlot2->graph(0)->setPen(QPen(Qt::red));

        ui->filterPlot->legend->setVisible(true);
        ui->filterPlot->legend->setFont(legendFont);
        ui->filterPlot->legend->setPositionStyle(QCPLegend::psBottomRight);
        ui->filterPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
        ui->filterPlot->xAxis->setLabel("Index");
        ui->filterPlot->yAxis->setLabel("Value");

        ui->filterPlot2->legend->setVisible(true);
        ui->filterPlot2->legend->setFont(legendFont);
        ui->filterPlot2->legend->setPositionStyle(QCPLegend::psBottomRight);
        ui->filterPlot2->legend->setBrush(QBrush(QColor(255,255,255,230)));
        ui->filterPlot2->xAxis->setLabel("Index");
        ui->filterPlot2->yAxis->setLabel("Value");

        ui->filterResponsePlot->legend->setVisible(true);
        ui->filterResponsePlot->legend->setFont(legendFont);
        ui->filterResponsePlot->legend->setPositionStyle(QCPLegend::psBottomRight);
        ui->filterResponsePlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
        ui->filterResponsePlot->xAxis->setLabel("Frequency (Hz)");
        ui->filterResponsePlot->yAxis->setLabel("Gain");

        ui->filterResponsePlot2->legend->setVisible(true);
        ui->filterResponsePlot2->legend->setFont(legendFont);
        ui->filterResponsePlot2->legend->setPositionStyle(QCPLegend::psBottomRight);
        ui->filterResponsePlot2->legend->setBrush(QBrush(QColor(255,255,255,230)));
        ui->filterResponsePlot2->xAxis->setLabel("Frequency (Hz)");
        ui->filterResponsePlot2->yAxis->setLabel("Gain");

        if (len_diff) {
            ui->filterPlot->rescaleAxes();
            ui->filterResponsePlot->rescaleAxes();
        }

        if (len_diff2) {
            ui->filterPlot2->rescaleAxes();
            ui->filterResponsePlot2->rescaleAxes();
        }

        if (decimation_diff) {
            ui->filterResponsePlot2->rescaleAxes();
        }

        ui->filterPlot->replot();
        ui->filterResponsePlot->replot();

        ui->filterPlot2->replot();
        ui->filterResponsePlot2->replot();
        mDoFilterReplot = false;
    }

    if (mDoReplot) {
        const double f_samp = (ui->sampleFreqBox->value() / ui->sampleIntBox->value());

        int size = curr1Array.size();
        double last_3 = -10;
        double rpm_3 = -10;

        QVector<double> curr1(size/2);
        for (int i=0; i<(size); i+=2)
        {
            curr1[i/2] = (double)((qint16)(((unsigned char)curr1Array[i] << 8) | (unsigned char)curr1Array[i + 1]));
        }

        QVector<double> curr2(size/2);
        for (int i=0; i<(size); i+=2)
        {
            curr2[i/2] = (double)((qint16)(((unsigned char)curr2Array[i] << 8) | (unsigned char)curr2Array[i + 1]));
        }

        QVector<double> ph1(size/2);
        for (int i=0; i<(size); i+=2)
        {
            ph1[i/2] = (double)((quint8)ph1Array.at(i))*256 + (double)((quint8)ph1Array.at(i+1));
        }

        QVector<double> ph2(size/2);
        for (int i=0; i<(size); i+=2)
        {
            ph2[i/2] = (double)((quint8)ph2Array.at(i))*256 + (double)((quint8)ph2Array.at(i+1));
        }

        QVector<double> ph3(size/2);
        for (int i=0; i<(size); i+=2)
        {
            ph3[i/2] = (double)((quint8)ph3Array.at(i))*256 + (double)((quint8)ph3Array.at(i+1));
        }

        QVector<double> vZero(size/2);
        for (int i=0; i<(size); i+=2)
        {
            vZero[i/2] = (double)((quint8)vZeroArray.at(i))*256 + (double)((quint8)vZeroArray.at(i+1));
        }

        QVector<double> position(size/2);
        for (int i=0;i < (size / 2);i++)
        {
            position[i] = (double)((quint8)statusArray.at(i) & 7);
            if (i > 0 && position[i] == 3 && position[i - 1] != 3) {
                if (last_3 < 0) {
                    last_3 = i;
                } else if (rpm_3 < 0) {
                    // Calculate RPM
                    double diff = fabs(last_3 - i);
                    rpm_3 = (1 / (diff / f_samp)) * 60.0;
                    ui->rpmLcdNumber->display(rpm_3);
                }
            }
        }

        QVector<double> position_hall(size/2);
        for (int i=0;i < (size / 2);i++)
        {
            position_hall[i] = (double)((quint8)(statusArray.at(i) >> 3) & 7) / 1.0;
        }

        QVector<double> totCurrentMc(size/2);
        for (int i=0; i<(size); i+=2)
        {
            totCurrentMc[i/2] = (double)((qint16)(((unsigned char)currTotArray[i] << 8) | (unsigned char)currTotArray[i + 1]));
            totCurrentMc[i/2] /= 100;
        }

        // Calculate current on phases and voltages
        QVector<double> curr3(curr2.size());
        QVector<double> totCurrent(curr2.size());

        for (int i=0;i < curr2.size(); i++)
        {
            curr1[i] *= (3.3 / 4095.0) / (0.001 * 10.0);
            curr2[i] *= (3.3 / 4095.0) / (0.001 * 10.0);
            curr3[i] = -(curr1[i] + curr2[i]);

            const double v_fact = (3.3 / 4095.0) * ((33000.0 + 2200.0) / 2200.0);
            ph1[i] *= v_fact;
            ph2[i] *= v_fact;
            ph3[i] *= v_fact;
            vZero[i] *= v_fact;

            if (ui->truncateBox->isChecked()) {
                if (!(position[i] == 1 || position[i] == 4)) {
                    ph1[i] = 0;
                }

                if (!(position[i] == 2 || position[i] == 5)) {
                    ph2[i] = 0;
                }

                if (!(position[i] == 3 || position[i] == 6)) {
                    ph3[i] = 0;
                }
            }

            int direction = 1;
            switch ((int)position[i]) {
            case 1:
            case 6:
                if (direction) {
                    totCurrent[i] = curr3[i];
                } else {
                    totCurrent[i] = curr2[i];
                }
                break;

            case 2:
            case 3:
                totCurrent[i] = curr1[i];
                break;

            case 4:
            case 5:
                if (direction) {
                    totCurrent[i] = curr2[i];
                } else {
                    totCurrent[i] = curr3[i];
                }
                break;
            }
        }

        // Filter currents
        if (ui->currentFilterActiveBox->isChecked()) {
            curr1 = f.filterSignal(curr1, filter, ui->compDelayBox->isChecked());
            curr2 = f.filterSignal(curr2, filter, ui->compDelayBox->isChecked());
            curr3 = f.filterSignal(curr3, filter, ui->compDelayBox->isChecked());
            totCurrent = f.filterSignal(totCurrent, filter, ui->compDelayBox->isChecked());
        }

        // Apply second filter
        int decimation = 1;
        if (ui->currentFilterActiveBox2->isChecked()) {
            decimation = ui->decimationSpinBox->value();

            QVector<double> currDec1, currDec2, currDec3, totCurrentDec;
            for (int i = 0;i < curr1.size();i++) {
                if (i % decimation == 0) {
                    currDec1.append(curr1[i]);
                    currDec2.append(curr2[i]);
                    currDec3.append(curr3[i]);
                    totCurrentDec.append(totCurrent[i]);
                }
            }

            curr1 = f.filterSignal(currDec1, filter2, ui->compDelayBox->isChecked());
            curr2 = f.filterSignal(currDec2, filter2, ui->compDelayBox->isChecked());
            curr3 = f.filterSignal(currDec3, filter2, ui->compDelayBox->isChecked());
            totCurrent = f.filterSignal(totCurrentDec, filter2, ui->compDelayBox->isChecked());
        }

        static bool lastSpectrum = false;
        bool spectrumChanged = ui->currentSpectrumButton->isChecked() != lastSpectrum;
        lastSpectrum = ui->currentSpectrumButton->isChecked();

        // Filtered x-axis vector for currents
        QVector<double> xAxisCurrDec;
        QVector<double> xAxisCurr;

        // Use DFT
        if (ui->currentSpectrumButton->isChecked()) {
            int fftBits = 16;

            curr1 = f.fftWithShift(curr1, fftBits, true);
            curr2 = f.fftWithShift(curr2, fftBits, true);
            curr3 = f.fftWithShift(curr3, fftBits, true);
            totCurrent = f.fftWithShift(totCurrent, fftBits, true);
            totCurrentMc = f.fftWithShift(totCurrentMc, fftBits, true);

            curr1.resize(curr1.size() / 2);
            curr2.resize(curr2.size() / 2);
            curr3.resize(curr3.size() / 2);
            totCurrent.resize(totCurrent.size() / 2);
            totCurrentMc.resize(totCurrentMc.size() / 2);

            // Resize x-axis
            xAxisCurrDec.resize(curr1.size());
            xAxisCurr.resize(totCurrentMc.size());

            // Generate Filtered X-axis
            for (int i = 0;i < xAxisCurrDec.size();i++) {
                xAxisCurrDec[i] = ((double)i / (double)xAxisCurrDec.size()) * (f_samp / (2 * decimation));
            }

            for (int i = 0;i < xAxisCurr.size();i++) {
                xAxisCurr[i] = ((double)i / (double)xAxisCurr.size()) * (f_samp / 2);
            }
        } else {
            // Resize x-axis
            xAxisCurrDec.resize(curr1.size());
            xAxisCurr.resize(totCurrentMc.size());

            // Generate X axis
            for (int i = 0;i < xAxisCurrDec.size();i++) {
                xAxisCurrDec[i] = ((double)i * (double)decimation) / f_samp;
            }

            for (int i = 0;i < xAxisCurr.size();i++) {
                xAxisCurr[i] = (double)i / f_samp;
            }
        }

        QVector<double> xAxisVolt(ph1.size());
        for (int i = 0;i < xAxisVolt.size();i++) {
            xAxisVolt[i] = (double)i / f_samp;
        }

        ui->currentPlot->clearGraphs();
        ui->voltagePlot->clearGraphs();

        QPen phasePen;
        phasePen.setStyle(Qt::DotLine);
        phasePen.setColor(Qt::darkMagenta);
        phasePen.setWidth(2);

        QPen phasePen2;
        phasePen2.setStyle(Qt::DotLine);
        phasePen2.setColor(Qt::darkYellow);
        phasePen2.setWidth(2);

        int graphIndex = 0;

        if (ui->showCurrent1Box->isChecked()) {
            ui->currentPlot->addGraph();
            ui->currentPlot->graph(graphIndex)->setPen(QPen(Qt::magenta));
            ui->currentPlot->graph(graphIndex)->setData(xAxisCurrDec, curr1);
            ui->currentPlot->graph(graphIndex)->setName("Phase 1 Current");
            graphIndex++;
        }

        if (ui->showCurrent2Box->isChecked()) {
            ui->currentPlot->addGraph();
            ui->currentPlot->graph(graphIndex)->setData(xAxisCurrDec, curr2);
            ui->currentPlot->graph(graphIndex)->setPen(QPen(Qt::red));
            ui->currentPlot->graph(graphIndex)->setName("Phase 2 Current");
            graphIndex++;
        }

        if (ui->showCurrent3Box->isChecked()) {
            ui->currentPlot->addGraph();
            ui->currentPlot->graph(graphIndex)->setData(xAxisCurrDec, curr3);
            ui->currentPlot->graph(graphIndex)->setPen(QPen(Qt::green));
            ui->currentPlot->graph(graphIndex)->setName("Phase 3 Current");
            graphIndex++;
        }

        if (ui->showTotalCurrentBox->isChecked()) {
            ui->currentPlot->addGraph();
            ui->currentPlot->graph(graphIndex)->setData(xAxisCurrDec, totCurrent);
            ui->currentPlot->graph(graphIndex)->setPen(QPen(Qt::darkCyan));
            ui->currentPlot->graph(graphIndex)->setName("Total current");
            graphIndex++;
        }

        if (ui->showMcTotalCurrentBox->isChecked()) {
            ui->currentPlot->addGraph();
            ui->currentPlot->graph(graphIndex)->setData(xAxisCurr, totCurrentMc);
            ui->currentPlot->graph(graphIndex)->setPen(QPen(Qt::blue));
            ui->currentPlot->graph(graphIndex)->setName("Total current filtered by MC");
            graphIndex++;
        }

        if (ui->showPosCurrentBox->isChecked() && !ui->currentSpectrumButton->isChecked()) {
            ui->currentPlot->addGraph();
            ui->currentPlot->graph(graphIndex)->setData(xAxisCurr, position);
            ui->currentPlot->graph(graphIndex)->setPen(phasePen);
            ui->currentPlot->graph(graphIndex)->setName("Current position");
            graphIndex++;
        }

        graphIndex = 0;

        if (ui->showPh1Box->isChecked()) {
            ui->voltagePlot->addGraph();
            ui->voltagePlot->graph(graphIndex)->setData(xAxisVolt, ph1);
            ui->voltagePlot->graph(graphIndex)->setPen(QPen(Qt::blue));
            ui->voltagePlot->graph(graphIndex)->setName("Phase 1 voltage");
            graphIndex++;
        }

        if (ui->showPh2Box->isChecked()) {
            ui->voltagePlot->addGraph();
            ui->voltagePlot->graph(graphIndex)->setData(xAxisVolt, ph2);
            ui->voltagePlot->graph(graphIndex)->setPen(QPen(Qt::red));
            ui->voltagePlot->graph(graphIndex)->setName("Phase 2 voltage");
            graphIndex++;
        }

        if (ui->showPh3Box->isChecked()) {
            ui->voltagePlot->addGraph();
            ui->voltagePlot->graph(graphIndex)->setData(xAxisVolt, ph3);
            ui->voltagePlot->graph(graphIndex)->setPen(QPen(Qt::green));
            ui->voltagePlot->graph(graphIndex)->setName("Phase 3 voltage");
            graphIndex++;
        }

        if (ui->showVirtualGndBox->isChecked()) {
            ui->voltagePlot->addGraph();
            ui->voltagePlot->graph(graphIndex)->setData(xAxisVolt, vZero);
            ui->voltagePlot->graph(graphIndex)->setPen(QPen(Qt::magenta));
            ui->voltagePlot->graph(graphIndex)->setName("Virtual ground");
            graphIndex++;
        }

        if (ui->showPosVoltageBox->isChecked()) {
            ui->voltagePlot->addGraph();
            ui->voltagePlot->graph(graphIndex)->setData(xAxisVolt, position);
            ui->voltagePlot->graph(graphIndex)->setPen(phasePen);
            ui->voltagePlot->graph(graphIndex)->setName("Current position");
            graphIndex++;

            ui->voltagePlot->addGraph();
            ui->voltagePlot->graph(graphIndex)->setData(xAxisVolt, position_hall);
            ui->voltagePlot->graph(graphIndex)->setPen(phasePen2);
            ui->voltagePlot->graph(graphIndex)->setName("Hall position");
            graphIndex++;
        }

        // Plot settings
        ui->currentPlot->legend->setVisible(true);
        ui->currentPlot->legend->setFont(legendFont);
        ui->currentPlot->legend->setPositionStyle(QCPLegend::psBottomRight);
        ui->currentPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
        if (ui->currentSpectrumButton->isChecked()) {
            ui->currentPlot->xAxis->setLabel("Frequency (Hz)");
            ui->currentPlot->yAxis->setLabel("Amplitude");
        } else {
            ui->currentPlot->xAxis->setLabel("Seconds (s)");
            ui->currentPlot->yAxis->setLabel("Amperes (A)");
        }

        ui->voltagePlot->legend->setVisible(true);
        ui->voltagePlot->legend->setFont(legendFont);
        ui->voltagePlot->legend->setPositionStyle(QCPLegend::psBottomRight);
        ui->voltagePlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
        ui->voltagePlot->xAxis->setLabel("Seconds (s)");
        ui->voltagePlot->yAxis->setLabel("Volts (V)");

        if (mDoRescale || spectrumChanged) {
            ui->currentPlot->rescaleAxes();
            ui->voltagePlot->rescaleAxes();
        }

        ui->currentPlot->replot();
        ui->voltagePlot->replot();

        mDoReplot = false;
        mDoRescale = false;
    }
}

void MainWindow::packetDataToSend(QByteArray &data)
{
    if (mPort->isOpen()) {
        mPort->writeData(data.data(), data.size());
    }
}

void MainWindow::mcValuesReceived(PacketInterface::MC_VALUES values)
{
    const int maxS = 500;

    appendDoubleAndTrunc(&tempMos1Vec, values.temp_mos1, maxS);
    appendDoubleAndTrunc(&tempMos2Vec, values.temp_mos2, maxS);
    appendDoubleAndTrunc(&tempMos3Vec, values.temp_mos3, maxS);
    appendDoubleAndTrunc(&tempMos4Vec, values.temp_mos4, maxS);
    appendDoubleAndTrunc(&tempMos5Vec, values.temp_mos5, maxS);
    appendDoubleAndTrunc(&tempMos6Vec, values.temp_mos6, maxS);
    appendDoubleAndTrunc(&tempPcbVec, values.temp_pcb, maxS);
    appendDoubleAndTrunc(&currInVec, values.current_in, maxS);
    appendDoubleAndTrunc(&currMotorVec, values.current_motor, maxS);
    appendDoubleAndTrunc(&dutyVec, values.duty_now, maxS);
    appendDoubleAndTrunc(&rpmVec, values.rpm, maxS);
    appendDoubleAndTrunc(&voltInVec, values.v_in, maxS);

    QPen dotPen;
    dotPen.setStyle(Qt::DotLine);
    dotPen.setColor(Qt::blue);
    dotPen.setWidth(2);

    QFont legendFont = font();
    legendFont.setPointSize(9);

    int dataSize = tempMos1Vec.size();
    float fsamp = 10.0;

    QVector<double> xAxis(dataSize);
    for (int i = 0;i < tempMos1Vec.size();i++) {
        xAxis[i] = (double)i / fsamp;
    }

    int graphIndex = 0;

    if (!mRealtimeGraphsAdded) {
        ui->realtimePlotTemperature->addGraph();
        ui->realtimePlotTemperature->graph(graphIndex)->setPen(QPen(Qt::blue));
        ui->realtimePlotTemperature->graph(graphIndex)->setName("Temperature MOSFET 1");
        graphIndex++;

        ui->realtimePlotTemperature->addGraph();
        dotPen.setColor(Qt::blue);
        ui->realtimePlotTemperature->graph(graphIndex)->setPen(dotPen);
        ui->realtimePlotTemperature->graph(graphIndex)->setName("Temperature MOSFET 2");
        graphIndex++;

        ui->realtimePlotTemperature->addGraph();
        ui->realtimePlotTemperature->graph(graphIndex)->setPen(QPen(Qt::red));
        ui->realtimePlotTemperature->graph(graphIndex)->setName("Temperature MOSFET 3");
        graphIndex++;

        ui->realtimePlotTemperature->addGraph();
        dotPen.setColor(Qt::red);
        ui->realtimePlotTemperature->graph(graphIndex)->setPen(dotPen);
        ui->realtimePlotTemperature->graph(graphIndex)->setName("Temperature MOSFET 4");
        graphIndex++;

        ui->realtimePlotTemperature->addGraph();
        ui->realtimePlotTemperature->graph(graphIndex)->setPen(QPen(Qt::green));
        ui->realtimePlotTemperature->graph(graphIndex)->setName("Temperature MOSFET 5");
        graphIndex++;

        ui->realtimePlotTemperature->addGraph();
        dotPen.setColor(Qt::green);
        ui->realtimePlotTemperature->graph(graphIndex)->setPen(dotPen);
        ui->realtimePlotTemperature->graph(graphIndex)->setName("Temperature MOSFET 6");
        graphIndex++;

        ui->realtimePlotTemperature->addGraph();
        ui->realtimePlotTemperature->graph(graphIndex)->setPen(QPen(Qt::magenta));
        ui->realtimePlotTemperature->graph(graphIndex)->setName("Temperature PCB");
        graphIndex++;

        // Next plot
        graphIndex = 0;
        ui->realtimePlotRest->addGraph();
        ui->realtimePlotRest->graph(graphIndex)->setPen(QPen(Qt::blue));
        ui->realtimePlotRest->graph(graphIndex)->setName("Current in");
        graphIndex++;

        ui->realtimePlotRest->addGraph();
        ui->realtimePlotRest->graph(graphIndex)->setPen(QPen(Qt::red));
        ui->realtimePlotRest->graph(graphIndex)->setName("Current motor");
        graphIndex++;

        ui->realtimePlotRest->addGraph(ui->realtimePlotRest->xAxis, ui->realtimePlotRest->yAxis2);
        ui->realtimePlotRest->graph(graphIndex)->setPen(QPen(Qt::green));
        ui->realtimePlotRest->graph(graphIndex)->setName("Duty cycle");
        graphIndex++;

        ui->realtimePlotTemperature->legend->setVisible(true);
        ui->realtimePlotTemperature->legend->setFont(legendFont);
        ui->realtimePlotTemperature->legend->setPositionStyle(QCPLegend::psBottomRight);
        ui->realtimePlotTemperature->legend->setBrush(QBrush(QColor(255,255,255,230)));
        ui->realtimePlotTemperature->xAxis->setLabel("Seconds (s)");
        ui->realtimePlotTemperature->yAxis->setLabel("Deg C");

        ui->realtimePlotRest->legend->setVisible(true);
        ui->realtimePlotRest->legend->setFont(legendFont);
        ui->realtimePlotRest->legend->setPositionStyle(QCPLegend::psBottomRight);
        ui->realtimePlotRest->legend->setBrush(QBrush(QColor(255,255,255,230)));
        ui->realtimePlotRest->xAxis->setLabel("Seconds (s)");
        ui->realtimePlotRest->yAxis->setLabel("Ampere (A)");
        ui->realtimePlotRest->yAxis2->setLabel("Duty Cycle");

        ui->realtimePlotTemperature->yAxis->setRange(0, 120);
        ui->realtimePlotRest->yAxis->setRange(-20, 130);
        ui->realtimePlotRest->yAxis2->setRange(-0.2, 1.3);
        ui->realtimePlotRest->yAxis2->setVisible(true);

        mRealtimeGraphsAdded = true;
    }

    // Set data
    graphIndex = 0;
    ui->realtimePlotTemperature->graph(graphIndex++)->setData(xAxis, tempMos1Vec);
    ui->realtimePlotTemperature->graph(graphIndex++)->setData(xAxis, tempMos2Vec);
    ui->realtimePlotTemperature->graph(graphIndex++)->setData(xAxis, tempMos3Vec);
    ui->realtimePlotTemperature->graph(graphIndex++)->setData(xAxis, tempMos4Vec);
    ui->realtimePlotTemperature->graph(graphIndex++)->setData(xAxis, tempMos5Vec);
    ui->realtimePlotTemperature->graph(graphIndex++)->setData(xAxis, tempMos6Vec);
    ui->realtimePlotTemperature->graph(graphIndex++)->setData(xAxis, tempPcbVec);

    // Next plot
    graphIndex = 0;
    ui->realtimePlotRest->graph(graphIndex++)->setData(xAxis, currInVec);
    ui->realtimePlotRest->graph(graphIndex++)->setData(xAxis, currMotorVec);
    ui->realtimePlotRest->graph(graphIndex++)->setData(xAxis, dutyVec);

    if (ui->realtimeAutoScaleBox->isChecked()) {
        ui->realtimePlotTemperature->rescaleAxes();
        ui->realtimePlotRest->rescaleAxes();
    }

    if (dataSize < maxS) {
        ui->realtimePlotTemperature->xAxis->setRange(0, dataSize / fsamp);
    }

    if (dataSize < maxS) {
        ui->realtimePlotRest->xAxis->setRange(0, dataSize / fsamp);
    }

    ui->realtimePlotTemperature->replot();
    ui->realtimePlotRest->replot();
}

void MainWindow::printReceived(QString str)
{
    ui->terminalBrowser->append(str);
}

void MainWindow::samplesReceived(QByteArray data)
{
    for (int i = 0;i < data.size();i++) {
        if (mSampleInt == 0 || mSampleInt == 1) {
            tmpCurr1Array.append(data[i]);
        } else if (mSampleInt == 2 || mSampleInt == 3) {
            tmpCurr2Array.append(data[i]);
        } else if (mSampleInt == 4 || mSampleInt == 5) {
            tmpPh1Array.append(data[i]);
        } else if (mSampleInt == 6 || mSampleInt == 7) {
            tmpPh2Array.append(data[i]);
        } else if (mSampleInt == 8 || mSampleInt == 9) {
            tmpPh3Array.append(data[i]);
        } else if (mSampleInt == 10 || mSampleInt == 11) {
            tmpVZeroArray.append(data[i]);
        } else if (mSampleInt == 12) {
            tmpStatusArray.append(data[i]);
        } else if (mSampleInt == 13 || mSampleInt == 14) {
            tmpCurrTotArray.append(data[i]);
        }

        mSampleInt++;
        if (mSampleInt == 15) {
            mSampleInt = 0;
        }

        if (tmpCurr1Array.size() == (ui->sampleNumBox->value() * 2)) {
            curr1Array = tmpCurr1Array;
            curr2Array = tmpCurr2Array;
            ph1Array = tmpPh1Array;
            ph2Array = tmpPh2Array;
            ph3Array = tmpPh3Array;
            vZeroArray = tmpVZeroArray;
            statusArray = tmpStatusArray;
            currTotArray = tmpCurrTotArray;
            mDoReplot = true;
            mDoFilterReplot = true;
            mDoRescale = true;
        }
    }
}

void MainWindow::on_connectButton_clicked()
{
    mPort->openPort(ui->serialDeviceEdit->text());
}

void MainWindow::on_disconnectButton_clicked()
{
    mPort->closePort();
}

void MainWindow::on_getDataButton_clicked()
{
    clearBuffers();

    QByteArray data;
    data.append((char)0);
    data.append((char)1);
    int value = ui->sampleNumBox->value();
    data.append((char)(value >> 8));
    data.append((char)value);
    data.append((char)ui->sampleIntBox->value());
    mPacketInterface->sendPacket(data);
}

void MainWindow::on_getDataStartButton_clicked()
{
    clearBuffers();

    QByteArray data;
    data.append((char)0);
    data.append((char)6);
    int value = ui->sampleNumBox->value();
    data.append((char)(value >> 8));
    data.append((char)value);
    data.append((char)ui->sampleIntBox->value());
    mPacketInterface->sendPacket(data);
}

void MainWindow::on_dutyButton_clicked()
{
    setDytyCycle(ui->dutyBox->value());
}

void MainWindow::on_rpmButton_clicked()
{
    QByteArray data;
    data.append((char)0);
    data.append((char)3);
    qint32 value = ui->rpmBox->value();
    data.append((char)(value >> 24));
    data.append((char)(value >> 16));
    data.append((char)(value >> 8));
    data.append((char)value);
    mPacketInterface->sendPacket(data);
}

void MainWindow::on_offButton_clicked()
{
    QByteArray data;
    data.append((char)0);
    data.append((char)2);
    qint16 value = 0;
    data.append((char)(value >> 8));
    data.append((char)value);
    mPacketInterface->sendPacket(data);
}

void MainWindow::clearBuffers()
{
    mSampleInt = 0;
    tmpCurr1Array.clear();
    tmpCurr2Array.clear();
    tmpPh1Array.clear();
    tmpPh2Array.clear();
    tmpPh3Array.clear();
    tmpVZeroArray.clear();
    tmpStatusArray.clear();
    tmpCurrTotArray.clear();
}

void MainWindow::on_replotButton_clicked()
{
    mDoReplot = true;
    mDoFilterReplot = true;
}

void MainWindow::on_rescaleButton_clicked()
{
    ui->currentPlot->rescaleAxes();
    ui->currentPlot->replot();

    ui->voltagePlot->rescaleAxes();
    ui->voltagePlot->replot();

    ui->filterPlot->rescaleAxes();
    ui->filterPlot->replot();

    ui->filterResponsePlot->rescaleAxes();
    ui->filterResponsePlot->replot();

    ui->filterResponsePlot2->rescaleAxes();
    ui->filterResponsePlot2->replot();

    ui->realtimePlotTemperature->rescaleAxes();
    ui->realtimePlotTemperature->replot();

    ui->realtimePlotRest->rescaleAxes();
    ui->realtimePlotRest->replot();
}

void MainWindow::on_horizontalZoomBox_clicked()
{
    Qt::Orientations orient;

    if (ui->horizontalZoomBox->isChecked()) {
        orient |= Qt::Horizontal;
    }

    if (ui->verticalZoomBox->isChecked()) {
        orient |= Qt::Vertical;
    }

    ui->currentPlot->setRangeZoom(orient);
    ui->voltagePlot->setRangeZoom(orient);
    ui->filterPlot->setRangeZoom(orient);
    ui->filterResponsePlot->setRangeZoom(orient);
    ui->filterPlot2->setRangeZoom(orient);
    ui->filterResponsePlot2->setRangeZoom(orient);
    ui->realtimePlotTemperature->setRangeZoom(orient);
    ui->realtimePlotRest->setRangeZoom(orient);
}

void MainWindow::on_verticalZoomBox_clicked()
{
    on_horizontalZoomBox_clicked();
}

void MainWindow::on_filterLogScaleBox_clicked(bool checked)
{
    if (checked) {
        ui->filterResponsePlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    } else {
        ui->filterResponsePlot->yAxis->setScaleType(QCPAxis::stLinear);
    }

    ui->filterResponsePlot->rescaleAxes();
    ui->filterResponsePlot->replot();
}

void MainWindow::on_filterLogScaleBox2_clicked(bool checked)
{
    if (checked) {
        ui->filterResponsePlot2->yAxis->setScaleType(QCPAxis::stLogarithmic);
    } else {
        ui->filterResponsePlot2->yAxis->setScaleType(QCPAxis::stLinear);
    }

    ui->filterResponsePlot2->rescaleAxes();
    ui->filterResponsePlot2->replot();
}

void MainWindow::on_detectButton_clicked()
{
    QByteArray data;
    data.append((char)0);
    data.append((char)4);
    mPacketInterface->sendPacket(data);
}

void MainWindow::on_resetBufferButton_clicked()
{
    clearBuffers();
}

void MainWindow::appendDoubleAndTrunc(QVector<double> *vec, double num, int maxSize)
{
    vec->append(num);

    if(vec->size() > maxSize) {
        vec->remove(0, vec->size() - maxSize);
    }
}

void MainWindow::on_clearTerminalButton_clicked()
{
    ui->terminalBrowser->clear();
}

void MainWindow::on_resetFaultButton_clicked()
{
    QByteArray data;
    data.append((char)0);
    data.append((char)5);
    mPacketInterface->sendPacket(data);
}

void MainWindow::on_sendTerminalButton_clicked()
{
    mPacketInterface->sendTerminalCmd(ui->terminalEdit->text());
    ui->terminalEdit->clear();
}
