#ifndef CONFIG_H
#define CONFIG_H

#include <QPoint>
#include <QSize>
#include <QList>
#include <QPointF>
#include <QColor>
#include <QRect>
#include <complex>
#include <QFont>

using std::complex;

enum class C_state { bigging, effect, stay, smalling, nonow };      // 卡牌状态
enum class State { player, enemy, p_rec, e_rec };       // 全局游戏状态
enum class Win { win, lose, unknown };      // 输赢状态

// 杂项
constexpr int msc_fps {100};        // 帧率
constexpr QSize msc_size {900,670};     // 屏幕大小
const QColor msc_bg_color {"#4C3232"};      // 背景颜色
const complex<QColor> msc_bt_c {"#CCCC66","#FFFFAA"};       // 按钮激活前后颜色
constexpr QPoint msc_del_pos {1300,300};        // 卡牌丢弃位置
constexpr complex<int> msc_e_rec {1,4};     // 敌人操作收牌的概率
constexpr int msc_lose {50};        // 自相矛盾卡牌消失时间
constexpr int msc_win_score {50};       // 获胜分数

// 分数配置
constexpr complex<QRect> sc_pos {{10,20,40,90},{515,580,200,40}};       // 两个分数的位置
constexpr int sc_sz {13};       // 分数文字大小

// 牌堆配置
constexpr QPoint hp_pos {40,150};        // 发牌牌堆位置
constexpr QSize hp_size {60,90};        // 发牌牌堆大小
constexpr int hp_count {60};        // 卡牌数量
constexpr QSize hp_stack_sz {80,120};       // 当前牌堆大小
constexpr QPoint hp_stack_pos {10,300};     // 当前牌堆位置
constexpr int hp_stack_gap {2};     // 当前牌间隔
const QColor hp_stack_c {"#281C1C"};        // 当前牌堆颜色
constexpr QPoint hp_p_pos {615,600};        // 玩家手牌位置
constexpr QPoint hp_e_pos {60,20};      // 敌人手牌位置
constexpr int hp_e_gap {22};        // 敌人手牌露出
constexpr int hp_rotate {50};       // 最大角度
constexpr QPoint hp_p_offset {-30,-130};        // 玩家卡牌与手偏移位置
constexpr int hp_a_speed {5};       // 透明度变化速度

// 卡牌配置
const QList<QString> c_name { "故弄玄虚", "铺垫", "道破", "曲义", "雄辩", "诡辩", "箭在弦上", "自圆其说", "见好就收", "利害重申" };       // 卡牌名字
const QList<QColor> c_color { "#44AA44", "#00FF00", "#FF2222", "#00FFFF", "#2222FF", "#66CCFF", "#FFFF00", "#FF9900", "#FF5599", "#9955FF" };       // 卡牌颜色
constexpr double c_ft_score {.2};       // 数字大小比率
constexpr double c_ft_name {.2};        // 名字大小比率
constexpr double c_sc_pos {.1};     // 数字位置比率
constexpr int c_max_score {7};      // 最大分数
constexpr int c_min_score {1};      // 最小分数
constexpr QRect c_big {300,100,300,450};        // 卡牌高亮大小
constexpr int c_mov {26};       // 移动帧数
constexpr int c_stay {26};      // 停留帧数

// 全局文字配置
const QColor f_cls {"#000000"};     // 全局文字颜色
const QString f_font {"黑体"};        // 所有文字字体

// 按钮配置
constexpr QSize btn_sz {130,50};        // 按钮大小
constexpr QPoint btn_get {760,440};     // 抽卡按钮位置
constexpr int btn_offset {60};      // 按钮间隔
constexpr int btn_font_sz {13};     // 按钮文字大小
constexpr int btn_ok_ypos {360};        // 选择窗口的确定按钮y坐标位置

// 选择窗口配置
constexpr QSize chs_sz {900,500};       // 选择窗口大小
constexpr QPoint chs_hp_pos {10,200};       // 选择堆位置
constexpr QSize chs_strength_sz {88,132};        // 选中框大小
constexpr int chs_null {-1};        // 没有选中任何卡牌
const QColor chs_color {"#FF0000"};     // 选中框颜色

// 说明配置
const QString intr_loc {"Intro/Introduce.txt"};     // 卡牌说明文件路径
const QColor intr_color {c_color[0]};       // 说明背景颜色
constexpr QRect intr_main_r {130,150,727,90};       // 主窗口卡牌说明位置
constexpr QRect intr_chs_r {20,100,860,90};     // 选择窗口卡牌说明位置
constexpr QRect intr_rule_r {10,440,450,170};       // 游戏说明矩形
const QString intr_rule {QString{"分数由每种卡牌分数最大的分数相加得出\n抽到重复的卡牌将因为“自相矛盾”而丢失当前卡牌\n先集齐%1分者胜\n卡牌共%2张，抽完时分数高者胜"}.arg(msc_win_score).arg(hp_count)};       // 游戏规则说明

// 自相矛盾配置
const QFont los_font {QString{"黑体"},36};        // 自相矛盾字体
const QColor los_c {"#FF0000"};     // 自相矛盾文字颜色

#endif // CONFIG_H
