#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);

public slots:
    void download();
    void setUrl(const QString &url);

signals:
    void finished(const QString& fileName);
    void error(const QString &message);
    void progress(qint64 bytesReceived, qint64 bytesTotal);

private slots:
    void onDownloadFinished(QNetworkReply *data);

private:
    QNetworkAccessManager* m_manager;
    QString m_url;
};

#endif // DOWNLOADER_H