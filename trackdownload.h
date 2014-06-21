#ifndef TRACKDOWNLOAD_H
#define TRACKDOWNLOAD_H

#include "track.h"
#include <QString>

class TrackDownload : public Track
{
public:
    TrackDownload(Track tr);
    TrackDownload();

    void setFileName(QString fn);
    QString getFileName();

private:
    QString fname;
};

#endif // TRACKDOWNLOAD_H
