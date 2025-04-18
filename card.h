#ifndef CARD_H
#define CARD_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QPainter>
#include <QFile>
#include <QString>

#include "config.h"

namespace Cards {       // 该名字空间定义卡牌

    enum class Cmp { isnot, less, more };       // 比较状态

    class Card {        // Card是一张可以运动的卡牌
    public:
        Card(int i, int g, QPoint p =hp_pos, QSize s =hp_size) : id{i}, score{g}, pos{p}, sz{s}, goal_pos{p}, goal_sz{s}, mv_time{0}, m_del{false} { }
        void move(QRect r, int m_tm =1);        // 在m_tm内移动到指定位置
        void move(const Card& o, int m_tm =1) { move(QRect{o.pos,o.sz}, m_tm); }
        bool update(C_state* s =nullptr);       // 更新牌的状态，返回是否更新完毕
        void draw(QPixmap* pix) const;      // 绘制
        void draw(QPainter& pain) const;        // 绘制
        Cmp cmp(const Card& c) const;       // 与另一张卡比较
        int get_score() const { return score*((get_name()=="雄辩") ? 2 : 1); }
        QString get_name() const { return c_name[id]; }
        void del() { m_del = true; }
        bool if_del() const { return m_del; }
        QString get_intro() const;      // 获取卡牌说明
    private:
        int id;
        int score;
        QPoint pos;
        QSize sz;
        QPoint goal_pos;
        QSize goal_sz;
        int mv_time;
        QString name_enter() const;     // 给名字中的每个字加\n
        bool m_del;
    };

}

#endif // CARD_H
