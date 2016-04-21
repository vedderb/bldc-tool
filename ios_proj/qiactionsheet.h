#ifndef QIACTIONSHEET_H
#define QIACTIONSHEET_H

#include <QQuickItem>

class QIActionSheet : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QStringList otherButtonTitles READ otherButtonTitles WRITE setOtherButtonTitles NOTIFY otherButtonTitlesChanged)
    Q_PROPERTY(int clickedButtonIndex READ clickedButtonIndex WRITE setClickedButtonIndex NOTIFY clickedButtonIndexChanged)
    Q_PROPERTY(QString cancelButtonTitle READ cancelButtonTitle WRITE setCancelButtonTitle NOTIFY cancelButtonTitleChanged)

public:
    QIActionSheet(QQuickItem* parent = 0);
    ~QIActionSheet();

    QString title() const;
    void setTitle(const QString &title);

    QStringList otherButtonTitles() const;
    void setOtherButtonTitles(const QStringList &otherButtonTitles);


    int clickedButtonIndex() const;
    void setClickedButtonIndex(int clickedButtonIndex);

    Q_INVOKABLE void show();

    QString cancelButtonTitle() const;
    void setCancelButtonTitle(const QString &cancelButtonTitle);

signals:
    void clicked(int buttonIndex);
    void titleChanged();
    void otherButtonTitlesChanged();
    void clickedButtonIndexChanged();
    void cancelButtonTitleChanged();

    /// The signal is emitted when the action sheet is dismissed completely.
    void dismissed(int buttonIndex);

private:
    Q_INVOKABLE void onReceived(QString name,QVariantMap data);

    QString m_title;
    QStringList m_otherButtonTitles;
    QString m_cancelButtonTitle;

    int m_clickedButtonIndex;
    bool running;

};

#endif // QIACTIONSHEET_H
