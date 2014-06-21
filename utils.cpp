#include "utils.h"

Utils::Utils()
{
}

QString Utils::transcodeStr(QString str)
{
    str = str.replace("&amp;", "&");
    return str;
}

int Utils::randomSortTrack(Track track1, Track track2)
{
    if (qrand() > 0.5) {
        return -1;
    } else {
        return 1;
    }
}

