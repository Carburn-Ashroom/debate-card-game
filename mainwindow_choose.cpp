#include "mainwindow_choose.h"
#include "ui_mainwindow_choose.h"

MainWindow_choose::MainWindow_choose(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_choose)
{
    ui->setupUi(this);

    setFixedSize(chs_sz);
    connect(&timer, SIGNAL(timeout()), this, SLOT(frame_update()));
}

MainWindow_choose::~MainWindow_choose()
{
    delete ui;
}

void MainWindow_choose::show(const QList<Heap::Pair>& p)
{
    chs_cards.set_cards(p);
    if_chs = false;
    timer.start(1000/msc_fps);
    QMainWindow::show();
}

int MainWindow_choose::choose()
{
    if (if_chs) {
        int res {chs_cards.get_choose()};
        chs_cards.clear();
        return res;
    }
    else
        return chs_null;
}

void MainWindow_choose::paintEvent(QPaintEvent* event)
{
    event->accept();
    QPainter pa {this};
    pa.drawPixmap(0, 0, snapshot);
}

void MainWindow_choose::mousePressEvent(QMouseEvent* event)
{
    ok.strength(event);
    chs_cards.press(event);
}

void MainWindow_choose::mouseReleaseEvent(QMouseEvent* event)
{
    ok.activate(event);
}

void MainWindow_choose::closeEvent(QCloseEvent* event)
{
    if (!if_chs) {
        QMessageBox::warning(this, tr("你真是个小天才"), tr("这玩意不能关，真的"));
        event->ignore();
    }
    else
        event->accept();
}

void MainWindow_choose::frame_update()
{
    QPixmap pix {chs_sz};
    QPainter pa {&pix};
    pa.fillRect(QRect{QPoint{0,0},chs_sz}, QBrush{msc_bg_color});
    pa.end();
    chs_cards.draw(&pix);
    ok.draw(&pix);
    snapshot = pix;
    if (ok.clicked() and chs_cards.get_choose()!=chs_null) {
        if_chs = true;
        timer.stop();
        close();
    }
    update();
}
