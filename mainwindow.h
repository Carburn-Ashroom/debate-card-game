#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPixmap>
#include <QPaintEvent>
#include <random>
#include <QMouseEvent>
#include <QMessageBox>
#include <QCloseEvent>

#include "card.h"
#include "heap.h"
#include "button.h"
#include "mainwindow_choose.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using std::uniform_int_distribution;

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent* event);

    void mousePressEvent(QMouseEvent* event);

    void mouseReleaseEvent(QMouseEvent* event);

    void closeEvent(QCloseEvent* event);

private slots:
    void frame_update();        // 更新每一帧

private:
    Ui::MainWindow *ui;
    QTimer timer;
    Heap::Slot slot;
    Heap::Hand player;
    Heap::Hand enemy {true};
    Heap::Deal dealling;
    QPixmap snapshot;
    Cards::Card now_card {1,2};
    int stay;
    C_state have_nowc;
    State state;
    void start_game();      // 重开游戏
    void get_card();        // 抽一张卡
    void recover_card();        // 收卡
    Btn::Button getcard {btn_get, tr("抽一张牌")};
    Btn::Button reccard {btn_get+QPoint{0,btn_offset}, tr("收牌")};
    Btn::Button start {btn_get+QPoint{0,2*btn_offset}, tr("重来")};
    MainWindow_choose chs_wnd {this};
    int chs_res;
    bool choose_state;
    QString card_intro;
    int lose_time;
    Win win;
    Heap::Pair get_norpt(const Heap::Hand& o);      // 从手牌堆o中取出一张与栈中不重复的卡牌并返回，没有不重复的则随机取
    void update_snapshot();     // 绘制这一帧的图像
    void recover_proc();        // 处理收卡的过程
    void wait_choose();     // 等待选择完毕
    void effect();      // 当前卡片生效
};

#endif // MAINWINDOW_H
