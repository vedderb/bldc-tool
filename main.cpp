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

#ifdef QML

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "bldcinterface.h"


#include <QQuickWindow>
#include <QtCore>

#ifdef Q_OS_IOS
#include "qisystemmessenger.h"
#include "quickios.h"
#include "qidevice.h"
#endif


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

#ifdef Q_OS_IOS
    // Quick iOS Initialization
    //engine.addImportPath("qrc:///");
    QuickIOS::registerTypes(); // It must be called before loaded any scene
#endif

    BLDCInterface bldcIntrface;
    engine.rootContext()->setContextObject(&bldcIntrface);


#ifdef Q_OS_IOS
    engine.load(QUrl(QStringLiteral("qrc:/main_ios.qml")));
    QQuickWindow *window = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
    /// Setup the QQuickWindow instance to fit the iOS environment
    QuickIOS::setupWindow(window);
    QuickIOS::setStatusBarStyle(QuickIOS::StatusBarStyleDefault);
    QISystemMessenger::instance()->sendMessage("activityIndicatorStart",QVariantMap());
#else
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
#endif

    return app.exec();
}

#else
#include <QApplication>
#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
#endif
