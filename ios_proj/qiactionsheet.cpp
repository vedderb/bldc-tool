#include "qisystemmessenger.h"
#include "qiactionsheet.h"

QIActionSheet::QIActionSheet(QQuickItem* parent) : QQuickItem(parent)
{
    m_clickedButtonIndex = -1;
    running = false;
    m_cancelButtonTitle = tr("Cancel");
}

QIActionSheet::~QIActionSheet()
{

}

QString QIActionSheet::title() const
{
    return m_title;
}

void QIActionSheet::setTitle(const QString &title)
{
    m_title = title;
    emit titleChanged();
}

QStringList QIActionSheet::otherButtonTitles() const
{
    return m_otherButtonTitles;
}

void QIActionSheet::setOtherButtonTitles(const QStringList &otherButtonTitles)
{
    m_otherButtonTitles = otherButtonTitles;
    emit otherButtonTitlesChanged();
}

void QIActionSheet::show()
{
    if (running)
        return;

    running = true;

    QISystemMessenger* system = QISystemMessenger::instance();

    QVariantMap data;
    data["title"] = m_title;
    data["otherButtonTitles"] = m_otherButtonTitles;
    data["cancelButtonTitle"] = m_cancelButtonTitle;

    QRect rect(x(),y(),width(),height());

    data["sourceRect"] = mapRectToScene(rect);

    running = true;
    connect(system,SIGNAL(received(QString,QVariantMap)),
            this,SLOT(onReceived(QString,QVariantMap)));

    system->sendMessage("actionSheetCreate",data);
}

void QIActionSheet::onReceived(QString name, QVariantMap data)
{
    if (name != "actionSheetClickedButtonAtIndex" &&
        name != "actionSheetDidDismissWithButtonIndex"){
        return;
    }

    int buttonIndex = data["buttonIndex"].toInt();
    if (buttonIndex >= m_otherButtonTitles.size())
        buttonIndex = -1;

    if (name == "actionSheetClickedButtonAtIndex") {
        setClickedButtonIndex(buttonIndex);
        emit clicked(buttonIndex);

    } else {
        running = false;

        QISystemMessenger* system = QISystemMessenger::instance();
        system->disconnect(this);
        emit dismissed(buttonIndex);
    }

}


QString QIActionSheet::cancelButtonTitle() const
{
    return m_cancelButtonTitle;
}

void QIActionSheet::setCancelButtonTitle(const QString &cancelButtonTitle)
{
    m_cancelButtonTitle = cancelButtonTitle;
}


int QIActionSheet::clickedButtonIndex() const
{
    return m_clickedButtonIndex;
}

void QIActionSheet::setClickedButtonIndex(int clickedButton)
{
    m_clickedButtonIndex = clickedButton;
    emit clickedButtonIndexChanged();
}




