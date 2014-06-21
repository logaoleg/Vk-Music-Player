#include "filedownloader.h"
#include <QDebug>

FileDownloader::FileDownloader( QObject *parent) :
    QObject(parent)
{
    connect(&networkManager, SIGNAL(finished(QNetworkReply*)),
                SLOT(fileDownloaded(QNetworkReply*)));
    connect(this, SIGNAL(trackAdded()),SLOT(slotTrackAdded()));

    firstCalling = true;
}

void FileDownloader::downloadFile(QString name, QUrl url)
{

}

void FileDownloader::downloadFile(Track track)
{
    if (permanentDir.exists()) //якщо папка встановлена
        fileName = permanentDir.path() + "/" + track.getTrackName() + ".mp3";
    else
        fileName = getFileName(track.getTrackName());

    if (fileName.isEmpty()) // TODO: check if fileName == ""
        return;
    qDebug() << track.getTrackName();
//    TrackDownload additionalTrack;
//    additionalTrack.fname = fileName;
//    additionalTrack.url = url;
    qDebug() << fileName;
    TrackDownload additionalTrack(track);
    additionalTrack.setFileName(fileName);
 qDebug() << additionalTrack.getFileName();
    trackQueue.enqueue(additionalTrack);

    if (firstCalling)
    {
        emit trackAdded();
        firstCalling = false;
    }
}


bool FileDownloader::isQueueEmpty()
{
    return trackQueue.isEmpty();
}

qint64 FileDownloader::getQueueLength()
{
    return trackQueue.length();
}

FileDownloader::~FileDownloader()
{
//    delete reply;
}


void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    QFile  f (currentTrack.getFileName());
    if (!f.open( QIODevice::WriteOnly ))
        qDebug() << "File doesn't open.";
    f.write(pReply->readAll());
    f.close();

//    qDebug() << "File downloaded" << currentTrack.getFileName();

    //emit a signal
    if (!trackQueue.isEmpty())
        //emit downloaded();
        slotTrackAdded();
    else firstCalling = true;
}

void FileDownloader::downloadProgress(qint64 f1, qint64 f2)
{
    emit dataProgress(f1,f2);
}

QString FileDownloader::getFileName(QString name)
{
    QString fname;
    QFileDialog dialog;
    dialog.selectFile(name);
    fname = dialog.getSaveFileName(
                0,
                tr("Завантаження треку"),
                QDir::currentPath() + "\\" + name + ".mp3",
                tr("Music (*.mp3)"));

    return fname;
}

QByteArray FileDownloader::getData() const
{
    return downloadData;
}
void FileDownloader::setParmanentDirrectory(QString path)
{
    permanentDir.setPath(path);

}

QDir FileDownloader::getPermanentDirectory()
{
    return permanentDir.path();
}

void FileDownloader::slotTrackAdded()
{
//    qDebug() << trackQueue.first();
    currentTrack = trackQueue.dequeue();
    request.setUrl(currentTrack.getUrl());
    emit textChanged(currentTrack.getTrackName());
//    qDebug() << "slotTrackAdded file name = " << currentTrack.getFileName();
//    qDebug() << "slotTrackAdded url name = " << currentTrack.getUrl();
    qDebug() << "slotTrackAdded track name = " << currentTrack.getTrackName();
    QNetworkReply *reply = networkManager.get(request);

    connect( reply,SIGNAL(downloadProgress(qint64,qint64)),
             this, SLOT(downloadProgress(qint64,qint64)));
}

