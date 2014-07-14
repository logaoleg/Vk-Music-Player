#include <QApplication>
#include <QCoreApplication>
#include "mainwindow.h"
#include <QObject>
#include<QTextCodec>
#include <QStyleFactory>
//bla bla 
#include <QStyle>

int main(int argc, char** argv)
{

    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("VK Music Player");
    QCoreApplication::setApplicationVersion("0.3.1");
    QCoreApplication::setOrganizationName("logaoleg");
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));    ///< Для рядкових констант
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));        ///< Для функцій переводу tr()

    MainWindow VkMainWindow;
    VkMainWindow.show();

    return app.exec();
}


