#include "qisystemmessenger.h"
#include "qiactivityindicator.h"

QIActivityIndicator::QIActivityIndicator(QQuickItem* parent) : QQuickItem(parent)
{
    m_isAnimating = false;
    m_style = Gray;
}

QIActivityIndicator::~QIActivityIndicator()
{

}

void QIActivityIndicator::startAnimation()
{
    if (m_isAnimating)
        return;

    QVariantMap map;
    map["style"] = m_style;

    QISystemMessenger::instance()->sendMessage("activityIndicatorStartAnimation",map);
    setIsAnimating(true);
}

void QIActivityIndicator::stopAnimation()
{
    if (!m_isAnimating)
        return;

    QISystemMessenger::instance()->sendMessage("activityIndicatorStopAnimation",QVariantMap());
    setIsAnimating(false);
}

bool QIActivityIndicator::isAnimating() const
{
    return m_isAnimating;
}

void QIActivityIndicator::setIsAnimating(bool isAnimating)
{
    m_isAnimating = isAnimating;
    emit isAnimatingChanged();
}

QIActivityIndicator::Style QIActivityIndicator::style() const
{
    return m_style;
}

void QIActivityIndicator::setStyle(const Style &style)
{
    m_style = style;
    emit styleChanged();
}



