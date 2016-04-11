#include "downloader.h"

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStandardPaths>

Downloader::Downloader(QObject *parent)
    : QObject(parent)
    , m_manager(new QNetworkAccessManager(this))
{
    QObject::connect(m_manager, &QNetworkAccessManager::finished, this, &Downloader::onDownloadFinished);
}

void Downloader::download() {
    QNetworkRequest request(m_url);
    QObject::connect(m_manager->get(request), &QNetworkReply::downloadProgress, this, &Downloader::progress);
}

void Downloader::setUrl(const QString &url) {
    m_url = url;
}


void Downloader::onDownloadFinished(QNetworkReply *reply) {

    if(reply->error() != QNetworkReply::NoError) {
        emit error(reply->errorString());
        return;
    }

    QStringList tempLocations = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
    if(tempLocations.empty()) {
        emit error("Temp locations are not avaiable");
        return;
    }

    QUrl url(m_url);

    QFile localFile(tempLocations.at(0) + "/" + url.fileName());
    if (!localFile.open(QIODevice::WriteOnly)) {
        emit error(tr("Can't open file for write %1").arg(localFile.fileName()));
        return;
    }

    const QByteArray data = reply->readAll();
    localFile.write(data);
    localFile.close();

    emit finished(localFile.fileName());
}

