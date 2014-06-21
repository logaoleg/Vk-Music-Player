TEMPLATE     = app
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    vkapi.cpp \
    musicparser.cpp \
    textrepresent.cpp \
    filedownloader.cpp \
    downloadmanager.cpp \
    lastfmapi.cpp \
    track.cpp \
    trackdownload.cpp \
    edittrack.cpp \
    user.cpp \
    friendparser.cpp \
    utils.cpp \
    test_project.cpp

HEADERS += \
    mainwindow.h \
    vkapi.h \
    musicparser.h \
    textrepresent.h \
    filedownloader.h \
    downloadmanager.h \
    lastfmapi.h \
    track.h \
    trackdownload.h \
    edittrack.h \
    user.h \
    friendparser.h \
    utils.h \
    test_project.h

FORMS += \
    mainwindow.ui \
    textrepresent.ui \
    edittrack.ui \
    settings.ui

QT += webkit \
    network \
    xml \
    phonon\
    testlib

OTHER_FILES += \
    ../VK_API-build-desktop-Qt_4_7_4_for_Desktop_-_MinGW_4_4__Qt_SDK_________/out.xml \
    style.qss

RESOURCES += \
    resourse.qrc
































