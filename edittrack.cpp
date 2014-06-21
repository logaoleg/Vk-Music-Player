#include "edittrack.h"
#include "ui_edittrack.h"

EditTrack::EditTrack(QString accessToken, QString userId, QDialog *parent) :
    QDialog(parent),
    ui(new Ui::EditTrack)
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));    ///< Для рядкових констант
    ui->setupUi(this);
    track = 0;
    vk = new VkApi(accessToken,userId);



    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(slotTrackEdited()));
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));    ///< Для рядкових констант
}

EditTrack::~EditTrack()
{
    delete ui;
    delete vk;
    delete track;
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));    ///< Для рядкових констант
}

void EditTrack::editTrack(Track track)
{
    ui->artistLineEdit->setText(track.getArtist());
    ui->titleLineEdit->setText(track.getTitle());
    this->track = new Track(track);
}

void EditTrack::slotTrackEdited()
{
    this->close();
    track->setArtist(ui->artistLineEdit->text());
    track->setTitle(ui->titleLineEdit->text());

    vk->editTrack(track->getAudioID(),track->getArtist(),track->getTitle());
    emit trackEdited(*track);
    this->close();

}

