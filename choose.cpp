/**
 * 实现选择牌堆
 * 创建者：Carburn Ashroom
 * 2023.10.24
 */

#include "choose.h"

namespace Chs {

    void Choose::set_cards(const QList<Heap::Pair>& cs)
    {
        pairs = cs;
        for (int i{}; i!=pairs.count(); ++i) {
            pairs[i].c.move(Heap::Slot::get_card_rect(i, chs_hp_pos));
            pairs[i].c.update();
        }
        choose = chs_null;
    }

    void Choose::press(QMouseEvent* event)
    {
        QPoint p {event->position().toPoint()};
        for (int i{}; i!=pairs.count(); ++i)
            if (Heap::Slot::get_card_rect(i, chs_hp_pos).contains(p))
                choose = i;
    }

    void Choose::clear()
    {
        choose = chs_null;
        pairs.clear();
        intro.clear();
    }

    void Choose::draw(QPixmap* pix)
    {
        QPainter pa {pix};
        pa.setBrush(QBrush{hp_stack_c});
        pa.drawRect(QRect{chs_hp_pos,QSize{chs_sz.width()-2*chs_hp_pos.x(),hp_stack_sz.height()}});
        pa.fillRect(intr_chs_r, intr_color);
        pa.end();
        for (const auto& p : pairs)
            p.c.draw(pix);
        pa.begin(pix);
        if (choose != chs_null) {
            pa.setFont(QFont{f_font,sc_sz});
            pa.setPen(QPen{f_cls});
            pa.drawText(intr_chs_r, Qt::AlignCenter, pairs[choose].c.get_intro());
            QRect chs_r {QPoint{0,0},chs_strength_sz};
            chs_r.moveCenter(Heap::Slot::get_card_rect(choose, chs_hp_pos).center());
            pa.setPen(QPen{chs_color});
            pa.setBrush(QBrush{Qt::NoBrush});
            pa.drawRect(chs_r);
        }
        pa.end();
    }
}
