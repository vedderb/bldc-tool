#ifndef QISYSTEMMESSENGER_H
#define QISYSTEMMESSENGER_H

#include <QObject>
#include <QVariantMap>

/// QISystemMessenger provides an simple async messaging interface between C/C++/QML and Objective-C source code.
/**
 */

class QISystemMessenger : public QObject {
    Q_OBJECT

public:
    static QISystemMessenger* instance();

    /// Deliver a message
    /** If there has a registered helper function , it will return TRUE. Otherwise, it will return FALSE.
     *
     * After processed by the registered helper, the "received" signal will be emitted
     * in next tick of event loop.
     */
    Q_INVOKABLE bool sendMessage(QString name , QVariantMap data);

    /// Register a message handler.
    /**
     * @brief registerMessageHandler
     * @param name
     * @return TRUE if it is successfully. If it is already registered, it will return false.
     */
    bool registerMessageHandler(QString name,bool (*func)(QVariantMap&) );

    /// Return TRUE if it there has a message helper registered already.
    bool contins(QString name);

signals:
    /// The signal is emitted when a message is received.
    void received(QString name , QVariantMap data);

private:
    explicit QISystemMessenger(QObject* parent = 0);

};

#endif // QISYSTEMUTILS_H
