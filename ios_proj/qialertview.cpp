#include "qialertview.h"
#include "qisystemmessenger.h"
#ifndef Q_OS_IOS

#endif

/*! \qmltype AlertView
 * \inqmlmodule QuickIOS
 */

QIAlertView::QIAlertView(QQuickItem *parent) :
    QQuickItem(parent)
{
    m_buttons << tr("Cancel") << tr("OK");
    m_opened = false;
    m_clickedButtonIndex = -1;
}

QIAlertView::~QIAlertView()
{
#ifndef Q_OS_IOS
    if (!dialog.isNull()) {
        dialog->reject();
    }
#endif
}

QString QIAlertView::title() const {
    return m_title;
}


QString QIAlertView::message() const
{
    return m_message;
}

void QIAlertView::setTitle(const QString &arg)
{
    m_title = arg;
    emit titleChanged();
}


void QIAlertView::setMessage(const QString &arg)
{
    m_message = arg;
    emit messageChanged();
}


void QIAlertView::show()
{
    if (m_opened)
        return;

#ifdef Q_OS_IOS
    QISystemMessenger* system = QISystemMessenger::instance();

    QVariantMap data;
    data["title"] = m_title;
    data["message"] = m_message;
    data["buttons"] = m_buttons;

    m_opened = true;
    connect(system,SIGNAL(received(QString,QVariantMap)),
            this,SLOT(onReceived(QString,QVariantMap)));

    system->sendMessage("alertViewCreate",data);
#else

    dialog = new QMessageBox();

    dialog->setWindowTitle(m_title);
    dialog->setText(m_message);

    for (int i = 0 ; i < m_buttons.size() ; i++) {
        dialog->addButton(m_buttons.at(i),QMessageBox::ActionRole);
    }

    connect(dialog.data(),SIGNAL(finished(int)),
            this,SLOT(onFinished(int)));

    connect(dialog.data(),SIGNAL(finished(int)),
            dialog.data(),SLOT(close()));

    connect(dialog.data(),SIGNAL(finished(int)),
            dialog.data(),SLOT(deleteLater()));

    dialog->show();

#endif
}

/*! \qmlmethod AlertView::dismiss(int clickedButtonIndex, bool animated)

   Dismisses the AlertView, optionally with animation.

   clickedButtonIndex: The index of the button that was clicked just before invoking this method.

   animated: Enable/disable animation
 */

void QIAlertView::dismiss(int clickedButtonIndex, bool animated)
{
    QISystemMessenger* system = QISystemMessenger::instance();

    QVariantMap message;
    message["index"] = clickedButtonIndex;
    message["animated"] = animated;

    system->sendMessage("alertViewDismissWithClickedButtonIndex",message);
}

void QIAlertView::onReceived(QString name, QVariantMap data)
{
    if (name != "alertViewClickedButtonAtIndex"){
        return;
    }

    QISystemMessenger* system = QISystemMessenger::instance();
    system->disconnect(this);

    int buttonIndex = data["buttonIndex"].toInt();
    onFinished(buttonIndex);
}

void QIAlertView::onFinished(int index)
{
    m_opened = false;
    setClickedButtonIndex(index);
    emit clicked(index);
}

int QIAlertView::clickedButtonIndex() const
{
    return m_clickedButtonIndex;
}

void QIAlertView::setClickedButtonIndex(int clickedButtonIndex)
{
    m_clickedButtonIndex = clickedButtonIndex;
    emit clickedButtonIndexChanged();
}


QStringList QIAlertView::buttons() const
{
    return m_buttons;
}

void QIAlertView::setButtons(const QStringList &buttons)
{
    m_buttons = buttons;
}


