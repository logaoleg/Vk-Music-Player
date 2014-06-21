#include "vkapi.h"
#include <QWebView>
#include <QUrl>
#include <QObject>
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QtNetwork/QNetworkRequest>



VkApi::VkApi(QString id, QObject *parent) :
    QObject(parent)
{
    setAppId(id);
    webView = 0;
    downloadManager = new DownloadManager(); //create DownloadManager object for downloading lyrics
}

VkApi::VkApi(QString accessToken, QString userID, QObject *parent) : QObject(parent)
{
    this->accessToken = accessToken;
    this->userId = userID;
    webView = 0;
    downloadManager = new DownloadManager();
}

VkApi::VkApi(QObject *parent)
{
}

VkApi::~VkApi()
{
//    if (downloadManager)
        delete downloadManager;
//    if (webView)
        delete webView;

}

void VkApi::authorization(QString appId)
{
    qDebug() << "authorization";
    if (appId.isNull())
    {
        qDebug() << "Error: appId is Null";
        return;
    }

    //Create webView for auhorization
    webView = new QWebView(0);
    webView->setWindowTitle("Авторизація");
    webView->setWindowIcon(QPixmap(":/icon/icon/cassette-2-icon.png"));
    webView->resize(750,380);
    //Set webView on center of screen
    webView->move((QApplication::desktop()->width() -  webView->width())/2,
            (QApplication::desktop()->height() -  webView->height())/2);

    //Create url for authorization
    QUrl url("http://oauth.vk.com/authorize");
    url.addQueryItem("client_id"     ,  appId);
    url.addQueryItem("display"  , "popup"  );
    url.addQueryItem("scope", "audio,status,wall");
    url.addQueryItem("redirect_uri", "http://api.vk.com/blank.html");
    url.addQueryItem("response_type", "token");

    webView->load(url);
    webView->show();
    //After webView send information, url will be redirected to another url with answer.
    connect(webView, SIGNAL(urlChanged(QUrl)),this, SLOT(slotUrlChanged(QUrl)));
}

void VkApi::start()
{
    if (accessToken.isEmpty() || accessToken.isNull()) //if user was not authorizated
        authorization(appId);
    else {
        emit success();
        getAudio();
    }
}

void VkApi::getAudio(qint64 count, qint64 offset,QString ownerId)
{
    qDebug() << "getAudio(" << count << ")";

    //Formulate audio.get request to vk.com
    QUrl request("https://api.vk.com/method/audio.get.xml");
    request.addQueryItem("access_token", accessToken);
    if (ownerId.isEmpty())
        request.addQueryItem("oid", userId);
    else
        request.addQueryItem("oid", ownerId);
    request.addQueryItem("count", QString::number(count));
    request.addQueryItem("offset", QString::number(offset));
    qDebug() << request;
//    downloadManager->sendRequest(request, DownloadManager::XML);
    networkReply =  downloadManager->getWithWaiting(QNetworkRequest (request));
    MusicParser parser; //create object for parsing xml file
    connect(&parser,SIGNAL(addTrack(QString,QString,uint,QUrl, QString, long long,QString)),
            this,SLOT(slotAddTrack(QString,QString,uint,QUrl,QString, long long, QString)));
    connect(&parser, SIGNAL(endFile()), this, SLOT(slotEndFile()));
    QXmlInputSource source(networkReply);
    QXmlSimpleReader reader;
    reader.setContentHandler(&parser);
    reader.parse(source);
    networkReply->close();
}

void VkApi::setAccessToken(QString token)
{
    accessToken = token;
}

QString VkApi::getAccessToken()
{
    return accessToken;
}

void VkApi::setAppId(QString appId)
{
    this->appId = appId;
}

bool VkApi::logout()
{
    //Logout user form vk.com
    accessToken.clear();
    networkReply->close();
    return true;
}

QString VkApi::getAppId()
{
    return appId;
}
QString VkApi::getUserId()
{
    return userId;
}

void VkApi::setUserID(QString id)
{
    this->userId = id;
}

QString VkApi::getLyrics(long long lyricsId)
{
    //Formulate getLyrics request for vk.com
    QUrl request("https://api.vk.com/method/audio.getLyrics.xml");
    request.addQueryItem("access_token", accessToken);
    request.addQueryItem("lyrics_id",QString::number(lyricsId));
    qDebug() << request;
    QString str = downloadManager->getWithWaiting(QNetworkRequest(request))->readAll();

    //Parse lyrics from reply
    qint32 start = str.indexOf("<text>") + QString("<text>").length();
    qint32 end = str.indexOf("</text>",start);
    return str.mid(start  , end - start);
}

void VkApi::slotAddTrack(QString artist, QString title, uint duration, QUrl url,QString audioID, long long lyricsId, QString ownerId)
{
    emit signalAddTrack(artist,title,duration,url,audioID, lyricsId, ownerId);
}
void VkApi::slotEndFile()
{
    emit endFile();
}

void VkApi::successAuthorization()
{
    emit success();
//    getAudio();
}
void VkApi::slotUrlChanged(QUrl url)
{
    url = url.toString().replace("#","?");

    if (url.hasQueryItem("client_id"))
        return;

    if (url.hasQueryItem("error") ||  (!url.hasQueryItem("access_token"))) {
        qDebug() << "Error: access_token not got";
        emit fail();
        return;
    }

    accessToken = url.queryItemValue("access_token"); // parse access_token
    userId = url.queryItemValue("user_id");
    qDebug() << userId;
    webView->close();
//    delete webView;

    successAuthorization();

}


void VkApi::setBroadcast(bool enabled)
{
    QUrl request("https://api.vk.com/method//audio.setBroadcast.xml");
    request.addQueryItem("access_token", accessToken);
    request.addQueryItem("enabled",QString(enabled));

//    downloadManager->sendRequest(request, DownloadManager::XML);

    QNetworkReply *reply = downloadManager->getWithWaiting(QNetworkRequest(request));

//    qDebug() << QString(reply->readAll()) ;
    QString str = reply->readAll();
    if (!str.contains("<enabled>1</enabled>"))
        qDebug() << "Error: setBroadcast";

}

void VkApi::setAudioStatus(QString audioID)
{
    QUrl request("https://api.vk.com/method//status.set.xml");
    request.addQueryItem("access_token", accessToken);
    request.addQueryItem("audio",QString(userId + "_" + audioID));
    qDebug() << request.toString();

//     downloadManager->sendRequest(request, DownloadManager::XML);

    QNetworkReply *reply = downloadManager->getWithWaiting(QNetworkRequest(request));

//    qDebug() << QString(reply->readAll()) ;
    QString str = reply->readAll();
    qDebug() << str;
    if (!str.contains("<response>1</response>"))
        qDebug() << "Error: setBroadcast";
}

void VkApi::setTextStatus(QString text)
{
    QUrl request("https://api.vk.com/method//status.set.xml");
    request.addQueryItem("access_token", accessToken);
    request.addQueryItem("text",text);
    qDebug() << request.toString();

//     downloadManager->sendRequest(request, DownloadManager::XML);

    QNetworkReply *reply = downloadManager->getWithWaiting(QNetworkRequest(request));

//    qDebug() << QString(reply->readAll()) ;
    QString str = reply->readAll();
    qDebug() << str;
    if (!str.contains("<response>1</response>"))
        qDebug() << "Error: setBroadcast";
}

void VkApi::audioSearch(QString text,qint64 count , qint64 offset )
{
    //Formulate audio.get request to vk.com
    QUrl request("https://api.vk.com/method/audio.search.xml");
    request.addQueryItem("access_token", accessToken);
    request.addQueryItem("q", text);
    request.addQueryItem("auto_complete", QString::number(1));
    request.addQueryItem("sort", QString::number(2));
    request.addQueryItem("count", QString::number(count));
    request.addQueryItem("offset", QString::number(offset));
      qDebug() << request;

      networkReply = downloadManager->getWithWaiting(QNetworkRequest(request));
//    downloadManager->sendRequest(request, DownloadManager::XML);
//    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
//    networkReply = manager->get(QNetworkRequest(request));
//    connect (manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkReplyDone()));
      MusicParser parser; //create object for parsing xml file
      connect(&parser,SIGNAL(addTrack(QString,QString,uint,QUrl, QString, long long, QString)),
              this,SLOT(slotAddTrack(QString,QString,uint,QUrl,QString, long long,QString)));
      connect(&parser, SIGNAL(endFile()), this, SLOT(slotEndFile()));
      QXmlInputSource source(networkReply);
      QXmlSimpleReader reader;
      reader.setContentHandler(&parser);
      reader.parse(source);
      networkReply->close();

}

void VkApi::audioSuggest(qint64 count , qint64 offset )
{
    QUrl request("https://api.vk.com/method/audio.getRecommendations.xml");
    request.addQueryItem("access_token", accessToken);
    request.addQueryItem("uid", userId);
    request.addQueryItem("count", QString::number(count));
    request.addQueryItem("offset", QString::number(offset));
    qDebug() << request;

    networkReply = downloadManager->getWithWaiting(QNetworkRequest(request));

     MusicParser parser; //create object for parsing xml file
    connect(&parser,SIGNAL(addTrack(QString,QString,uint,QUrl, QString, long long, QString)),
            this,SLOT(slotAddTrack(QString,QString,uint,QUrl,QString, long long, QString)));
    connect(&parser, SIGNAL(endFile()), this, SLOT(slotEndFile()));
    QXmlInputSource source(networkReply);
    QXmlSimpleReader reader;
    reader.setContentHandler(&parser);
    reader.parse(source);
    networkReply->close();
}

QString VkApi::editTrack(QString audioID, QString artist, QString title)
{
    QUrl request("https://api.vk.com/method/audio.edit.xml");
    request.addQueryItem("access_token", accessToken);
    request.addQueryItem("oid", userId);
    request.addQueryItem("aid", audioID);
    request.addQueryItem("artist", artist);
    request.addQueryItem("title", title);


    qDebug() << request;

    networkReply = downloadManager->getWithWaiting(QNetworkRequest(request));
    QString str = networkReply->readAll();
    qint32 start = str.indexOf("<response>") + QString("<response>").length();
    qint32 end = str.indexOf("</response>",start);
    return str.mid(start, end - start);
     networkReply->close();
}

void VkApi::addAudioToPage(QString audioId, QString ownerId)
{
    QUrl request("https://api.vk.com/method/audio.add.xml");
    request.addQueryItem("access_token", accessToken);
    request.addQueryItem("aid", audioId);
    request.addQueryItem("oid", ownerId);
//    request.addQueryItem("gid", QString::number(-1));



    qDebug() << request;

    networkReply = downloadManager->getWithWaiting(QNetworkRequest(request));
     networkReply->close();
}

void VkApi::deleteAudio(QString audioId)
{
    QUrl request("https://api.vk.com/method/audio.delete.xml");
    request.addQueryItem("access_token", accessToken);
    request.addQueryItem("aid", audioId);
    request.addQueryItem("oid", userId);

     networkReply = downloadManager->getWithWaiting(QNetworkRequest(request));

     networkReply->close();

}

void VkApi::getFriend(qint64 count, qint64 offset)
{
    QUrl request("https://api.vk.com/method/friends.get.xml");
    request.addQueryItem("uid", userId);
    request.addQueryItem("order", "hints");
    request.addQueryItem("count", QString::number(count));
    request.addQueryItem("offset", QString::number(offset));
    request.addQueryItem("fields", "uid, first_name, last_name, photo");

    qDebug() << request;

    networkReply = downloadManager->getWithWaiting(QNetworkRequest(request));
    FriendParser parser; //create object for parsing xml file
    connect(&parser,SIGNAL(addFriend(User)),
            this,SLOT(addFriend(User)));
    connect(&parser, SIGNAL(endParsing()), this, SLOT(endFriendParsing()));
    QXmlInputSource source(networkReply);
    QXmlSimpleReader reader;
    reader.setContentHandler(&parser);
    reader.parse(source);
    networkReply->close();
}

void VkApi::endFriendParsing()
{
    emit endFrienParsingSgn();
}

void VkApi::addFriend(User user)
{
    emit addFriendSgn(user);
}






