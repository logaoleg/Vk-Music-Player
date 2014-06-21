#include "test_project.h"

test_project::test_project(QObject *parent) :
    QObject(parent)
{
}


void test_project::trackNameCheck()
{
    /*
    <aid>270330365</aid>
    <owner_id>13726527</owner_id>
    <artist>Guns N' Roses</artist>
    <title>This I Love</title>
    <duration>334</duration>
<url>
http://cs9-4v4.vk.me/p5/6703e5c7a5c538.mp3?extra=Z4t4dcDmUvcyIbiE4i-ZhUgnTkP_iBAWA_NeRjAd8fuRsxuFIU49ffKjfJMjnRrhCA0YV0iIsGWh91YLNmlTWO0zx0OyhgH9
</url>
<lyrics_id>3387609</lyrics_id>
<genre>1</genre>
    */

    Track track(QString("Guns N' Roses"), QString("This I Love"),334 ,
    QUrl("http://cs9-4v4.vk.me/p5/6703e5c7a5c538.mp3?extra=Z4t4dcDmUvcyIbiE4i-ZhUgnTkP_iBAWA_NeRjAd8fuRsxuFIU49ffKjfJMjnRrhCA0YV0iIsGWh91YLNmlTWO0zx0OyhgH9"),
    QString(270330365), 3387609, QString(13726527));
    QCOMPARE(track.getArtist(),QString("Gu ns N' Roses"));
    QCOMPARE(track.getTitle(),QString("This I Love"));
    QCOMPARE(track.getTrackName(), track.getArtist() + " - " + track.getTitle());
    QCOMPARE(int(track.getDuration()),  int (334));
}

void test_project::testUser()
{
    User user(QString("Oleg"), QString("Logazyak"), QString (123));

    QCOMPARE(user.getFirstName(),QString("Oleg"));
    QCOMPARE(user.getLastName(),QString("Logazyak"));
    QCOMPARE(user.getUserID(), QString (13));
}
