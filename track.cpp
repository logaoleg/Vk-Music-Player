#include "track.h"

Track::Track(QString artist, QString title,unsigned int duration, QUrl url,QString audioID, long long lyricsID, QString ownerId)
{
    setTrack(artist, title, duration, url, audioID, lyricsID,ownerId);
}

Track::Track(const Track &tr)
{
    setTrack(tr.artist,
             tr.title,
             tr.duration,
             tr.url,
             tr.audioID,
             tr.lyricsID,
             tr.ownerId);

}

Track::Track()
{
    setTrack(QString(""),QString(""),0,QString(""),QString(""),0);

}

void Track::setTrack(QString artist, QString title, unsigned int duration, QUrl url,QString audioID, long long lyricsID, QString ownerId)
{
    this->artist = artist;
    this->title = title;
    this->duration = duration;
    this->url = url;
    this->lyricsID = lyricsID;
    this->audioID = audioID;
    this->ownerId = ownerId;
}

void Track::getTrack(QString &artist, QString &title,unsigned int &duration, QUrl &url,QString &audioID, long long &lyricsID, QString &ownerId)
{
    artist = this->artist;
    title = this->title;
    url = this->url;
    duration = this->duration;
    lyricsID = this->lyricsID;
    audioID = this->audioID;
    ownerId = this->ownerId;
}

void Track::setArtist(QString artist)
{
    this->artist = artist;
}

QString Track::getArtist()
{
    return artist;
}

void Track::setTitle(QString title)
{
    this->title = title;
}

QString Track::getTitle()
{
    return title;
}

void Track::setUrl(QUrl url)
{
    this->url = url;
}

QUrl Track::getUrl()
{
    return url;
}

void Track::setLyricsID(long long lyricsID)
{
    this->lyricsID = lyricsID;
}

long long Track::getLyricsID()
{
    return lyricsID;
}

void Track::setDuration(unsigned int duration)
{
    this->duration = duration;
}

unsigned int Track::getDuration()
{
    return duration;
}

QString Track::getTrackName()
{
    return artist + " - " + title;
}

QString Track::getAudioID()
{
    return audioID;
}

void Track::setAudioID(QString audioID)
{
    this->audioID = audioID;
}

void Track::setOwnerId(QString owner)
{
    this->ownerId = owner;
}

QString Track::getOwnerId()
{
    return ownerId;
}

bool Track::operator ==(Track track)
{
    return (this->getTrackName() == track.getTrackName());
}



