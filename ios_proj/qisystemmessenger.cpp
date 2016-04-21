#include <QCoreApplication>
#include <QPointer>
#include <QtCore>
#include "qisystemmessenger.h"

typedef bool (*handler)(QVariantMap& data);
static QMap<QString,handler> handlers;
static QPointer<QISystemMessenger> m_instance;

QISystemMessenger *QISystemMessenger::instance()
{
    if (!m_instance) {
        QCoreApplication* app = QCoreApplication::instance();
        m_instance = new QISystemMessenger(app);
    }
    return m_instance;
}

QISystemMessenger::QISystemMessenger(QObject *parent) : QObject(parent) {
}

bool QISystemMessenger::sendMessage(QString name , QVariantMap data) {

    QMetaObject::invokeMethod(this,"received",Qt::QueuedConnection,
                              Q_ARG(QString , name),
                              Q_ARG(QVariantMap,data));

    bool res = false;
    if (handlers.contains(name)) {
        res = handlers[name](data);
    }

    return res;
}

bool QISystemMessenger::registerMessageHandler(QString name, bool (*func)(QVariantMap&))
{
    if (handlers.contains(name)) {
        qWarning() << QString("%s is already registered").arg(name);
        return false;
    }

    handlers[name] = func;
    return true;
}
