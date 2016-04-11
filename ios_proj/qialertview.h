#ifndef QIALERTVIEW_H
#define QIALERTVIEW_H

#include <QQuickItem>

#ifndef Q_OS_IOS
#include <QMessageBox>
#include <QPointer>
#endif

/// QIAlertView provides an interface to access iOS's UIAlertView

class QIAlertView : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
    Q_PROPERTY(QStringList buttons READ buttons WRITE setButtons NOTIFY buttonsChanged)
    Q_PROPERTY(int clickedButtonIndex READ clickedButtonIndex WRITE setClickedButtonIndex NOTIFY clickedButtonIndexChanged)

public:

    explicit QIAlertView(QQuickItem *parent = 0);
    ~QIAlertView();

    virtual QString title() const;
    QString text() const;
    QString message() const;


    QStringList buttons() const;
    void setButtons(const QStringList &buttons);

    int clickedButtonIndex() const;
    void setClickedButtonIndex(int clickedButtonIndex);

public Q_SLOTS:
    virtual void setTitle(const QString &arg);

    void setMessage(const QString &arg);

    void show();

    void dismiss(int clickedButtonIndex, bool animated = true);

Q_SIGNALS:
    void titleChanged();
    void messageChanged();
    void buttonsChanged();

    void clicked(int buttonIndex);
    void clickedButtonIndexChanged();

private:
    Q_INVOKABLE void onReceived(QString name,QVariantMap data);
    Q_INVOKABLE void onFinished(int index);
    QString m_title;
    QString m_message;

    QStringList m_buttons;
    bool m_opened;
    int m_clickedButtonIndex;

#ifndef Q_OS_IOS
    QPointer<QMessageBox> dialog;
#endif
};

#endif // QIALERTVIEW_H
