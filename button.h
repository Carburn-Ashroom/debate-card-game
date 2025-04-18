#ifndef BUTTON_H
#define BUTTON_H

#include <QPoint>
#include <QSize>
#include <QRect>
#include <QMouseEvent>
#include <QPixmap>
#include <QPainter>
#include <QString>

#include "config.h"

namespace Btn {     // 该名字空间负责按钮

    class Button {      // Button类是一个按钮
    public:
        Button(QPoint p, QString title, QSize sz =btn_sz) : clcd{false}, rect{p,sz}, strenthen{false}, text{title} { }
        bool clicked() { return (clcd) ? !(clcd=false) : false; }
        void strength(QMouseEvent* event) {strenthen = rect.contains(event->position().toPoint()); }
        void activate(QMouseEvent* event);      // 根据位置触发按钮
        void draw(QPixmap* pix) const;      // 绘制
    private:
        bool clcd;
        QRect rect;
        bool strenthen;
        QString text;
    };

}

#endif // BUTTON_H
