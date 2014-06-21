#ifndef TEST_PROJECT_H
#define TEST_PROJECT_H

#include <QObject>
#include <QTest>
#include "track.h";
#include "user.h"


class test_project : public QObject
{
    Q_OBJECT
public:
    explicit test_project(QObject *parent = 0);

signals:

public slots:


private slots:
    void trackNameCheck();
    void testUser();


};

#endif // TEST_PROJECT_H
