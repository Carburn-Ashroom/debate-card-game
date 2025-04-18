/**
 * 实现卡牌操作
 * 创建者：Carburn Ashroom
 * 2023.10.14
 */

#include "card.h"

namespace Cards {

    void Card::move(QRect r, int m_tm)
    {
        goal_pos = r.topLeft();
        goal_sz = r.size();
        mv_time = m_tm;
    }

    bool Card::update(C_state* s)
    {
        QSize gap_sz {goal_sz-sz};
        QPoint gap_p {goal_pos-pos};
        switch (mv_time) {
        case 1:
            pos = goal_pos;
            sz = goal_sz;
            mv_time = 0;
            break;
        case 0:
            break;
        default:
            sz += gap_sz/mv_time;
            pos += gap_p/mv_time--;
            return false;
        }
        if (s) {
            if (*s == C_state::bigging)
                *s = C_state::stay;
            else if (m_del)
                *s = C_state::nonow;
            else
                *s = C_state::effect;
        }
        return true;
    }

    void Card::draw(QPixmap* pix) const
    {
        QPainter pain {pix};
        draw(pain);
    }

    void Card::draw(QPainter& pain) const
    {
        pain.setBrush(QBrush{c_color[id]});
        pain.drawRect(QRect{pos,sz});
        QSize sc_p {sz*c_sc_pos};
        pain.setPen(QPen{f_cls});
        pain.setFont(QFont{f_font,static_cast<int>(sz.width()*c_ft_score)});
        pain.drawText(QPoint{pos.x()+sc_p.width(),pos.y()+sc_p.height()*2}, QString{}.setNum(score));
        pain.setFont(QFont{f_font,static_cast<int>(sz.width()*c_ft_name)});
        pain.drawText(QRect(pos,sz), Qt::AlignCenter, name_enter());
    }

    Cmp Card::cmp(const Card& c) const
    {
        if (id != c.id)
            return Cmp::isnot;
        return (score>c.score) ? Cmp::more : Cmp::less;
    }

    QString Card::get_intro() const
    {
        QFile intro {intr_loc};
        intro.open(QIODevice::ReadOnly);
        QList inf {intro.readAll().split('\n')};
        return inf[id];
    }

    QString Card::name_enter() const
    {
        QString res_n {c_name[id]};
        for (int i{1}; i!=res_n.length(); i+=2)
            res_n.insert(i, '\n');
        return res_n;
    }

}
