#ifndef UTILS_H
#define UTILS_H

#include <QString>

#include "track.h"

class Utils
{
public:
    Utils();

public:
    static QString transcodeStr(QString str);
    static int randomSortTrack(Track track1, Track track2);
};

#endif // UTILS_H
