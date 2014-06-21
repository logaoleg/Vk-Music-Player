#ifndef TRACK_H
#define TRACK_H
#include <QUrl>
#include <QString>

class Track
{
public:
    Track();
    Track(QString artist, QString title, unsigned int duration,QUrl url, QString audioID,long long lyricsID, QString ownerId);
    Track(const Track &tr);
    void setTrack(QString artist, QString title, unsigned int duration ,QUrl url, QString audioID,long long lyricsID, QString ownerId = "");
    void getTrack(QString &artist, QString &title,unsigned int &duration, QUrl &url, QString &audioID,long long &lyricsID, QString &ownerId);

    void setArtist(QString artist);
    QString getArtist();
    void setTitle(QString title);
    QString getTitle();
    QString getTrackName();
    void setUrl(QUrl url);
    QUrl getUrl ();
    void setLyricsID(long long lyricsID);
    long long getLyricsID();
    void setDuration(unsigned int duration);
    unsigned int getDuration();
    QString getAudioID();
    void setAudioID(QString audioID);
    void setOwnerId(QString owner);
    QString getOwnerId();

    bool operator ==(Track track);



private:
    QString artist;
    QString title;
    QString ownerId;
    QUrl url;
    unsigned int duration;
    long long  lyricsID;
    QString audioID;
};

#endif // TRACK_H
