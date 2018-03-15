#include "mainwindow.h"
#include "glwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
       GLWidget wt;
       wt.show();
    return a.exec();
}
