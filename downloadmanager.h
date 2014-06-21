#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H
#include <QString>
#include <QUrl>
#include <QEventLoop>
#include <QObject>
#include <QNetworkReply>
#include <QDebug>
#include<QNetworkRequest>
#include <QNetworkAccessManager>

class DownloadManager : public QObject
{
    Q_OBJECT
public:

    enum ContentType
    {
        LYRIC = 0,
        IMAGE,
        XML,
        ERROR
    };

    explicit DownloadManager(QObject *parent = 0);
    virtual ~DownloadManager();

    /***Methods****/
    void sendRequest(QUrl url, ContentType type);
    bool setType(ContentType type);
    QNetworkReply *getWithWaiting(QNetworkRequest req);
    ContentType getType();

signals:
    void lyricDownloaded(QNetworkReply *reply);
    void imageDownloaded(QNetworkReply *reply);
    void xmlDownloaded(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    ContentType type;
    qint32 TYPE_SIZE;

private slots:
    void informationDownloaded(QNetworkReply *networkReply);



};
#endif // DOWNLOADMANAGER_H
