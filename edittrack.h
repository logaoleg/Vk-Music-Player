#ifndef EDITTRACK_H
#define EDITTRACK_H

#include <QDialog>
#include<QTextCodec>
#include "track.h"
#include "vkapi.h"

namespace Ui {
    class EditTrack;
}

class EditTrack : public QDialog
{
    Q_OBJECT

public:
    explicit EditTrack(QString accessToken, QString userId, QDialog *parent = 0);

    void editTrack(Track track);
//    void setAccessToken(QString accessToken);
    ~EditTrack();

private:
    Ui::EditTrack *ui;
    VkApi *vk;
//    QString audioID;
    Track *track;

signals:
    void trackEdited(Track track);

private slots:
    void slotTrackEdited();
};

#endif // EDITTRACK_H
