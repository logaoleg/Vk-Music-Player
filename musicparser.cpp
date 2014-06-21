/**
  Class for parsing XML file
 **/

#include "musicparser.h"


bool MusicParser::fatalError(const QXmlParseException& exception)
{
    qDebug() << "Line:"      << exception.lineNumber()
             << ", Column:"  << exception.columnNumber()
             << ", Message:" << exception.message();
    return false;
}

bool MusicParser::startElement(const QString &, const QString &, const QString &tagName, const QXmlAttributes &attrs)
{
     if (tagName == "response")
         existLyrics = false;
    return true;
}

bool MusicParser::characters(const QString &strText)
{
    m_strText = strText;
    return true;
}

bool MusicParser::endElement(const QString &, const QString &, const QString &tagName)
{
    if (tagName == "artist")
        artist = Utils::transcodeStr(m_strText);
    else if (tagName == "title")
        title =  Utils::transcodeStr(m_strText);
    else if (tagName == "duration")
        duration = m_strText.toUInt();
    else if (tagName == "url")
        url = static_cast<QUrl>(m_strText);
    else if (tagName == "aid")
        audioID =  Utils::transcodeStr(m_strText);
    else if (tagName == "owner_id")
        ownerId =  Utils::transcodeStr(m_strText);
    else if (tagName == "lyrics_id")
    {
        lyricsId = m_strText.toLongLong();
        existLyrics = true;
    }
    else if (tagName == "audio")
    {
        if (!existLyrics)
            lyricsId = 0;
//        qDebug() << "parseTrack";
        emit addTrack(artist, title, duration, url, audioID, lyricsId, ownerId);
        existLyrics = false;
    }

    else if (tagName == "response"){
//        qDebug() << "endFile";
        emit endFile();
    }


    return true;
}
