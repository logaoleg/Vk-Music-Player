#include "trackdownload.h"

TrackDownload::TrackDownload(Track tr):Track(tr)
{

}
TrackDownload::TrackDownload():Track()
{
    setFileName(QString(""));
}

void TrackDownload::setFileName(QString fn)
{
    this->fname = fn;
}

QString TrackDownload::getFileName()
{
    return fname;
}


