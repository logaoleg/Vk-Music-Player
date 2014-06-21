#ifndef LASTFM_H
#define LASTFM_H
#include <QObject>
#include <QUrl>
#include <QDebug>
#include "downloadmanager.h"
#include <QEventLoop>
#include <QString>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class LastFmApi : public QObject
{
    Q_OBJECT
public:
    LastFmApi(QString apiKey);
    //void requstArtistIcon(QString artist);
    void requestArtistIcon(QString artist);

    ~LastFmApi();

signals:
    void urlRecieved(QUrl);
private:
    QString apiKey;
    DownloadManager *downloadManager;

private slots:
    void processXML(QNetworkReply*reply);
};

#endif // LASTFM_H
