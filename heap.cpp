/**
 * 实现牌堆
 * 创建者：Carburn Ashroom
 * 2023.10.16
 */

#include <ctime>

#include "heap.h"

namespace Heap {
    
    default_random_engine egn {static_cast<uint32_t>(time(nullptr))};

    void Heap::draw(QPixmap* pix) const
    {
        for (auto& c : cards)
            c.draw(pix);
    }

    void Slot::draw(QPixmap* pix) const
    {
        QPainter p {pix};
        p.setBrush(QBrush{hp_stack_c});
        p.drawRect(get_rect());
        p.end();
        Heap::draw(pix);
    }

    void Slot::draw(QPixmap* pix, int lose_t) const
    {
        Slot::draw(pix);
        if (lose_t >= 0) {
            QPainter pa {pix};
            int stt {del_start()-1};
            QColor cls {hp_stack_c};
            cls.setAlpha(255-255*lose_t/msc_lose);
            pa.setBrush(QBrush{cls});
            while (++stt != get_c_cards().count())
                pa.drawRect(get_card_rect(stt, hp_stack_pos));
            QRect text_r {get_rect()};
            text_r.setLeft(get_card_rect(del_start(), hp_stack_pos).left());
            pa.setFont(los_font);
            pa.setPen(QPen{los_c});
            pa.drawText(text_r, Qt::AlignVCenter, QString{"自相矛盾"});
        }
    }

    void Slot::append(Cards::Card c)
    {
        c.move(get_card_rect(get_cards().count(), hp_stack_pos));
        c.update();
        Heap::append(c);
    }

    bool Slot::update_cards()
    {
        int ard_count {};
        for (auto& c : get_cards())
            ard_count += c.update();
        return ard_count==get_cards().count();
    }

    bool Slot::is_repeat() const
    {
        Cards::Card temp_c {get_c_cards().last()};
        for (auto i=++get_c_cards().rbegin(); i!=get_c_cards().rend(); ++i)
            if ((*i).cmp(temp_c)!=Cards::Cmp::isnot)
                return true;
        return false;
    }

    void Slot::del()
    {
        auto count = get_cards().count()-del_start();
        get_cards().remove(del_start(), count);
    }

    Cards::Card Slot::take(int i)
    {
        if (get_cards().empty())
            throw Empty_error{};
        if (i < 0) {
            uniform_int_distribution<> dis {0,static_cast<int>(get_cards().count()-2)};
            i = dis(egn);
        }
        Cards::Card res {get_cards().takeAt(i)};
        for (int j{i}; j!=get_cards().count(); ++j)
            get_cards()[j].move(get_card_rect(j, hp_stack_pos), c_mov);
        res.move(get_card_rect(get_cards().count(), hp_stack_pos), c_mov);
        return res;
    }

    QRect Slot::get_card_rect(int i, QPoint pos)
    {
        int x {(hp_stack_sz.width()+hp_stack_gap)*i};
        return QRect{pos+QPoint{x,0},hp_stack_sz};
    }

    int Slot::del_start() const
    {
        for (auto i=get_c_cards().count()-1; i!=0; --i)
            if (get_c_cards()[i].get_name() == "自圆其说")
                return i;
        return 0;
    }

    void Hand::draw(QPixmap* pix) const
    {
        QList<Pair> d_cards {max()};
        if (enemy) {
            for (const auto& c : d_cards)
                if (!vtual_th.contains(c.th))
                    c.c.draw(pix);
        }
        else {
            if (d_cards.count() == 1) {
                QPainter p {pix};
                p.translate(hp_p_pos);
                d_cards[0].c.draw(p);
            }
            else {
                int gap {hp_rotate*2/static_cast<int>(c_name.count())};
                int offset {(hp_rotate*2-static_cast<int>(d_cards.count()-1)*gap)/2-hp_rotate};
                for (int i{}; i!=d_cards.count(); ++i) {
                    QPainter p {pix};
                    p.translate(hp_p_pos);
                    p.rotate(offset+i*gap);
                    d_cards[i].c.draw(p);
                }
            }
        }
    }

    int Hand::score() const
    {
        QList ds {max()};
        int s {};
        for (const auto& d : ds)
            s += d.c.get_score();
        return s;
    }

    void Hand::append(QList<Cards::Card>& cs, bool get_only)
    {
        if (!get_only)
            clear_vtual();
        for (auto& c : cs)
            append(c, get_only);
    }

    void Hand::append(Cards::Card& c, bool get_only)
    {
        Cards::Card card {c};
        if (enemy) {
            if (!get_only) {
                append(card);
                return;
            }
            QList<Pair> d_cards {max()};
            bool find {false};
            for (const auto& cd : d_cards)
                if (c.cmp(cd.c) != Cards::Cmp::isnot) {
                    c.move(cd.c, c_mov);
                    find = true;
                    break;
                }
            if (!find) {
                card.move(QRect{hp_e_pos+QPoint{static_cast<int>(d_cards.count()*hp_e_gap),0},hp_size});
                card.update();
                c.move(card, c_mov);
                vtual_th.append(get_cards().count());
                append(card);
            }
        }
        else if (get_only)
            c.move(QRect{hp_p_pos+hp_p_offset,hp_size}, c_mov);
        else {
            card.move(QRect{hp_p_offset,hp_size});
            card.update();
            append(card);
        }
    }

    void Hand::append(Cards::Card c)
    {
        Heap::append(c);
    }

    void Hand::clear_vtual()
    {
        for (auto i=vtual_th.rbegin(); i!=vtual_th.rend(); ++i)
            get_cards().remove(*i);
        vtual_th.clear();
    }
    
    QList<Pair> Hand::max() const
    {
        QList<Pair> res;
        for (int i{}; i!=get_c_cards().count(); ++i) {
            bool common {false};
            for (auto& r : res)
                if (get_c_cards()[i].cmp(r.c) == Cards::Cmp::more) {
                    r.th = i;
                    r.c = get_c_cards()[i];
                    common = true;
                    break;
                }
                else if (get_c_cards()[i].cmp(r.c) == Cards::Cmp::less) {
                    common = true;
                    break;
                }
            if (!common) {
                Pair r {i,get_c_cards()[i]};
                res.append(r);
            }
        }
        return res;
    }

    QList<Pair> Hand::min() const
    {
        QList<Pair> res;
        for (int i{}; i!=get_c_cards().count(); ++i) {
            bool common {false};
            for (auto& r : res)
                if (get_c_cards()[i].cmp(r.c) == Cards::Cmp::less) {
                    r.th = i;
                    r.c = get_c_cards()[i];
                    common = true;
                    break;
                }
                else if (get_c_cards()[i].cmp(r.c) == Cards::Cmp::more) {
                    common = true;
                    break;
                }
            if (!common) {
                Pair r {i,get_c_cards()[i]};
                res.append(r);
            }
        }
        return res;
    }

    Cards::Card Hand::take(int i)
    {
        Cards::Card c {get_cards().takeAt(i)};
        if (!enemy) {
            c.move(QRect{hp_p_pos+hp_p_offset,hp_size});
            c.update();
        }
        c.move(c_big, c_mov);
        return c;
    }

    Cards::Card Hand::take()
    {
        if (get_cards().count() == 0)
            throw Empty_error{};
        uniform_int_distribution<> dis {0,static_cast<int>(get_cards().count()-1)};
        Cards::Card c {get_cards().takeAt(dis(egn))};
        if (!enemy) {
            c.move(QRect{hp_p_pos+hp_p_offset,hp_size});
            c.update();
        }
        c.move(c_big, c_mov);
        c.del();
        return c;
    }

    void Hand::sort()
    {
        QList<Cards::Card> t_c;
        for (auto& c : get_cards()) {
            bool find {false};
            for (const auto& tc : t_c)
                if (c.cmp(tc) != Cards::Cmp::isnot) {
                    find = true;
                    c.move(tc);
                    c.update();
                }
            if (!find) {
                c.move(QRect{hp_e_pos+QPoint{static_cast<int>(t_c.count()*hp_e_gap),0},hp_size});
                c.update();
                t_c.append(c);
            }
        }
    }

    Cards::Card Deal::take()
    {
        if (get_cards().empty())
            throw Empty_error{};
        Cards::Card c {get_cards().takeLast()};
        c.move(c_big, c_mov);
        return c;
    }

    void Deal::draw(QPixmap* pix) const
    try
    {
        see().draw(pix);
        QPainter pa {pix};
        QColor a_c {hp_stack_c};
        a_c.setAlpha((can_see) ? alpha : 255);
        pa.setBrush(QBrush{a_c});
        pa.drawRect(QRect{hp_pos,hp_size});
    }
    catch (Empty_error) {
        QPainter pa {pix};
        pa.setBrush(QBrush{hp_stack_c});
        pa.drawRect(QRect{hp_pos,hp_size});
    }

    void Deal::update()
    {
        alpha += (a_bigger) ? hp_a_speed : -hp_a_speed;
        if (alpha < 0) {
            alpha = 0;
            a_bigger = true;
        }
        else if (alpha > 255) {
            alpha = 255;
            a_bigger = false;
        }
    }

    void Deal::summon(int n)
    {
        get_cards().clear();
        while (n--)
            append(Cards::Card{ider(egn),loder(egn)});
        can_see = false;
    }

}
