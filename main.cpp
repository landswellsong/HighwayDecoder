#include <QtGui/QApplication>
#include <QtXml/QDomDocument>
#include <QtCore/QFile>
#include <QTranslator>
#include <QLocale>
#include "mainwindow.h"
#include "quizwidget.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QString langcode=QLocale::system().name().split("_").first();
    
    QTranslator qtTranslator;
    qtTranslator.load("qt_"+langcode);
    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load("highwaydecoder_"+langcode);
    a.installTranslator(&myappTranslator);
    
    MainWindow w;
    w.show();
    return a.exec();
}
