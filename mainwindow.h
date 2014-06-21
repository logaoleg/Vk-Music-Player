#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QMessageBox>
#include <QUrl>
#include <phonon/MediaObject>
#include <phonon/AudioOutput>
#include <QApplication>
#include <QDesktopWidget>
#include <QNetworkReply>
#include <QKeyEvent>
#include <QDesktopServices>
#include <QDir>
#include <QDebug>
#include <QTime>
#include <QtWebKit/QWebView>
#include <QMenu>
#include <QScrollArea>
#include <QDialogButtonBox>
#include <QtNetwork/QNetworkAccessManager>
#include <QScrollBar>
#include <phonon/backendcapabilities.h>
#include <phonon/effect.h>
#include <phonon/effectparameter.h>
#include <phonon/effectwidget.h>

#include "vkapi.h"
#include "user.h"
#include "filedownloader.h"
#include "downloadmanager.h"
#include "lastfmapi.h"
#include "track.h"
#include "textrepresent.h"
#include "edittrack.h"
#include "utils.h"

QT_BEGIN_NAMESPACE
class Ui_settings;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    enum PlayerState
    {
        MY_AUDIO = 0,
        FRIENDS_AUDIO,
        USERS_AUDIO,
        SUGGESTED_AUDIO,
        SEARCH_AUDIO,
        NO_STATE
    };

    explicit MainWindow(QWidget *parent = 0);


    void writeSettings(); //Wtite setting of player
    void readSettings (); //Read setting of player
    ~MainWindow();


private:
    /******Variables******/
    Ui::MainWindow *ui;
    Track currentTrack;
    QAction *showLyricsAction;
    QAction *downloadAction;
    QAction *editAction;
    QAction *addAudioToPageAct;
    QAction *deleteAudioAct;
    QList<QAction*> myAudioActions;
    QList<QAction*> suggestionActions;
    PlayerState playerState;
    Phonon::Path audioOutputPath;
    QList <Track> playlistTrack;
    QList <Track> myAudioList;
    QList <Track> friendsAudioList;
    QList <User> userList;
    QDialog *settingsDialog;
    Ui_settings *settingsUi;
    QMessageBox similarityMsgBox;

    QMenu *settingsMenu;
    //QList <QUrl> playList; //list of url of track
    //QList <long long> lyricsList; //lost of lyrics of track
    Phonon::AudioOutput *audioOutput;
    Phonon::MediaObject *mediaObject;
    VkApi *vk; //object for interaction with vk.com
    LastFmApi *lastfmApi; // ofject for interaction with last.fm
    qint64 currentRow; //current position in playlist
    QSettings settings;
    Phonon::Effect *nextEffect;
    FileDownloader *fileDownloader; // object for downloading file from vk.com
    DownloadManager *downloadManager; //object for download information
    bool isFreeForDownloading; //flags for checking queue of downloadin
    bool repeatTrack;
    bool broadcastTrack;
    uint previousTrackIndex;

    /*******Methods*******/
    void enqueuePlaylist(); //shange the order of Phonon's playlist
    void setStyle(QString fname);//set style of player from CSS file
    void setGuiSettings(); //set configuration of GUI
    void play();
    void setButtonsToolTip(); // set tool tip on buttons
    void createSettingsMenu(); //create menu on tool button
    void clearPlayer();
    void initEffectsMenu();
    void initSettingsDialog();
    void initSimilarityDialog();
    void enableButons(bool value);


//    void contexMyAudioMenu();
    void createActions();
    void createMenus();
    void clearContexMenu(QList<QAction*> list);




protected:
    bool eventFilter(QObject *object, QEvent *event); //handler of keyboard event

signals:
    void artistChanged(QString );
    void playerStateChanged(PlayerState oldState, PlayerState newState);

/******Slots******/
public slots:
    void addTrack(QString artist, QString title, uint duration, QUrl url, QString audioID, long long lyricsId, QString ownerId); //addTrack to playlist
    void addTrackToFriendsList(QString artist, QString title, uint duration, QUrl url, QString audioID, long long lyricsId, QString ownerId); //addTrack to playlist
    void addTrackToMyList(QString artist, QString title, uint duration, QUrl url, QString audioID, long long lyricsId, QString ownerId); //addTrack to playlist
    void broadcastMusic(bool boolBroadcast);

    void logout(); //logout from vk.com

    void nextTrack(); // play next track in queue
    void previousTrack();  //play previous track in queue
    void setPermanentDirectory(); //Set permanent dirrectory for downloading track
    void crossingToPage(); // cross to vk.com

     void stateChanged(Phonon::State newState, Phonon::State oldState);


     void playerStateChangedSlot(PlayerState oldState, PlayerState newState);
     void slotSetArtistIcon(QNetworkReply * reply);////set icon of artist from last.fm to artistIconLabel

private slots:
    void setTextProgressBar(QString text);
    void valueChanged(qint64 a, qint64 b);
    void trackWasEdited(Track track);
    void sourceChanged(const Phonon::MediaSource &source);
    void searchInVK();
    void editTrack();

    void fillFriendList();
    void showSettingsDialog();
//    void showSimilarity();
    void showSimilarity();
    void calculateSimilarity();

    void scrollAudioValueChanged(int value);
    void scrollFriendValueChanged(int value);

    void downloadCurrentTrack();
    void downloadSelectedTrack();
    void suggestAudio();
    void slotPlay();  //Play track. Invoked by double clicking on track in playlist
    void slotPlayPause(); //Play or pause playing of track. Invoked by clicking on play/pause button.

    void loadFriendsMusic();
    void updateInfo(); //update info about track
    void loadMyAudio(); // update playlist
    void aboutToFinish();
    void supplementTracks();
    void slotAuthorization(); //Invoke authorization in vk.com
    void failAuthorization(); //Invoke whet authoriation in vk.com was failed
    void successAuthorization();//Invoke when authorization was succeeded
    void showLyrics(); //show lyrics of text from vk.com
    void slotEndParse(); //After ending of parsing music from vk.com, set list of tracks in playlist
    void slotArtistChanged(QString artist);
    void downloadArtistIcon(QUrl url);

    void effectChanged();

    void deleteAudio();
    void finishedPlaylist(); //Invoke when Phonon's playlist ended.
    void updateTime();
    void repeatTrackSlot(bool repeat);

    void addAudioToPage();
    void updateStatus();

    void shuffle();
    void friendsAdded(User user);
    void supplementFriend();
    void slotEndParsingFriend();
    void configureEffect();
    void playFriendsMusic();


};

#endif // MAINWINDOW_H
