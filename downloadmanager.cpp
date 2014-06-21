#include "downloadmanager.h"


DownloadManager::DownloadManager(QObject *parent)
{
    networkManager = new QNetworkAccessManager();
    TYPE_SIZE = 3; //size of enum Content Type

    connect(networkManager,SIGNAL(finished(QNetworkReply*)), this, SLOT(informationDownloaded(QNetworkReply*)));
}

DownloadManager::~DownloadManager()
{

}

void DownloadManager::sendRequest(QUrl url, DownloadManager::ContentType type)
{
    QNetworkRequest request;
    request.setUrl(url);
    networkManager->get(request);
    setType(type);
}

bool DownloadManager::setType(DownloadManager::ContentType type)
{
    if (type >= 0 && type < TYPE_SIZE)
    {
        this->type = type;
        return true;
    }
    else
    {
        this->type  = ERROR;
    }
}

QNetworkReply *DownloadManager::getWithWaiting(QNetworkRequest req)
{
    QNetworkAccessManager* netAccManager = new QNetworkAccessManager();

    QNetworkReply *reply = netAccManager->get(req);
    QEventLoop loop;
    QObject::connect(reply,SIGNAL(finished()), &loop,SLOT(quit()));
   // connect(reply,SIGNAL(),&loop,SLOT(quit()));
    loop.exec();

    return reply;
}

DownloadManager::ContentType DownloadManager::getType()
{
    return this->type;
}

void DownloadManager::informationDownloaded(QNetworkReply *networkReply)
{
    switch(type)
    {
    case IMAGE:
        emit imageDownloaded(networkReply);
        break;

    case XML:
        emit xmlDownloaded(networkReply);
        break;

    case LYRIC:
        emit lyricDownloaded(networkReply);
        break;

    default:
        qDebug() << "Error type or information";
    }
}
