#ifndef VKAUTHORIZATION_H
#define VKAUTHORIZATION_H

#include <QWebView>
class QString;
class QUrl;

class VkAuthorization : public QWebView
{
    Q_OBJECT
private:
    QString appId;
public:
    VkAuthorization(QString appId, QWidget *parent);
    void setAppId(QString appId);
    QString getAppId();

public slots:
    void urlChanged(const QUrl &url);
    
};

#endif // VKAUTHORIZATION_H
