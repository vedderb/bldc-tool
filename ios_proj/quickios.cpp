#include <QtQml>
#include <QtGui>
#include <QVariantMap>
#include <QPointer>
#include "quickios.h"
#include "qialertview.h"
#include "qisystemmessenger.h"
#include "qidevice.h"
#include "qiactionsheet.h"
#include "qiactivityindicator.h"

static QJSValue systemProvider(QQmlEngine* engine , QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    QISystemMessenger *system = QISystemMessenger::instance();

    QJSValue value = scriptEngine->newQObject(system);
    return value;
}

static QJSValue deviceProvider(QQmlEngine* engine , QJSEngine *scriptEngine) {
    Q_UNUSED(engine);

    QIDevice* device = QIDevice::instance();

    QScreen *src = QGuiApplication::screens().at(0); // @TODO: Dynamic update
    device->setScreenWidth(src->availableGeometry().width());
    device->setScreenHeight(src->availableGeometry().height());

    QJSValue value = scriptEngine->newQObject(device);
    return value;
}


void QuickIOS::registerTypes()
{
  qmlRegisterSingletonType("QuickIOS", 0, 1, "SystemMessenger", systemProvider);
  qmlRegisterSingletonType("QuickIOS", 0, 1, "QIDevice", deviceProvider);

  qmlRegisterType<QIAlertView>("QuickIOS",0,1,"AlertView");
  qmlRegisterType<QIActionSheet>("QuickIOS",0,1,"ActionSheet");
  qmlRegisterType<QIActivityIndicator>("QuickIOS",0,1,"ActivityIndicator");
}

void QuickIOS::setupWindow(QQuickWindow *window)
{
#ifdef Q_OS_IOS
    QIDevice* device = QIDevice::instance();
    device->setScreenFillStatusBar(true);
    window->showFullScreen();
#else
    Q_UNUSED(window);
#endif
}

void QuickIOS::setStatusBarStyle(QuickIOS::StatusBarStyle style)
{
    QISystemMessenger *system = QISystemMessenger::instance();
    QVariantMap data;
    data["style"] = style;

    system->sendMessage("applicationSetStatusBarStyle",data);
}
