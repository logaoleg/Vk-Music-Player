#include "lastfmapi.h"


LastFmApi::LastFmApi(QString apiKey)
{
    this->apiKey = apiKey;
    downloadManager = new DownloadManager();
    connect(downloadManager, SIGNAL(xmlDownloaded(QNetworkReply*)), this, SLOT(processXML(QNetworkReply*)));
}

void LastFmApi::requestArtistIcon(QString artist)
{
    QUrl request("http://ws.audioscrobbler.com/2.0/?");
    request.addQueryItem("method", "artist.getinfo");
    request.addQueryItem("artist", artist);
    request.addQueryItem("api_key",  apiKey);
    downloadManager->sendRequest(request, DownloadManager::XML);
}

//void LastFmApi::requestArtistIcon(QString artist)
//{
//    QUrl request("http://ws.audioscrobbler.com/2.0/?");
//    request.addQueryItem("method", "artist.getinfo");
//    request.addQueryItem("artist", artist);
//    request.addQueryItem("api_key",  apiKey);
//    downloadManager->sendRequest(request, DownloadManager::XML);
//}

void LastFmApi::processXML(QNetworkReply *reply)
{
     QString str = reply->readAll();
     qint32 start = str.indexOf("<image size=\"large\">") + QString("<image size=\"large\">").length();
     qint32 end = str.indexOf("</image>",start);
     emit urlRecieved(QUrl(str.mid(start  , end - start)));
}

LastFmApi::~LastFmApi()
{
    delete downloadManager;
}

