#ifndef CHOOSE_H
#define CHOOSE_H

#include <QList>
#include <QMouseEvent>
#include <QString>

#include "heap.h"

namespace Chs {     // 该名字空间负责选择窗口

    class Choose {      // Choose类提供了选择卡牌的接口
    public:
        Choose() { clear(); }
        void set_cards(const QList<Heap::Pair>& cs);        // 设置卡牌
        void press(QMouseEvent* event);     // 鼠标按下
        void clear();       // 清空卡牌
        void draw(QPixmap* pix);        // 绘制
        int get_choose() const { return (choose==chs_null) ? chs_null : pairs[choose].th; }
    private:
        int choose;
        QList<Heap::Pair> pairs;
        QString intro;
    };

}

#endif // CHOOSE_H
