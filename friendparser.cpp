#include "friendparser.h"
#include <QDebug>


bool FriendParser::fatalError(const QXmlParseException& exception)
{
    return false;
}

bool FriendParser::startElement(const QString &, const QString &, const QString &tagName, const QXmlAttributes &attrs)
{
    return true;
}

bool FriendParser::characters(const QString &strText)
{
    m_strText = strText;
    return true;
}

bool FriendParser::endElement(const QString &, const QString &, const QString &tagName)
{
    if (tagName == "uid")
        user.setUserID(m_strText);
    else if (tagName == "first_name")
        user.setFirstName(m_strText);
    else if (tagName == "last_name")
        user.setLastName(m_strText);
    else if (tagName == "photo")
        user.setPhotoUrl(m_strText);
    else if (tagName == "user")
    {
        emit addFriend(user);
    }
    else if (tagName == "response")
        emit endParsing();

    return true;
}
