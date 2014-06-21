#ifndef VKAPI_H
#define VKAPI_H

#include <QObject>
#include <QUrl>
#include <QFile>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QByteArray>
#include "musicparser.h"
#include "downloadmanager.h"
#include "friendparser.h"
#include "user.h"
//#include "filedownloader.h"

class QString;
class QWebView;


class VkApi : public QObject
{
    Q_OBJECT
public:
    VkApi(QString id, QObject *parent = 0);
    VkApi(QString accessToken, QString userID, QObject *parent = 0);
    VkApi(QObject *parent = 0);
    ~VkApi();

    void authorization(QString appId); //auhorizate user in vk.com
    bool logout();

    void start(); //start working with API vk.com
    void getAudio(qint64 count = 50, qint64 offset = 0, QString ownerId = ""); //get audio files of user

    void addAudioToPage(QString audioId, QString ownerId);
    void getFriend(qint64 count = 25, qint64 offset = 0 );

    void deleteAudio(QString audioId);

    //Access methods to accessToken
    void setAccessToken(QString token);
    QString getAccessToken();

    //Access methods to appId
    void setAppId(QString appId);
    QString getAppId();

    void audioSearch(QString text,qint64 count = 50, qint64 offset = 0);
    void audioSuggest(qint64 count = 50, qint64 offset = 0);

    void setBroadcast(bool enabled);

    QString editTrack(QString audioID, QString artist, QString title);

    void setAudioStatus(QString audioID);
    void setTextStatus(QString text = "");

    //Access methods to userId
    QString getUserId();
    void setUserID(QString id);

    QString getLyrics(long long lyricsId);

private:
    QString appId; //aplication Id
    QString accessToken;
    QString userId;
    QWebView *webView;
    //QStringList *playlist;
    QNetworkReply *networkReply;
    DownloadManager *downloadManager; //for loading lyrics

/******Signals******/
signals:
    void signalAddTrack(QString, QString, uint, QUrl, QString, long long, QString);
    void endFile(); //alert that parsing was ended
    void fail();//failed authorization
    void success(); // successed authorization
    void endFrienParsingSgn();
    void addFriendSgn(User);


/******SLOTS*******/
public slots:
    void slotAddTrack(QString artist, QString title,uint duration, QUrl url, QString audioID,long long lyricsId, QString ownerId);
    void slotEndFile(); //actions after parsing
    void successAuthorization();
    void endFriendParsing();
    void addFriend(User user);

private slots:
    void slotUrlChanged(QUrl url);
//    void networkReplyDone(); //method for getting information that was downloaded
};

#endif // VKAPI_H
