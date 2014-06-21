/****
  Class for downloading files from internet. Using for downloading track from vk.com
  ****/
#ifndef FileDownloader_H
#define FileDownloader_H
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QFile>
#include <QUrl>
#include <QQueue>
#include <QFileDialog>
#include <QNetworkReply>

#include "track.h"
#include "trackdownload.h"

//struct TrackDownload
//{
//    QString fname;
//    QUrl url;
//};

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QObject *parent = 0);
    void downloadFile(Track track);
    void downloadFile(QString name, QUrl url);
    bool isQueueEmpty();
    qint64 getQueueLength();

    virtual ~FileDownloader();

    QByteArray getData() const;
    void setParmanentDirrectory(QString path);
    QDir getPermanentDirectory();

signals:
    void downloaded();
    void dataProgress(qint64 , qint64);
    void trackAdded();
    void textChanged(QString text);


private slots:
    void slotTrackAdded();
    void fileDownloaded(QNetworkReply* pReply);
    void downloadProgress (qint64 f1, qint64 f2);

private:
    QQueue<TrackDownload> trackQueue;
    TrackDownload currentTrack;
    QNetworkAccessManager networkManager;
    QString fileName;
    QByteArray downloadData;
    QNetworkReply *reply;
    QNetworkRequest request;
    QDir permanentDir;
    bool firstCalling;
    QString getFileName(QString name);

};

#endif // FileDownloader_H
