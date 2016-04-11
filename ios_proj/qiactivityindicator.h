#ifndef QIACTIVITYINDICATOR_H
#define QIACTIVITYINDICATOR_H

#include <QObject>
#include <QQuickItem>

class QIActivityIndicator : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool isAnimating READ isAnimating WRITE setIsAnimating NOTIFY isAnimatingChanged)
    Q_PROPERTY(Style style READ style WRITE setStyle NOTIFY styleChanged)
    Q_ENUMS(Style)

public:
    enum Style {
        WhiteLarge = 0,
        White,
        Gray
    };

    QIActivityIndicator(QQuickItem* parent = 0);
    ~QIActivityIndicator();

    Q_INVOKABLE void startAnimation();

    Q_INVOKABLE void stopAnimation();

    bool isAnimating() const;
    void setIsAnimating(bool isAnimating);

    Style style() const;
    void setStyle(const Style &style);

signals:
    void isAnimatingChanged();
    void styleChanged();

private:
    bool m_isAnimating;

    Style m_style;

};

#endif // ACTIVITYINDICATOR_H
