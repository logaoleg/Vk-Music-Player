#ifndef FRIENDPARSER_H
#define FRIENDPARSER_H

#include <QObject>
#include <QXmlDefaultHandler>
#include "user.h"

class FriendParser : public QObject, public QXmlDefaultHandler
{
    Q_OBJECT
private:
    User user;
    QString m_strText;

public:
    bool startElement(const QString&, const QString&,
                      const QString& tagName,
                      const QXmlAttributes& attrs
                     );
    bool characters(const QString& strText) ;
    bool endElement(const QString&, const QString&, const QString& tagName);
    bool fatalError(const QXmlParseException& exception);
signals:
    void addFriend(User u);
    void endParsing();

};

#endif // FRIENDPARSER_H

