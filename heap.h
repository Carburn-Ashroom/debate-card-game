#ifndef HEAP_H
#define HEAP_H

#include <QPixmap>
#include <QList>
#include <random>
#include <QMouseEvent>

#include "card.h"

namespace Heap {        // 该名字空间负责牌堆

    using std::default_random_engine;
    using std::uniform_int_distribution;
    
    extern default_random_engine egn;       // 随机数引擎
    
    struct Empty_error {};      // 操作空牌堆

    class Heap {        // Heap提供了一个牌堆
    public:
        virtual void draw(QPixmap* pix) const;      // 绘制
        virtual ~Heap() { }
        QList<Cards::Card>& get_cards() { return cards; }
        const QList<Cards::Card>& get_c_cards() const { return cards; }
        virtual void append(Cards::Card c) { cards.append(c); }
    private:
        QList<Cards::Card> cards;
    };

    class Slot : public Heap {      // Slot是一个卡槽
    public:
        void draw(QPixmap* pix) const override;     // 绘制
        void draw(QPixmap* pix, int lose_t) const;      // 绘制自相矛盾第lose_t帧的卡槽，若lose_t为负则没有自相矛盾
        void append(Cards::Card c) override;        //加入卡牌
        bool update_cards();        // 更新所有的卡
        bool is_repeat() const;     // 检测最后一张卡是否与前面的重复
        void del();     // 自相矛盾后删除卡
        Cards::Card take(int i =chs_null);      // 当利害重申时，取出第i张卡片以便将其放置在最后，若i为负数则随机取。i必须小于Slot中卡牌的数量-1，因为最后一张一定是利害重申所以不算
        static QRect get_card_rect(int i, QPoint pos);      // 获取第i张卡片的位置
    private:
        int del_start() const;      // 检测自相矛盾时删除的第一张卡
        QRect get_rect() const { return QRect{hp_stack_pos,QSize{msc_size.width()-2*hp_stack_pos.x(),hp_stack_sz.height()}}; }
    };

    struct Pair {       // 一个卡牌与序号的绑定
        int th;
        Cards::Card c;
    };

    class Hand : public Heap {      // Hand类是玩家和敌人手里的牌
    public:
        explicit Hand(bool e =false) : enemy{e} { }
        void draw(QPixmap* pix) const override;     // 绘制
        int score() const;      // 返回分数
        void append(QList<Cards::Card>& cs, bool get_only =false);      // 测试加入位置或加入牌堆，取决于get_only
        QList<Pair> max() const;        // 分数最大的卡
        QList<Pair> min() const;        // 分数最低的卡
        Cards::Card take(int i);        // 将第i张牌设置好移动状态并拿出。i必须小于牌数且非负
        Cards::Card take();     // 随机拿走一张牌，并设置这张牌为删除（道破）状态
        void sort();        // 重新排序卡牌位置，仅限用于敌人的手牌
    private:
        bool enemy;
        void append(Cards::Card& c, bool get_only);     // 测试加入位置或加入卡牌，取决于get_only
        void append(Cards::Card c) override;        // 加入卡牌
        QList<int> vtual_th;
        void clear_vtual();     // 清除虚卡
    };

    class Deal : public Heap {      // Deal类是发牌的牌堆
    public:
        explicit Deal() : loder{c_min_score,c_max_score}, ider{0,static_cast<int>(c_name.count()-1)}, alpha{13} { }
        Cards::Card take();     // 发一张牌
        void draw(QPixmap* pix) const override;     // 绘制
        void update();      // 更新透明度状态
        void set_cansee(bool cs =false) { can_see = cs; }
        Cards::Card see() const { return (get_c_cards().count()) ? get_c_cards().last() : throw Empty_error{}; }
        void summon(int n);     // 生成n张初始牌
    private:
        uniform_int_distribution<> loder;
        uniform_int_distribution<> ider;
        int alpha;
        bool a_bigger;
        bool can_see;
    };

}

#endif // HEAP_H
