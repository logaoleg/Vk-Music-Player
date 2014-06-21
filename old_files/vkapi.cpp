#include "vkapi.h"
#include "vkauthorization.h"
#include <QtWebKit>
#include <QDebug>




VkApi::VkApi(QWidget *parent /*=0*/) : QObject(parent)
{
}

void VkApi::authorization(QString appId)
{
//    connect(view, SIGNAL(urlChanged(QUrl)),this, SLOT(getAccessToken(QUrl)));

    view = new QWebView();
    connect(view, SIGNAL(urlChanged(QUrl)),this, SLOT(urlChanged(QUrl)));
    this->appId = appId;
//    qDebug() << appId;
//    VkAuthorization author(appId);
//    author.show();

    QUrl url("http://oauth.vk.com/authorize");

    url.addQueryItem("client_id"     ,  appId   );
    url.addQueryItem("display"  , "popup"  );
    url.addQueryItem("scope", "audio,offline,ads");
    url.addQueryItem("redirect_uri", "http://api.vk.com/blank.html");
    url.addQueryItem("response_type", "token");

//    qDebug() << url;



//    connect (view, SIGNAL(loadStarted()),this, SLOT(test()));

    view->resize(640,420);
//    view->load(QUrl("http://vf.com"));
    view->load(url);
//    view->setUrl(url);
    view->show();


//    QObject::connect(view, SIGNAL())
//    connect(view, SIGNAL(loadFinished(bool)), this, SLOT(slotLoadFinished(bool)));


}

void VkApi::getAccessToken(const QUrl &url)
{
    qDebug() << "work";
//    url.toString().replace("#","&");
//    if (url.hasQueryItem("error") ||  (!url.hasQueryItem("access_token"))) {
//        qDebug() << "Error: access_token not got";
//        return;
//    }

//    accessToken = url.queryItemValue("access_token");
//    expires = url.queryItemValue("expires").toInt();
//    userId = url.queryItemValue("user_id");

//    qDebug() << accessToken;
//    qDebug() << expires;
//    qDebug() << userId;
}

void VkApi::slotLoadFinished(bool ok)
{
    if (ok)
        view->close();


}
