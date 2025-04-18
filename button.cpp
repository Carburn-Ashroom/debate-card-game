/**
 * 实现按钮
 * 创建者：Carburn Ashroom
 * 2023.10.21
 */

#include "button.h"

namespace Btn {

    void Button::activate(QMouseEvent* event)
    {
        if (!strenthen)
            return;
        clcd = rect.contains(event->position().toPoint());
        strenthen = false;
    }

    void Button::draw(QPixmap* pix) const
    {
        QPainter p {pix};
        p.setBrush(QBrush{(strenthen) ? msc_bt_c.imag() : msc_bt_c.real()});
        p.drawRect(rect);
        p.setFont(QFont{f_font,btn_font_sz});
        p.setPen(QPen{f_cls});
        p.drawText(rect, Qt::AlignCenter, text);
    }

}
