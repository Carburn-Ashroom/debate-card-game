#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(msc_size);
    connect(&timer, SIGNAL(timeout()), this, SLOT(frame_update()));
    timer.start(1000/msc_fps);
    start_game();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    event->accept();
    QPainter pa {this};
    pa.drawPixmap(0, 0, snapshot);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    getcard.strength(event);
    reccard.strength(event);
    start.strength(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    getcard.activate(event);
    reccard.activate(event);
    start.activate(event);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    event->accept();
    qApp->exit(0);
}

void MainWindow::frame_update()
{
    update_snapshot();
    if (start.clicked()) {
        if (win == Win::unknown) {
            if (QMessageBox::question(this, tr("你说真的？"), tr("真的要重开？不允许反悔")) == QMessageBox::Yes)
                start_game();
        }
        else
            start_game();
    }
    dealling.update();
    if (state==State::e_rec || state==State::p_rec) {
        recover_proc();
        if (state==State::enemy and player.score()>=msc_win_score) {
            QMessageBox::information(this, tr("恭喜"), tr("你赢了"));
            win = Win::win;
        }
        else if (state==State::player and enemy.score()>=msc_win_score) {
            QMessageBox::warning(this, tr("哦不"), tr("你输了（悲"));
            win = Win::lose;
        }
        else if (dealling.get_cards().empty() and (state==State::enemy or state==State::player)) {
            if (player.score() > enemy.score()) {
                QMessageBox::information(this, tr("恭喜"), tr("你赢了"));
                win = Win::win;
            }
            else {
                QMessageBox::warning(this, tr("哦不"), tr("你输了（悲"));
                win = Win::lose;
            }
        }
    }
    else if (choose_state)
        wait_choose();
    else if (lose_time >= 0) {
        if (--lose_time < 0) {
            slot.del();
            if (!slot.get_cards().empty())
                recover_card();
            else
                state = (state==State::enemy) ? State::player : State::enemy;
            return;
        }
    }
    else {
        switch (have_nowc) {
        case C_state::bigging: case C_state::smalling:
            now_card.update(&have_nowc);
            slot.update_cards();
            break;
        case C_state::effect:
            if (!slot.update_cards())
                break;
            slot.append(now_card);
            card_intro = now_card.get_intro();
            have_nowc = C_state::nonow;
            if (slot.is_repeat() and lose_time<0) {
                lose_time = msc_lose;
                return;
            }
            effect();
            break;
        case C_state::stay:
            if (++stay == c_stay) {
                have_nowc = C_state::smalling;
                now_card.move(QRect{(now_card.if_del()) ? msc_del_pos : Heap::Slot::get_card_rect(slot.get_cards().count(), hp_stack_pos).topLeft(),hp_stack_sz}, c_mov);
                stay = 0;
            }
            break;
        case C_state::nonow:
            if (win != Win::unknown)
                break;
            if (state == State::enemy) {
                uniform_int_distribution<> dis {msc_e_rec.real(),msc_e_rec.imag()};
                if (dis(Heap::egn) > msc_e_rec.real())
                    get_card();
                else
                    recover_card();
            }
            else if (state == State::player) {
                if (getcard.clicked())
                    get_card();
                else if (reccard.clicked())
                    recover_card();
            }
            break;
        }
        getcard.clicked();
        reccard.clicked();
    }
    update();
}

void MainWindow::get_card()
try
{
    dealling.set_cansee();
    now_card = dealling.take();
    have_nowc = C_state::bigging;
}
catch (Heap::Empty_error) {
    recover_card();
}

void MainWindow::start_game()
{
    enemy.get_cards().clear();
    player.get_cards().clear();
    slot.get_cards().clear();
    dealling.summon(hp_count);
    state = State::player;
    card_intro.clear();
    have_nowc = C_state::nonow;
    lose_time = -1;
    stay = 0;
    choose_state = false;
    win = Win::unknown;
}

void MainWindow::recover_card()
{
    if (slot.get_cards().empty())
        return;
    else if (state == State::enemy) {
        state = State::e_rec;
        enemy.append(slot.get_cards(), true);
    }
    else if (state == State::player) {
        state = State::p_rec;
        player.append(slot.get_cards(), true);
    }
}

Heap::Pair MainWindow::get_norpt(const Heap::Hand& o)
{
    if (o.get_c_cards().empty())
        throw Heap::Empty_error{};
    for (int i{}; i!=o.get_c_cards().count(); ++i) {
        bool repeat {false};
        for (auto c : slot.get_c_cards())
            if (c.cmp(o.get_c_cards()[i]) != Cards::Cmp::isnot) {
                repeat = true;
                break;
            }
        if (!repeat)
            return Heap::Pair{i,o.get_c_cards()[i]};
    }
    uniform_int_distribution<> rand {0,static_cast<int>(o.get_c_cards().count())};
    int th {rand(Heap::egn)};
    return Heap::Pair{th,o.get_c_cards()[th]};
}

void MainWindow::update_snapshot()
{
    QPixmap new_snap {msc_size};
    QPainter pa {&new_snap};
    pa.fillRect(QRect{QPoint{0,0},msc_size}, msc_bg_color);
    pa.fillRect(intr_main_r, intr_color);
    pa.fillRect(intr_rule_r, intr_color);
    pa.setPen(QPen{f_cls});
    pa.setFont(QFont{f_font,sc_sz});
    pa.drawText(sc_pos.real(), Qt::AlignCenter, QString{}.setNum(enemy.score()));
    pa.drawText(sc_pos.imag(), Qt::AlignCenter, QString{}.setNum(player.score()));
    pa.drawText(intr_main_r, Qt::AlignCenter, card_intro);
    pa.drawText(intr_rule_r, Qt::AlignCenter, intr_rule);
    pa.end();
    slot.draw(&new_snap, lose_time);
    player.draw(&new_snap);
    enemy.draw(&new_snap);
    dealling.draw(&new_snap);
    if (have_nowc != C_state::nonow)
        now_card.draw(&new_snap);
    reccard.draw(&new_snap);
    getcard.draw(&new_snap);
    start.draw(&new_snap);
    snapshot = std::move(new_snap);
}

void MainWindow::recover_proc()
{
    if (slot.update_cards()) {
        if (state == State::p_rec) {
            player.append(slot.get_cards());
            state = State::enemy;
        }
        else {
            enemy.append(slot.get_cards());
            state = State::player;
        }
        slot.get_cards().clear();
    }
    getcard.clicked();
    reccard.clicked();
}

void MainWindow::wait_choose()
{
    if ((chs_res=chs_wnd.choose()) != chs_null) {
        choose_state = false;
        if (now_card.get_name()=="曲义" or now_card.get_name()=="道破") {
            QString old_name {now_card.get_name()};
            now_card = enemy.take(chs_res);
            enemy.sort();
            have_nowc = C_state::bigging;
            if (old_name == "道破")
                now_card.del();
        }
        else if (now_card.get_name() == "利害重申") {
            now_card = slot.take(chs_res);
            have_nowc = C_state::smalling;
        }
        else if (now_card.get_name() == "诡辩") {
            now_card = player.get_c_cards()[chs_res];
            now_card.move(QRect{hp_p_pos+hp_p_offset,hp_size});
            now_card.update();
            now_card.move(c_big, c_mov);
            have_nowc = C_state::bigging;
        }
    }
}

void MainWindow::effect()
try
{
    if (now_card.get_name() == "铺垫") {
        if (state == State::player)
            dealling.set_cansee(true);
        else {
            bool rec {false};
            for (const auto& c : slot.get_cards())
                if (c.cmp(dealling.see()) != Cards::Cmp::isnot) {
                    rec = true;
                    recover_card();
                    break;
                }
            if (!rec)
                get_card();
        }
    }
    else if (now_card.get_name() == "箭在弦上")
        get_card();
    else if (now_card.get_name() == "见好就收")
        recover_card();
    else if (now_card.get_name() == "曲义") {
        if (state == State::enemy) {
            Heap::Pair p {get_norpt(player)};
            now_card = player.take(p.th);
            have_nowc = C_state::bigging;
        }
        else if (enemy.get_cards().count()) {
            choose_state = true;
            auto min = enemy.min();
            chs_wnd.show(min);
        }
    }
    else if (now_card.get_name() == "道破") {
        if (state == State::enemy) {
            now_card = player.take();
            have_nowc = C_state::bigging;
        }
        else if (enemy.get_cards().count()) {
            choose_state = true;
            auto max = enemy.max();
            chs_wnd.show(max);
        }
    }
    else if (now_card.get_name() == "利害重申") {
        if (state == State::enemy) {
            if (slot.get_cards().count() == 1)
                throw Heap::Empty_error{};
            now_card = slot.take();
            have_nowc = C_state::smalling;
        }
        else {
            if (slot.get_cards().count() == 1)
                throw Heap::Empty_error{};
            choose_state = true;
            QList<Heap::Pair> temp_p;
            for (int i{}; i!=slot.get_cards().count()-1; ++i)
                temp_p.append(Heap::Pair{i,slot.get_cards()[i]});
            chs_wnd.show(temp_p);
        }
    }
    else if (now_card.get_name() == "诡辩") {
        if (state == State::enemy) {
            now_card = get_norpt(enemy).c;
            now_card.move(c_big, c_mov);
            have_nowc = C_state::bigging;
        }
        else if (player.get_cards().count()) {
            choose_state = true;
            chs_wnd.show(player.min());
        }
    }
}
catch (Heap::Empty_error) {
    return;
}
