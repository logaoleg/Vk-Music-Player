#ifndef VKAPI_H
#define VKAPI_H

#include <QObject>
#include <QUrl>
#include <QDebug>

class QString;
class QWidget;
class QWebView;


class VkApi : public QObject
{
    Q_OBJECT
private:
    QString appId;
    QString userId;
    QString accessToken;
    qint64 expires;
    QWebView *view;

public:
    explicit VkApi(QWidget *parent = 0);
    void authorization(QString appId);

public slots:
    void getAccessToken(const QUrl &url);
    void urlChanged(const QUrl &url)
    {
        qDebug() << "url = " << url;
    }
    void test()
    {
        qDebug()<<
                  "test";

    }

    void slotLoadFinished(bool ok);
};
#endif // VKAPI_H
