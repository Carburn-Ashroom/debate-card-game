#ifndef MAINWINDOW_CHOOSE_H
#define MAINWINDOW_CHOOSE_H

#include <QMainWindow>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QMessageBox>

#include "heap.h"
#include "button.h"
#include "choose.h"

namespace Ui {
class MainWindow_choose;
}

class MainWindow_choose : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_choose(QWidget *parent = nullptr);
    ~MainWindow_choose();
    void show(const QList<Heap::Pair>& p);      // 显示窗口，选择完毕后变动chs_id
    int choose();       // 获取选择状态

protected:
    void paintEvent(QPaintEvent* event);

    void mousePressEvent(QMouseEvent* event);

    void mouseReleaseEvent(QMouseEvent* event);

    void closeEvent(QCloseEvent* event);

private slots:
    void frame_update();        // 更新每一帧

private:
    Ui::MainWindow_choose *ui;
    QTimer timer;
    Chs::Choose chs_cards;
    QPixmap snapshot;
    Btn::Button ok {QPoint{(chs_sz.width()-btn_sz.width())/2,btn_ok_ypos},tr("确定")};
    bool if_chs;
};

#endif // MAINWINDOW_CHOOSE_H
