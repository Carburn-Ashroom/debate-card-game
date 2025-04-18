/**
 * 辩论游戏
 * 帝成内置辩论游戏的翻版
 * Qt（C++）编写，MinGW-w64编译
 * 作者：Carburn Ashroom
 */

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
