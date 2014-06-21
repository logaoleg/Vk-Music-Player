#ifndef MUSICPARSER_H
#define MUSICPARSER_H

#include <QXmlDefaultHandler>
#include <QUrl>
#include <QObject>
#include <QDebug>

#include "utils.h"

//inherit QXmlDefaultHandler for writing own parser
class MusicParser : public QObject, public QXmlDefaultHandler
{
    Q_OBJECT
private:
    QString m_strText;
    QString artist;
    QString title;
    long long lyricsId;
    QString audioID;
    unsigned int duration;
    bool existLyrics; // available lyrics of track
    QUrl url;
    QString ownerId;

public:
    bool startElement(const QString&, const QString&,
                      const QString& tagName,
                      const QXmlAttributes& attrs
                     );
    bool characters(const QString& strText) ;
    bool endElement(const QString&, const QString&, const QString& tagName);
    bool fatalError(const QXmlParseException& exception);

signals:
    void addTrack(QString a, QString, uint d, QUrl u, QString audioID, long long lyricsId = 0, QString ownerId = "");
    void endFile();
};

#endif // MUSICPARSER_H
