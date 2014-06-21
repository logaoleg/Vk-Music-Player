#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_settings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), settings("VK Music Player", "logaoleg")
{

    //Setup GUI for class
    ui->setupUi(this);
    //Configure Phonon for playing audio
    mediaObject = new Phonon::MediaObject;
    audioOutput = new Phonon::AudioOutput;
    audioOutputPath = Phonon::createPath(mediaObject,audioOutput);
    ui->seekSlider->setMediaObject(mediaObject);
    ui->volumeSlider->setAudioOutput(audioOutput);

    //Set interval for update information
    mediaObject->setTickInterval(1000);

    //Initialise variables
    fileDownloader = new FileDownloader(this);
    vk = new VkApi("3090438");
    lastfmApi = new LastFmApi("4b0f67edcf785602f11a1d1f3fc0c849");
    currentRow = 0;
    settingsDialog = 0;
    previousTrackIndex = 0;
    nextEffect = 0;
    isFreeForDownloading = true;
    downloadManager = new DownloadManager();
    broadcastTrack = false;

   //For showing window without border uncomment
   //setWindowFlags( Qt::CustomizeWindowHint ); //without border


    //Set location of player in left corner of display
    this->move(QApplication::desktop()->width() - this->width() - 15 , 0);

    connect (this, SIGNAL(playerStateChanged(PlayerState, PlayerState )), this, SLOT(playerStateChangedSlot(PlayerState, PlayerState)));

    connect(vk, SIGNAL(addFriendSgn(User)),this,SLOT(friendsAdded(User)));

    playerState = NO_STATE;
    createActions();
    createMenus();

    //Customise GUI
    setGuiSettings();
    setStyle(":style.qss");
    updateTime();
//    ui->playlistWidget->vis

    ui->playlistWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->friendsListWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->friendsListWidget->hide();
//    playerState = MY_AUDIO;

    //Click on play button invoke authorisation in VK
    //TODO: create other button for authorization
//    connect(ui->authorizationButton,SIGNAL(clicked()), this, SLOT(slotAuthorization()));
    connect(ui->myAudioButton, SIGNAL(clicked()), this, SLOT(slotAuthorization()));

//    connect(ui->authorizationButton,SIGNAL)
    //Update time even 1 second
    connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(updateTime()));

    //When object in Phonon changed update info in ui->trackLabel
    connect(mediaObject, SIGNAL(metaDataChanged()), this, SLOT(updateStatus()));

    /*Connecting signals from this mediaObject(Phonon) with slot of this class */
    connect(mediaObject, SIGNAL(finished()), this, SLOT(finishedPlaylist()));
    connect(mediaObject, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));
    connect(mediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)),this, SLOT(updateInfo()));
    connect(ui->suggestionButton, SIGNAL(clicked()), this, SLOT(suggestAudio()));
    connect(ui->myFrAudioButton,SIGNAL(clicked()), this,SLOT(loadFriendsMusic()));

    connect(vk,SIGNAL(success()),this,SLOT(successAuthorization()));
    ui->playlistWidget->verticalScrollBar()->setRange(0,100);
     connect(ui->playlistWidget->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollAudioValueChanged(int)));
    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),this,SLOT(stateChanged(Phonon::State,Phonon::State)));

    connect(ui->searchLineEdit, SIGNAL(returnPressed()),this, SLOT(searchInVK()));
    //Recover setting that was installed
    readSettings();

    if(broadcastTrack)
        ui->broadcastButton->setEnabled(true);

    enableButons(false);

}

void MainWindow::writeSettings()
{
    settings.beginGroup("/Settings");
    settings.setValue("/access_token", vk->getAccessToken());
    settings.setValue("/permanentDir",fileDownloader->getPermanentDirectory().path());
    settings.setValue("/volume",audioOutput->volume());
    settings.setValue("/userID",vk->getUserId());
    settings.setValue("/broadcastMusic",broadcastTrack);
    settings.endGroup();
}

void MainWindow::readSettings()
{
    settings.beginGroup("/Settings");
    vk->setAccessToken( settings.value("/access_token", "").toString());
    qDebug() << settings.value("/access_token", "").toString();
    fileDownloader->setParmanentDirrectory(settings.value("/permanentDir", "").toString());
    audioOutput->setVolume(settings.value("/volume", "").toDouble());
    vk->setUserID(settings.value("/userID").toString());
    broadcastTrack = settings.value("/icic").toBool();
    settings.endGroup();
}

MainWindow::~MainWindow()
{
    writeSettings();
    vk->setTextStatus();
    delete ui;
    delete mediaObject;
    delete audioOutput;
    delete vk;
    delete fileDownloader;
}

void MainWindow::enqueuePlaylist()
{
    mediaObject->clearQueue();

    for (qint64 i = currentRow + 1; i < playlistTrack.size(); ++i)// TODO: test limit
    {
       // mediaObject->enqueue(playList[i]);
        mediaObject->enqueue(playlistTrack[i].getUrl());
    }
}

void MainWindow::setStyle(QString fname)
{
    QFile styleSheet(fname); // load CSS file
    if (!styleSheet.open(QIODevice::ReadOnly)) {
        qWarning("Unable to open file");
        return;
    }
    qApp->setStyleSheet(styleSheet.readAll()); // set own style
    styleSheet.close(); //close the file
}

void MainWindow::setGuiSettings()
{
    ui->progressBar->hide();
    setButtonsToolTip();

    createSettingsMenu();
}

void MainWindow::play()
{
   // mediaObject->setCurrentSource(playList[currentRow]);
    mediaObject->setCurrentSource(playlistTrack[currentRow].getUrl());
    if (!repeatTrack)
        enqueuePlaylist();
    mediaObject->play();
//    if (broadcastTrack)
//        vk->setAudioStatus(playlistTrack[currentRow].getAudioID());
}

void MainWindow::setButtonsToolTip()
{
//    ui->authorizationButton->setToolTip("Завантажити плейлист з vk.com");
    ui->downloadButton->setToolTip("Завантажити трек, що грає");

    ui->nextButton->setToolTip("Наступний трек");
    ui->previousButton->setToolTip("Попередній трек");
    ui->playButton->setToolTip("Програти трек");
    ui->reloadButton->setToolTip("Перемішати аудіозаписи");
    ui->toolButton->setToolTip("Налаштування");
}

void MainWindow::createSettingsMenu()
{
    settingsMenu = new QMenu();

//    initEffectsMenu();
    QAction *settingAction = new QAction("Налаштування", this);
    settingsMenu->addAction(settingAction);
    QAction *crossToPage = new QAction("Перейти на сторінку користувача", this);
    settingsMenu->addAction(crossToPage);
    QAction *chooseDirectory = new QAction("Встановити папку завантаження", this);
    settingsMenu->addAction(chooseDirectory);
    QAction *logOut = new QAction("Вийти з акаунту", this);
    settingsMenu->addAction(logOut);
    ui->toolButton->setMenu(settingsMenu);
    connect(chooseDirectory,SIGNAL(triggered()), SLOT(setPermanentDirectory()));
    connect(settingAction,SIGNAL(triggered()), SLOT(showSettingsDialog()));
    connect(logOut, SIGNAL(triggered()), this, SLOT(logout()));
    connect(crossToPage, SIGNAL(triggered()),this,SLOT(crossingToPage()));
}


void MainWindow::slotSetArtistIcon(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QPixmap p;

    p.loadFromData(bytes);


    //qDebug() << "p.isNull() = " << p.isNull();
    if (p.isNull())
        ui->artistIconLabel->setPixmap(QPixmap(":/icon/icon/cassette-2-icon.png"));
    else
    {
        if (p.height() > 126) //scale image
            p = p.scaledToHeight(125, Qt::SmoothTransformation);
        ui->artistIconLabel->setPixmap(p);
    }
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    //Handler for keyboard event
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch(keyEvent->key())
        {
//        case Qt::Key_F5:
//            this->loadMyAudio();
//            break;

        case Qt::Key_Left:
            if (keyEvent->modifiers() & Qt::ControlModifier)
                this->previousTrack();
            break;

        case Qt::Key_Right:
            if (keyEvent->modifiers() & Qt::ControlModifier)
                this->nextTrack();
            break;

        case Qt::Key_Space:
            this->slotPlayPause();
            break;

        case Qt::Key_Z:
            qDebug() <<  "currentTrack.id = " << currentTrack.getAudioID() << currentTrack.getTrackName();
            qDebug() <<  "length = " <<playlistTrack.length();
            break;

        default:
            return false;
        }

    }
    return false;
}

void MainWindow::setPermanentDirectory()
{
    QString path = QFileDialog::getExistingDirectory (this, tr("Папка "), ".");
    if ( !path.isNull())
        fileDownloader->setParmanentDirrectory(path);

   // qDebug()<< "paht = "<< path;
}

void MainWindow::addTrack(QString artist, QString title, uint duration, QUrl url, QString audioID, long long lyricsId, QString ownerId)
{
    Track track(artist, title, duration,  url, audioID, lyricsId, ownerId);
    if (playlistTrack.indexOf(track) < 0){
        ui->playlistWidget->addItem(artist + "  -  " + title);
        playlistTrack << track;
        //playList << url;
        if(currentTrack.getAudioID() == audioID){
            currentRow  = playlistTrack.length() - 1;
            ui->playlistWidget->item(currentRow)->setBackgroundColor(QColor(170, 255, 255));
        }
    }
    //lyricsList << lyricsId;

}



void MainWindow::slotPlay()
{
    ui->playButton->setIcon(QPixmap(":/icon/icon/pause.png"));

    currentRow = ui->playlistWidget->currentRow();

    this->play();
}

void MainWindow::slotPlayPause()
{
    if (mediaObject->state() == Phonon::PlayingState) {
        mediaObject->pause();
    }
    else if (mediaObject->state() == Phonon::StoppedState )
        {
            qDebug() << "mediaObject->currentSoursace() = " << mediaObject->currentSource().url() ;
            qDebug() << "currentRow = " << currentRow;
            mediaObject->setCurrentSource(playlistTrack[currentRow].getUrl());
            enqueuePlaylist();
              mediaObject->play();
    } else if (mediaObject->state() == Phonon::PausedState){
        mediaObject->play();
    }
}

void MainWindow::slotEndParse()
{
    static bool firstRunning = true;
    enqueuePlaylist();

    if(firstRunning)
    {
        vk->getFriend();
        firstRunning = false;
    }

    qDebug() << "slotEndParse";
    connect(ui->playlistWidget->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollAudioValueChanged(int)));

//    qDebug() << playList;
}

void MainWindow::slotAuthorization()
{

    vk->start();

}

void MainWindow::failAuthorization()
{
    ui->trackLabel->setText("Cann't login.");
}

void MainWindow::successAuthorization()
{

    ui->playButton->setText("");
    enableButons(true);
    //TODO: block this button at the start
    connect(ui->nextButton, SIGNAL(clicked()),this, SLOT(nextTrack()));
    connect(ui->previousButton, SIGNAL(clicked()), this, SLOT(previousTrack()));

    //Disconnect authorization's buttons
//    disconnect(ui->playButton,SIGNAL(clicked()),this,SLOT(slotAuthorization()));
//    disconnect(ui->authorizationButton, SIGNAL(clicked()), this, SLOT(slotAuthorization()));
    disconnect(ui->myAudioButton, SIGNAL(clicked()), this, SLOT(slotAuthorization()));
//    connect(ui->authorizationButton,SIGNAL(clicked()), this, SLOT(supplementTracks()));

    connect(ui->myAudioButton, SIGNAL(clicked()), this, SLOT(loadMyAudio()),  Qt::UniqueConnection);

    /*Connecting signals from class VkApi with slot of this class */
    connect(vk,SIGNAL(signalAddTrack(QString,QString,uint,QUrl,QString, long long,QString)),
            this, SLOT(addTrack(QString,QString,uint,QUrl,QString,long long,QString)));
    connect (vk, SIGNAL(endFile()),this, SLOT(slotEndParse()));
     connect (vk, SIGNAL(endFrienParsingSgn()),this, SLOT(slotEndParsingFriend()));
    connect(vk,SIGNAL(fail()),this, SLOT(failAuthorization()));
    /*Connecting signals from class LastFmApi with slot of this class */
    connect(lastfmApi, SIGNAL(urlRecieved(QUrl)),this, SLOT(downloadArtistIcon(QUrl)));

    /*Connecting signals from class FileDownloader with slot of this class */
    connect(fileDownloader, SIGNAL(dataProgress(qint64,qint64)),this, SLOT(valueChanged(qint64,qint64)));
    connect(fileDownloader, SIGNAL(textChanged(QString)),this, SLOT(setTextProgressBar(QString)));

    /*Connecting signals from class DownloadManager with slot of this class */
    connect(downloadManager, SIGNAL(imageDownloaded(QNetworkReply*)),this, SLOT(slotSetArtistIcon(QNetworkReply*)));

    /*Connecting signals from this class with slot of this class */
    connect(ui->playlistWidget, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(slotPlay()));
    connect(ui->playButton, SIGNAL(clicked()),this, SLOT(slotPlayPause()));
    connect(ui->downloadButton, SIGNAL(clicked()),this, SLOT(downloadCurrentTrack()));
    //TODO
//    connect(ui->reloadButton, SIGNAL(clicked()), this, SLOT(loadMyAudio()));
    connect(ui->reloadButton, SIGNAL(clicked()), this, SLOT(shuffle()));
    //connect(ui->authorizationButton, SIGNAL(clicked()), this, SLOT(crossingToPage()));
    connect(ui->repeatTrackButton, SIGNAL(clicked(bool)), SLOT(repeatTrackSlot(bool)));
    connect(this, SIGNAL(artistChanged(QString)),this, SLOT(slotArtistChanged(QString)));


    connect (ui->broadcastButton, SIGNAL(clicked(bool)),this, SLOT(broadcastMusic(bool)));

    connect(ui->friendsListWidget, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(playFriendsMusic()));


    /*Set keyboard event for player*/
    ui->playlistWidget->installEventFilter(this);
    ui->volumeSlider->installEventFilter(this);
    ui->seekSlider->installEventFilter(this);


    //Set flag of repeating track
    repeatTrack = false;


    emit playerStateChanged(playerState, MY_AUDIO);
}

void MainWindow::finishedPlaylist()
{
    qDebug() << "finished";
    nextTrack();
}


void MainWindow::updateTime()
{
    long len = mediaObject->totalTime();
    long pos = mediaObject->currentTime();
    QString timeString;
    if (pos || len)
    {
        int sec = pos/1000;
        int min = sec/60;
        int hour = min/60;
        int msec = pos;

        QTime playTime(hour%60, min%60, sec%60, msec%1000);
        sec = len / 1000;
        min = sec / 60;
        hour = min / 60;
        msec = len;

        QTime stopTime(hour%60, min%60, sec%60, msec%1000);
        QString timeFormat = "m:ss";
        if (hour > 0)
            timeFormat = "h:mm:ss";
        timeString = playTime.toString(timeFormat);
        if (len)
            timeString += " / " + stopTime.toString(timeFormat);
    }
    ui->timeLabel->setText(timeString);

    //qDebug()<<mediaObject->bufferStatus();

}


void MainWindow::repeatTrackSlot(bool repeat)
{
    repeatTrack = repeat;
    if (repeatTrack){
        mediaObject->clearQueue();
        mediaObject->enqueue(mediaObject->currentSource());
//        mediaObject->enqueue(playlistTrack[currentRow].getUrl());
    } else {
        enqueuePlaylist();

    }
}

void MainWindow::logout()
{
    clearPlayer();
    mediaObject->stop();
    ui->artistIconLabel->setPixmap(QPixmap(":/icon/icon/cassette-2-icon.png"));
    ui->trackLabel->setText("");
    enableButons(false);
    disconnect(ui->myAudioButton, SIGNAL(clicked()), this, SLOT(loadMyAudio()));
    connect(ui->myAudioButton, SIGNAL(clicked()), this, SLOT(slotAuthorization()),  Qt::UniqueConnection);
//    mediaObject->stateChanged(
//    ui->t
//    ui->myAudioButton->
    vk->logout();
}

void MainWindow::updateInfo()
{
    qDebug()<< "mediaObject->state = " << mediaObject->state();
    qDebug() << "updateInfo()";
    switch(mediaObject->state())
    {
    case 0:
        qDebug()<< "mediaObject->state = Phonon::LoadingState";
        break;
    case 1:
        qDebug()<< "mediaObject->state = Phonon::StoppedState";
        break;
    case 2:
        qDebug()<< "mediaObject->state = Phonon::PlayingState";
        break;
    case 3:
        qDebug()<< "mediaObject->state = Phonon::BufferingState";
        break;
    case 4:
        qDebug()<< "mediaObject->state = Phonon::PausedState";
        break;

    case 5:
        qDebug()<< "mediaObject->state = Phonon::ErrorState";
        return;
        break;
    default:
        qDebug () << "Error: media object state doesnt exists";
    }

//    qDebug() << "mediaObject->currentSource()" << mediaObject->currentSource().url().toString();
//    qDebug() << ui->playlistWidget->item(currentRow)->text();

//    if (newState != Phonon::StoppedState && newState != Phonon::PausedState)
//             return;

//         if (metaInformationResolver->currentSource().type() == Phonon::MediaSource::Invalid)
//                 return;

    if (mediaObject->state() != Phonon::LoadingState)
    {
//        static uint previousTrackIndex = 0;
        if ((currentRow == previousTrackIndex)
                &&  (mediaObject->state() == Phonon::PlayingState) && !repeatTrack)
            currentRow++;

        //Set text to ui->trackLabel
        QString currentText = ui->playlistWidget->item(currentRow)->text();
        qDebug() << "currentText = " << currentText;
        currentTrack = playlistTrack[currentRow];

        qDebug() << "ui->playlistWidget->item(currentRow)->text()" << ui->playlistWidget->item(currentRow)->text();
        if (currentText.length() < 35)
            ui->trackLabel->setText(currentText);
        else
            ui->trackLabel->setText(currentText.mid(0,30) + "...");

        //Delete highlighting of previous track
        ui->playlistWidget->item(previousTrackIndex)->setBackgroundColor(QColor(255, 255, 255));
        //Highligth current track in playlist
        ui->playlistWidget->item(currentRow)->setBackgroundColor(QColor(170, 255, 255));
        previousTrackIndex = currentRow;

        //Emit signal for changing artist's icon
        emit artistChanged(currentText.mid(0, currentText.indexOf(" - ")));
    }
}

void MainWindow::setTextProgressBar(QString text)
{
    qDebug() << "setTextProgressBar  artistText = " << text;

    if (text.length() > 45)
        text = text.mid(0,45) + "...";
    qDebug() << "setTextProgressBar  artistText = " << text;
    ui->progressBar->setFormat(text);
    ui->progressBar->setToolTip("В черзі - " + QString::number(fileDownloader->getQueueLength()));
}

void MainWindow::nextTrack()
{
    if (currentRow != playlistTrack.size() - 1 )
        ++currentRow;
    else currentRow = 0;

    play();
}

void MainWindow::crossingToPage()
{
    QDesktopServices::openUrl(QUrl("http://vk.com/id" + vk->getUserId()));
}

void MainWindow::showLyrics()
{
//    qDebug() << "playlist[currentRow] = "<< playList[currentRow];

    long long int lyricsId = playlistTrack[ui->playlistWidget->currentIndex().row()].getLyricsID();

    if(lyricsId) //if lyrics of text exist
    {
        TextRepresent textRepresent; //create object for showing lyrics
        textRepresent.setHeader(ui->playlistWidget->currentItem()->text());
        textRepresent.setText(vk->getLyrics(lyricsId));
        textRepresent.show();
        textRepresent.exec();
    }
    else
    {
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
        QMessageBox msgBox;
        QFont font("Tahoma", 15);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle("Упс! :(");
        msgBox.setFont(font);
        msgBox.setText("Текст відсутній");
        msgBox.exec();
        qDebug() << "text is not available";
    }
}

void MainWindow::slotArtistChanged(QString artist)
{
    lastfmApi->requestArtistIcon(artist);
//    if (broadcastTrack)
//        broadcastMusic(true);
}

void MainWindow::downloadArtistIcon(QUrl url)
{
    downloadManager->sendRequest(url,DownloadManager::IMAGE);
}

void MainWindow::previousTrack()
{
    if (currentRow)
        currentRow--;

    play();
}

void MainWindow::downloadCurrentTrack()
{
   //TODO: Test download current Track
    qDebug() << "downloadCurrentTrack ui->playlistWidget->currentRow() = " << ui->playlistWidget->currentRow();
    if (ui->playlistWidget->currentRow() != -1) //if playlist wasn't played
        fileDownloader->downloadFile(playlistTrack[ui->playlistWidget->currentRow()]);
    else fileDownloader->downloadFile(playlistTrack[0]);//download first
    // TODO:get text from playlistWidget
    isFreeForDownloading = true;
}

void MainWindow::downloadSelectedTrack()
{
//    qDebug() << ui->playlistWidget->currentItem()->text() << " =|=  "<<
//                                              playList[ui->playlistWidget->currentIndex().row()];
    qint64 selectedTrack = ui->playlistWidget->currentRow();
    fileDownloader->downloadFile(playlistTrack[selectedTrack]);
    isFreeForDownloading = true;
}

void MainWindow::valueChanged(qint64 a, qint64 b)
{
    if (isFreeForDownloading)
    {
        ui->progressBar->show();
        ui->progressBar->setMaximum(b);
        isFreeForDownloading = false;
        return;
    }

    if (a == b)
    {
        isFreeForDownloading = true;
        if (fileDownloader->isQueueEmpty())
            ui->progressBar->hide();
    }

    ui->progressBar->setValue(a);
}

void MainWindow::loadMyAudio()
{
    static int i= 0;
    qDebug() << "loadMyAudio" << ++i;
    ui->friendsListWidget->hide();
    qint64 countTrack = ui->playlistWidget->count();
    if(countTrack == 0){
        countTrack = 50;
    }
    clearPlayer();
//    playerState = MY_AUDIO;
    emit playerStateChanged(playerState, MY_AUDIO);

    vk->getAudio(countTrack);
//    contexMyAudioMenu();
}

void MainWindow::aboutToFinish()
{
    qDebug() << "aboutToFinish";
    if (repeatTrack)
    {
        mediaObject->enqueue(playlistTrack[currentRow].getUrl());
       mediaObject->play();
    }
}



void MainWindow::stateChanged(Phonon::State newState, Phonon::State oldState)
{
    qDebug() << QPixmap(":/icon/icon/play.png").isNull() ;
    switch (newState) {
        case Phonon::ErrorState:
            if (mediaObject->errorType() == Phonon::FatalError) {
                QMessageBox::warning(this, tr("Fatal Error"),
                mediaObject->errorString());
            } else {
                QMessageBox::warning(this, tr("Error"),
                mediaObject->errorString());
            }
            break;

        case Phonon::PlayingState:
         ui->playButton->setIcon(QPixmap(":/icon/icon/pause.png"));

                break;
        case Phonon::StoppedState:
        case Phonon::PausedState:
       ui->playButton->setIcon(QPixmap(":/icon/icon/play.png"));
                break;

        case Phonon::BufferingState:
        qDebug()<<"Buffering State";
                break;
        default:
            ;
    }
}

void MainWindow::supplementTracks()
{
    qDebug() << "suplementTrack";
    switch (playerState)
    {
    case MY_AUDIO:
        vk->getAudio(50, ui->playlistWidget->count() + 1);
        break;

    case SUGGESTED_AUDIO:
//        vk->search(100, ui->playlistW/*idget->count());*/
        vk->audioSuggest(50,ui->playlistWidget->count() + 1);
        break;

    case SEARCH_AUDIO:
         vk->audioSearch(ui->searchLineEdit->text(),30,ui->playlistWidget->count() + 1);
        break;

    case FRIENDS_AUDIO:
         vk->getAudio(50, ui->playlistWidget->count() + 1, userList[ui->friendsListWidget->currentRow()].getUserID());
        break;

    default:
        qDebug () << "Eroor: player state changed";;

    }


}

void MainWindow::broadcastMusic(bool boolBroadcast)
{
    qDebug() << boolBroadcast;
    vk->setBroadcast(boolBroadcast);
    broadcastTrack = boolBroadcast;
    if (broadcastTrack)
        vk->setAudioStatus(playlistTrack[currentRow].getAudioID());
    else
        vk->setTextStatus();
}


void MainWindow::sourceChanged(const Phonon::MediaSource &source)
{
    qDebug () << "source changed ";
}

void MainWindow::searchInVK()
{
    ui->friendsListWidget->hide();
    emit playerStateChanged(playerState,SEARCH_AUDIO);
    clearPlayer();
    vk->audioSearch(ui->searchLineEdit->text());
}

void MainWindow::suggestAudio()
{
    clearPlayer();
    ui->friendsListWidget->hide();
    emit playerStateChanged(playerState,SUGGESTED_AUDIO);
    vk->audioSuggest();
}

void MainWindow::clearPlayer()
{
    playlistTrack.clear();
    currentRow = 0;
    ui->playlistWidget->clear();
}



void MainWindow::editTrack()
{
//    ui->playlistWidget->currentRow()
//    ui->playlistWidget
    EditTrack editWindow(vk->getAccessToken(), vk->getUserId());
    connect(&editWindow, SIGNAL(trackEdited(Track)), this, SLOT(trackWasEdited(Track)));
    Track currentTrack = playlistTrack[ui->playlistWidget->currentRow()];
    editWindow.editTrack(currentTrack);
//    QString str = vk->editTrack(currentTrack.getAudioID(), currentTrack.getArtist(), currentTrack.getTitle());
//    qDebug() << str;
    editWindow.show();
    editWindow.exec();
}

void MainWindow::trackWasEdited(Track track)
{
    qint32 currentRow = ui->playlistWidget->currentRow();
    QListWidgetItem* item = ui->playlistWidget->takeItem(currentRow);
    delete item;
    ui->playlistWidget->insertItem(currentRow, track.getArtist() + "  -  " + track.getTitle());
//    ui->playlistWidget->setCurrentItem(new QListWidgetItem(track.getArtist() + "  -  " + track.getTitle()));
    playlistTrack[currentRow] = track;
}

void MainWindow::playerStateChangedSlot(PlayerState oldState, PlayerState newState)
{
    if (oldState == newState)
        return; //don't change the contex menu

    playerState = newState;
    clearContexMenu(ui->playlistWidget->actions());

    switch (newState)
    {
    case MY_AUDIO:
        ui->playlistWidget->addActions(myAudioActions);
        break;

    case SEARCH_AUDIO:
    case FRIENDS_AUDIO:
    case SUGGESTED_AUDIO:
        ui->playlistWidget->addActions(suggestionActions);
        break;



    default:
        qDebug () << "Eroor: player state changed";;

    }
}

void MainWindow::createActions()
{
    showLyricsAction = new QAction("Текст пісні", this);
    connect(showLyricsAction, SIGNAL(triggered()), this, SLOT(showLyrics()));
    showLyricsAction->setIcon(QIcon(":icon/icon/showText.png"));

    downloadAction = new QAction("Завантажити", this);
    downloadAction->setIcon(QIcon(":icon/icon/save.png"));
    connect(downloadAction, SIGNAL(triggered()), this, SLOT(downloadSelectedTrack()));

    editAction = new QAction("Редагувати", this);
//    downloadAction->setIcon(QIcon("icon/save.png"));
    connect(editAction, SIGNAL(triggered()), this, SLOT(editTrack()));


    addAudioToPageAct = new QAction("Додати на сторінку", this);
    //    downloadAction->setIcon(QIcon("icon/save.png"));
    connect(addAudioToPageAct, SIGNAL(triggered()), this, SLOT(addAudioToPage()));

    deleteAudioAct = new QAction("Видалити", this);
    //    downloadAction->setIcon(QIcon("icon/save.png"));
    connect(deleteAudioAct, SIGNAL(triggered()), this, SLOT(deleteAudio()));

    QAction *rateSimilarityAct = new QAction("Відсоток схожості аудіо", this);
//    connect(rateSimilarityAct, SIGNAL(triggered()), this, SLOT(showSimilarity()));
    connect(rateSimilarityAct, SIGNAL(triggered()), this, SLOT(showSimilarity()));
    ui->friendsListWidget->addAction(rateSimilarityAct);
}

void MainWindow::createMenus()
{
    myAudioActions << showLyricsAction << downloadAction << editAction << deleteAudioAct;
    suggestionActions << showLyricsAction << downloadAction << addAudioToPageAct;
}

void MainWindow::loadFriendsMusic()
{
    ui->friendsListWidget->show();

    emit playerStateChanged(playerState,FRIENDS_AUDIO);
    if (userList.isEmpty())
        vk->getFriend();
}

void MainWindow::clearContexMenu(QList<QAction*> list)
{
    foreach( QAction* action, list)
    {
        ui->playlistWidget->removeAction(action);
    }
}

void MainWindow::addAudioToPage()
{
    Track selectedTrack = playlistTrack[ui->playlistWidget->currentRow()];
    vk->addAudioToPage(selectedTrack.getAudioID(), selectedTrack.getOwnerId());
}

void MainWindow::deleteAudio()
{
    qint64 selectedRow = ui->playlistWidget->currentRow();
    vk->deleteAudio(playlistTrack[selectedRow].getAudioID());
    QListWidgetItem* item = ui->playlistWidget->takeItem(selectedRow);
    delete item;
    playlistTrack.removeAt(selectedRow);
    enqueuePlaylist();
}

void MainWindow::friendsAdded(User user)
{
    QListWidgetItem *item = new QListWidgetItem(user.getFirstName() + " " + user.getLastName());
    QByteArray bytes = downloadManager->getWithWaiting(QNetworkRequest(user.getPhotoUrl()))->readAll();
    QPixmap p;
    p.loadFromData(bytes);
    userList << user;

    item->setIcon(p);
    ui->friendsListWidget->addItem(item);
}

void MainWindow::supplementFriend()
{
    vk->getFriend(25,ui->friendsListWidget->count() + 1);
}

void MainWindow::playFriendsMusic()
{
    clearPlayer();
    vk->getAudio(50,0,userList[ui->friendsListWidget->currentRow()].getUserID());
}

void MainWindow::updateStatus()
{
    if (broadcastTrack)
     {
         qDebug() << playlistTrack[currentRow].getAudioID();
         vk->setAudioStatus( playlistTrack[currentRow].getAudioID());

    }
}

void MainWindow::scrollAudioValueChanged(int value)
{
//    qDebug() << "scroll = " << value << (value == ui->playlistWidget->verticalScrollBar()->maximum());
    if(value == ui->playlistWidget->verticalScrollBar()->maximum()){
        disconnect(ui->playlistWidget->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollAudioValueChanged(int)));
        this->supplementTracks();
    }
}

void MainWindow::shuffle()
{
    disconnect(ui->reloadButton, SIGNAL(clicked()), this, SLOT(shuffle()));
    qSort(this->playlistTrack.begin(), this->playlistTrack.end(), Utils::randomSortTrack);

    ui->playlistWidget->clear();
    foreach(Track track, this->playlistTrack){
        ui->playlistWidget->addItem(track.getArtist() + "  -  " + track.getTitle());
        if(currentTrack.getAudioID() == track.getAudioID()){
            currentRow  = playlistTrack.indexOf(currentTrack);
            qDebug() << "currentRow" << currentRow;
            ui->playlistWidget->item(currentRow)->setBackgroundColor(QColor(170, 255, 255));
            previousTrackIndex = currentRow;
        }
    }
    connect(ui->reloadButton, SIGNAL(clicked()), this, SLOT(shuffle()));
}

void MainWindow::slotEndParsingFriend()
{
    qDebug() << "endParsing";
    connect(ui->friendsListWidget->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollFriendValueChanged(int)));
}

void MainWindow::scrollFriendValueChanged(int value)
{
//    qDebug() << "scroll = " << value << (value == ui->friendsListWidget->verticalScrollBar()->maximum());
    if(value == ui->friendsListWidget->verticalScrollBar()->maximum()){
        disconnect(ui->friendsListWidget->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollFriendValueChanged(int)));
        this->supplementFriend();
    }
}


void MainWindow::showSettingsDialog()
{

    qDebug() << "test" ;
    if (!settingsDialog)
        initSettingsDialog();

    int currentEffect = settingsUi->audioEffectsCombo->currentIndex();

    settingsDialog->exec();

    qDebug() << "result " << settingsDialog->result();
    if (settingsDialog->result() == QDialog::Accepted){
        QList<Phonon::AudioOutputDevice> devices = Phonon::BackendCapabilities::availableAudioOutputDevices();
//        audioOutput->setOutputDevice(devices[settingsUi->deviceCombo->currentIndex()]);
        QList<Phonon::Effect *> currEffects = audioOutputPath.effects();
        QList<Phonon::EffectDescription> availableEffects = Phonon::BackendCapabilities::availableAudioEffects();

        if (settingsUi->audioEffectsCombo->currentIndex() > 0){
            Phonon::Effect *currentEffect = currEffects.size() ? currEffects[0] : 0;
            if (!currentEffect || currentEffect->description() != nextEffect->description()){
                foreach(Phonon::Effect *effect, currEffects) {
                    audioOutputPath.removeEffect(effect);
                    delete effect;
                }
                audioOutputPath.insertEffect(nextEffect);
            }
        } else {
            foreach(Phonon::Effect *effect, currEffects) {
                audioOutputPath.removeEffect(effect);
                delete effect;
                nextEffect = 0;
            }
        }
    }
}



void MainWindow::initSettingsDialog()
{
    qDebug() << "init";
    settingsDialog = new QDialog(this);
    settingsUi = new Ui_settings();
    settingsUi->setupUi(settingsDialog);

    connect(settingsUi->effectButton, SIGNAL(clicked()), this, SLOT(configureEffect()));

//     Insert audio devices:
    QList<Phonon::AudioOutputDevice> devices = Phonon::BackendCapabilities::availableAudioOutputDevices();
    for (int i=0; i<devices.size(); i++){
        QString itemText = devices[i].name();
        if (!devices[i].description().isEmpty()) {
            itemText += QString::fromLatin1(" (%1)").arg(devices[i].description());
        }
        settingsUi->deviceCombo->addItem(itemText);
        if (devices[i] == audioOutput->outputDevice())
            settingsUi->deviceCombo->setCurrentIndex(i);
    }

    // Insert audio effects:
    settingsUi->audioEffectsCombo->addItem(tr("<no effect>"));
    QList<Phonon::Effect *> currEffects = audioOutputPath.effects();
    Phonon::Effect *currEffect = currEffects.size() ? currEffects[0] : 0;
    QList<Phonon::EffectDescription> availableEffects = Phonon::BackendCapabilities::availableAudioEffects();
    for (int i=0; i<availableEffects.size(); i++){
        settingsUi->audioEffectsCombo->addItem(availableEffects[i].name());
        if (currEffect && availableEffects[i] == currEffect->description())
            settingsUi->audioEffectsCombo->setCurrentIndex(i+1);
    }
    connect(settingsUi->audioEffectsCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(effectChanged()));
}

void MainWindow::effectChanged()
{
    qDebug() << "effectChanged";
    int currentIndex = settingsUi->audioEffectsCombo->currentIndex();
    if (currentIndex) {
        QList<Phonon::EffectDescription> availableEffects = Phonon::BackendCapabilities::availableAudioEffects();
        Phonon::EffectDescription chosenEffect = availableEffects[currentIndex - 1];

        QList<Phonon::Effect *> currEffects = audioOutputPath.effects();
        Phonon::Effect *currentEffect = currEffects.size() ? currEffects[0] : 0;

        // Deleting the running effect will stop playback, it is deleted when removed from path
        if (nextEffect && !(currentEffect && (currentEffect->description().name() == nextEffect->description().name())))
            delete nextEffect;

        nextEffect = new Phonon::Effect(chosenEffect);
    }
    settingsUi->effectButton->setEnabled(currentIndex);
}

void MainWindow::configureEffect()
{
    if (!nextEffect)
        return;


    QList<Phonon::Effect *> currEffects = audioOutputPath.effects();
    const QList<Phonon::EffectDescription> availableEffects = Phonon::BackendCapabilities::availableAudioEffects();
    if (settingsUi->audioEffectsCombo->currentIndex() > 0) {
        Phonon::EffectDescription chosenEffect = availableEffects[settingsUi->audioEffectsCombo->currentIndex() - 1];

        QDialog effectDialog;
        effectDialog.setWindowTitle(tr("Configure effect"));
        QVBoxLayout *topLayout = new QVBoxLayout(&effectDialog);

        QLabel *description = new QLabel("<b>Description:</b><br>" + chosenEffect.description(), &effectDialog);
        description->setWordWrap(true);
        topLayout->addWidget(description);

        QScrollArea *scrollArea = new QScrollArea(&effectDialog);
        topLayout->addWidget(scrollArea);

        QVariantList savedParamValues;
        foreach(Phonon::EffectParameter param, nextEffect->parameters()) {
            savedParamValues << nextEffect->parameterValue(param);
        }

        QWidget *scrollWidget = new Phonon::EffectWidget(nextEffect);
        scrollWidget->setMinimumWidth(320);
        scrollWidget->setContentsMargins(10, 10, 10,10);
        scrollArea->setWidget(scrollWidget);

        QDialogButtonBox *bbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &effectDialog);
        connect(bbox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), &effectDialog, SLOT(accept()));
        connect(bbox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), &effectDialog, SLOT(reject()));
        topLayout->addWidget(bbox);

        effectDialog.exec();

        if (effectDialog.result() != QDialog::Accepted) {
            //we need to restore the parameters values
            int currentIndex = 0;
            foreach(Phonon::EffectParameter param, nextEffect->parameters()) {
                nextEffect->setParameterValue(param, savedParamValues.at(currentIndex++));
            }
//            mediaObject->play();
        }
    }
}


void MainWindow::addTrackToFriendsList(QString artist, QString title, uint duration, QUrl url, QString audioID, long long lyricsId, QString ownerId)
{
    if (friendsAudioList.indexOf(Track(artist, title, duration,  url, audioID, lyricsId, ownerId)) < 0){
        friendsAudioList << Track(artist, title, duration,  url, audioID, lyricsId, ownerId);
    }
}

void MainWindow::calculateSimilarity()
{
    disconnect(vk,SIGNAL(signalAddTrack(QString,QString,uint,QUrl,QString, long long,QString)),
            this, SLOT(addTrackToFriendsList(QString,QString,uint,QUrl,QString,long long,QString)));
    disconnect (vk, SIGNAL(endFile()),this, SLOT(calculateSimilarity()));
    uint countMutual = 0;
    foreach(Track myTrack, myAudioList){
        foreach(Track friendTrack, friendsAudioList){
            if (myTrack == friendTrack)
                countMutual++;
        }
//        qDebug() << myTrack.getAudioID();
    }
    connect(vk,SIGNAL(signalAddTrack(QString,QString,uint,QUrl,QString, long long,QString)),
            this, SLOT(addTrack(QString,QString,uint,QUrl,QString,long long,QString)));
    connect (vk, SIGNAL(endFile()),this, SLOT(slotEndParse()));

    qDebug() << "count " << countMutual;
    uint currentIndex = ui->friendsListWidget->currentIndex().row();

    double rate =  double(countMutual) / myAudioList.length() * 100;

    qDebug() << "rate = " << rate << myAudioList.length();

    similarityMsgBox.setText("Інформація про спільні аудіозаписи з користувачем\n" + userList[currentIndex].getFirstName() + " "+ userList[currentIndex].getLastName() + "\n"
                             + "Кількість спільних треків: " + QString::number(countMutual) + "\n"
                             + "Відсоток спільних аудіо: " +  QString::number(int(rate*100 + 0.5)/100.0) + "%");

    friendsAudioList.clear();
}

void MainWindow::addTrackToMyList(QString artist, QString title, uint duration, QUrl url, QString audioID, long long lyricsId, QString ownerId)
{
    myAudioList << Track(artist, title, duration,  url, audioID, lyricsId, ownerId);
}

void MainWindow::fillFriendList()
{
    qDebug() << "fillFriendList";
    uint currentIndex = ui->friendsListWidget->currentIndex().row();
    disconnect(vk,SIGNAL(signalAddTrack(QString,QString,uint,QUrl,QString, long long,QString)),
            this, SLOT(addTrackToMyList(QString,QString,uint,QUrl,QString,long long,QString)));
    disconnect (vk, SIGNAL(endFile()),this, SLOT(fillFriendList()));
    connect(vk,SIGNAL(signalAddTrack(QString,QString,uint,QUrl,QString, long long,QString)),
            this, SLOT(addTrackToFriendsList(QString,QString,uint,QUrl,QString,long long,QString)));
    connect (vk, SIGNAL(endFile()),this, SLOT(calculateSimilarity()));
    vk->getAudio(0,0,userList[currentIndex].getUserID());
}

void MainWindow::showSimilarity()
{
    qDebug() << "test";



    initSimilarityDialog();

//    similarityDialog->exec();
    disconnect(vk,SIGNAL(signalAddTrack(QString,QString,uint,QUrl,QString, long long,QString)),
            this, SLOT(addTrack(QString,QString,uint,QUrl,QString,long long,QString)));
    disconnect (vk, SIGNAL(endFile()),this, SLOT(slotEndParse()));
    connect(vk,SIGNAL(signalAddTrack(QString,QString,uint,QUrl,QString, long long,QString)),
            this, SLOT(addTrackToMyList(QString,QString,uint,QUrl,QString,long long,QString)));
    connect (vk, SIGNAL(endFile()),this, SLOT(fillFriendList()));
    if (myAudioList.empty()) {
        vk->getAudio(0,0); //get My Audio
    } else {
        fillFriendList();
    }
}

void MainWindow::initSimilarityDialog()
{
    similarityMsgBox.setWindowTitle("Схожість аудіозаписів");
    similarityMsgBox.setText("Обраховується схожість. Почекайте, будь-ласка. :)");
    similarityMsgBox.setStandardButtons(QMessageBox::Ok);
    similarityMsgBox.setModal(false);
    similarityMsgBox.setWindowModality(Qt::NonModal);
    similarityMsgBox.show();
    //    similarityMsgBox.exec();
}

void MainWindow::enableButons(bool value)
{
    ui->repeatTrackButton->setEnabled(value);
    ui->downloadButton->setEnabled(value);
    ui->reloadButton->setEnabled(value);
    ui->previousButton->setEnabled(value);
    ui->nextButton->setEnabled(value);
    ui->playButton->setEnabled(value);
    ui->myFrAudioButton->setEnabled(value);
    ui->suggestionButton->setEnabled(value);
    ui->toolButton->setEnabled(value);
}



