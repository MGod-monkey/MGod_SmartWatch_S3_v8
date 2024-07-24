#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lv_rollmsg.h"

// 以下是各种设置
/////////////////////////////////////////////
#define numberof_msg_container 20 // 最大消息数目
#define msg_panel_w 200           // 卡片宽度
#define msg_panel_h 80            // 卡片高度
#define msg_panel_distance 85     // 卡片间距
#define msg_panel_offset_x 20     // 卡片x坐标偏移
#define card_color 0x555555       // 消息卡片背景颜色
#define back_w 240                // 大背景板宽度
#define back_h 280                // 大背景板高度
#define msg_popup 1               // 使能消息弹窗
/////////////////////////////////////////////

LV_IMG_DECLARE(bmp_tianqi_40x40_0)

static double calculateYCoordinate(double centerX, double centerY, double radius, double x);

typedef struct
{
  lv_obj_t *msg_container;
  bool has_msg_container;
  lv_obj_t *msg_container_card;
  int y;
  int index;
} msg_container_typedef;

static lv_obj_t *screen1, *botton_exit, *window, *close_btn, *mini_btn, *bar_btn, *drag_btn2;
static int offsetx, offsety, tuoching, win_old_x, win_old_y, win_old_w, win_old_h, screen1_offsety;
static bool touched2, movex, test_movexy, drag_touched, tempcard_touched;
static msg_container_typedef msg_container[numberof_msg_container];
static lv_obj_t *window;
static void exit_game_cb(lv_event_t *e);

static void released_cb(lv_event_t *e);
static void slider_released_cb(lv_event_t *e);
static void slider_cb(lv_event_t *e);
static void card_back_cb(void *var, int32_t v);
static void drag_PRESS_cb(lv_event_t *e);
static void drag_RELEASED_cb(lv_event_t *e);
static void board_move_y(void *var, int32_t v);
static void tempcard_ready_cb(lv_anim_t *a);
static void tempcard_del_cb(lv_anim_t *a);
static void tempcard_press_cb(lv_event_t *e);
static void tempcard_released_cb(lv_event_t *e);
// static void btn1_cb(lv_event_t * e);
static void allcard_moveup_cb(void *var, int32_t v);
static void allcard_movedown_cb(void *var, int32_t v);

void msg_roll(lv_obj_t *parent)
{

  int i, j;

  lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE); // 清除主屏滚动

  drag_btn2 = lv_label_create(lv_scr_act()); // 主屏上面的面板下拉手柄
  lv_obj_set_size(drag_btn2, back_w, 10);
  lv_obj_align(drag_btn2, LV_ALIGN_TOP_MID, 0, 0);
  lv_label_set_text(drag_btn2, " ");
  lv_obj_add_flag(drag_btn2, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(drag_btn2, drag_PRESS_cb, LV_EVENT_PRESSING, 0);    // 按住回调
  lv_obj_add_event_cb(drag_btn2, drag_RELEASED_cb, LV_EVENT_RELEASED, 0); // 松开回调

  screen1 = lv_tileview_create(lv_scr_act()); // 消息的大背景
  lv_obj_set_size(screen1, back_w, back_h);
  lv_obj_align(screen1, LV_ALIGN_TOP_MID, 0, -back_h);
  lv_obj_set_style_bg_color(screen1, lv_color_hex(0x000000), LV_PART_MAIN);
  lv_obj_clear_flag(screen1, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_event_cb(screen1, slider_cb, LV_EVENT_PRESSING, 0);          // 按住回调
  lv_obj_add_event_cb(screen1, slider_released_cb, LV_EVENT_RELEASED, 0); // 松开回调

  lv_obj_t *drag_btn = lv_label_create(screen1); // 消息背景的上拉手柄
  lv_obj_set_size(drag_btn, back_w, 20);
  lv_obj_align(drag_btn, LV_ALIGN_BOTTOM_MID, 0, 10);
  lv_obj_set_ext_click_area(drag_btn, 20);
  lv_label_set_text(drag_btn, " ");
  lv_obj_add_flag(drag_btn, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_OVERFLOW_VISIBLE);
  lv_obj_add_event_cb(drag_btn, drag_PRESS_cb, LV_EVENT_PRESSING, 0); // 按住回调
  lv_obj_add_event_cb(drag_btn, drag_RELEASED_cb, LV_EVENT_RELEASED, 0);

  for (int i = 0; i < numberof_msg_container; i++) // 新建消息卡片的透明容器
  {
    msg_container[i].y = i * msg_panel_distance;                  // 容器的y坐标
    msg_container[i].msg_container = lv_tileview_create(screen1); // 新建容器
    msg_container[i].has_msg_container = false;                   // 是否拥有消息内容
    msg_container[i].index = i;                                   // 容器的索引号
    lv_obj_set_size(msg_container[i].msg_container, msg_panel_w, msg_panel_h + msg_panel_distance);
    lv_obj_set_style_bg_opa(msg_container[i].msg_container, 0, LV_PART_MAIN);
    lv_obj_add_event_cb(msg_container[i].msg_container, slider_cb, LV_EVENT_PRESSING, 0); // 容器按住回调
    lv_obj_add_event_cb(msg_container[i].msg_container, slider_released_cb, LV_EVENT_RELEASED, 0);
    ; // 容器松开回调
    lv_obj_clear_flag(msg_container[i].msg_container, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    lv_obj_set_pos(msg_container[i].msg_container, msg_panel_offset_x, msg_container[i].y);
  }
}

static void drag_PRESS_cb(lv_event_t *e) // 手柄按住回调
{

  static lv_point_t click_point1;
  static lv_point_t click_point2;

  if (drag_touched == false) // 第一次按下记录坐标
  {

    lv_indev_get_point(lv_indev_get_act(), &click_point1);
    lv_obj_move_foreground(screen1);
    drag_touched = true;
    return;
  }
  else
  {
    lv_indev_get_point(lv_indev_get_act(), &click_point2); // 第2次按下记录坐标
  }

  screen1_offsety = click_point2.y - click_point1.y; // 计算2次坐标的差值
  click_point1.y = click_point2.y;                   // 把后一次的y保存到前一次

  lv_obj_set_y(screen1, lv_obj_get_y(screen1) + screen1_offsety);     // 根据差值调整背景板y坐标
  lv_obj_set_y(drag_btn2, lv_obj_get_y(drag_btn2) + screen1_offsety); // 根据差值调整手柄y坐标
}

static void drag_RELEASED_cb(lv_event_t *e) // 手柄松开回调
{

  drag_touched = false;

  if (screen1_offsety > 5) // 如果y差值大于5，背景往下拉到底
  {

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, screen1);
    lv_anim_set_exec_cb(&a, board_move_y);
    lv_anim_set_time(&a, -lv_obj_get_y(screen1));
    lv_anim_set_values(&a, lv_obj_get_y(screen1), 0);
    lv_anim_start(&a);
    lv_obj_set_y(drag_btn2, 0);
    return;
  }

  if (screen1_offsety < -5) // 如果y差值小于-5，背景往上拉到头
  {

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, screen1);
    lv_anim_set_exec_cb(&a, board_move_y);
    lv_anim_set_time(&a, lv_obj_get_height(screen1) + lv_obj_get_y(screen1));
    lv_anim_set_values(&a, lv_obj_get_y(screen1), -lv_obj_get_height(screen1));
    lv_anim_start(&a);
    lv_obj_set_y(drag_btn2, 0);
    return;
  }

  if (lv_obj_get_y(screen1) >= -lv_obj_get_height(screen1) / 2) // 超过一半往下拉
  {

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, screen1);
    lv_anim_set_exec_cb(&a, board_move_y);
    lv_anim_set_time(&a, -lv_obj_get_y(screen1));
    lv_anim_set_values(&a, lv_obj_get_y(screen1), 0);
    lv_anim_start(&a);
    lv_obj_set_y(drag_btn2, 0);
  }
  else // 小于一半往上拉
  {

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, screen1);
    lv_anim_set_exec_cb(&a, board_move_y);
    lv_anim_set_time(&a, lv_obj_get_height(screen1) + lv_obj_get_y(screen1));
    lv_anim_set_values(&a, lv_obj_get_y(screen1), -lv_obj_get_height(screen1));
    lv_anim_start(&a);
    lv_obj_set_y(drag_btn2, 0);
  }
}

bool lv_add_newmsg(const lv_img_dsc_t *icon, const char *title, const char *txt, lv_event_cb_t event_cb) // 添加新的消息卡片
{

#if msg_popup // 使能弹窗，就在主屏创建一个消息卡片副本
  if (lv_obj_get_y(screen1) == -back_h)
  {
    lv_obj_t *temp_card = lv_tileview_create(lv_scr_act());
    lv_obj_set_size(temp_card, msg_panel_w, msg_panel_h);
    lv_obj_set_style_bg_color(temp_card, lv_color_hex(card_color), LV_PART_MAIN);
    lv_obj_set_style_radius(temp_card, 15, LV_PART_MAIN);
    lv_obj_clear_flag(temp_card, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(temp_card, LV_ALIGN_TOP_MID, 0, -msg_panel_h);

    lv_obj_add_event_cb(temp_card, tempcard_press_cb, LV_EVENT_PRESSING, 0);
    lv_obj_add_event_cb(temp_card, tempcard_released_cb, LV_EVENT_RELEASED, 0);
    lv_obj_add_event_cb(temp_card, event_cb, LV_EVENT_SHORT_CLICKED, 0);

    lv_obj_t *msg_icon = lv_img_create(temp_card);
    lv_img_set_src(msg_icon, icon);
    lv_obj_set_pos(msg_icon, 10, 10);

    lv_obj_t *title_lable = lv_label_create(temp_card);
    lv_label_set_text(title_lable, title);
    lv_obj_set_pos(title_lable, 160, 10);
    lv_obj_set_style_text_color(title_lable, lv_color_hex(0xffffff), LV_PART_MAIN);

    lv_obj_t *txt_lable = lv_label_create(temp_card);
    lv_label_set_text(txt_lable, txt);
    lv_obj_set_pos(txt_lable, 10, 50);
    lv_obj_set_style_text_color(txt_lable, lv_color_hex(0xffffff), LV_PART_MAIN);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, temp_card);
    lv_anim_set_exec_cb(&a, board_move_y);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_set_ready_cb(&a, tempcard_ready_cb);
    lv_anim_set_time(&a, 300);
    lv_anim_set_values(&a, -msg_panel_h, 5);
    lv_anim_start(&a);
  }

#endif

  if (msg_container[numberof_msg_container - 1].has_msg_container != false) // 如果最后一个容器有内容，就删掉
  {

    lv_obj_del(msg_container[numberof_msg_container - 1].msg_container_card);
    msg_container[numberof_msg_container - 1].has_msg_container = false;
  }

  for (int i = numberof_msg_container - 2; i >= 0; i--) // 所有的消息卡片都向下移动一个容器
  {

    if (msg_container[i].has_msg_container != false)
    {
      msg_container[i + 1].msg_container_card = msg_container[i].msg_container_card;
      lv_obj_set_parent(msg_container[i + 1].msg_container_card, msg_container[i + 1].msg_container);
      msg_container[i + 1].msg_container_card->user_data = &msg_container[i + 1];
      msg_container[i + 1].has_msg_container = true;
    }
  }

  msg_container[0].msg_container_card = lv_tileview_create(msg_container[0].msg_container); // 在0号容器创建消息卡片
  lv_obj_set_size(msg_container[0].msg_container_card, msg_panel_w, msg_panel_h);
  lv_obj_set_style_bg_color(msg_container[0].msg_container_card, lv_color_hex(card_color), LV_PART_MAIN);
  lv_obj_set_style_radius(msg_container[0].msg_container_card, 15, LV_PART_MAIN);
  lv_obj_clear_flag(msg_container[0].msg_container_card, LV_OBJ_FLAG_SCROLLABLE);

  msg_container[0].msg_container_card->user_data = &msg_container[0];

  lv_obj_add_event_cb(msg_container[0].msg_container_card, slider_cb, LV_EVENT_PRESSING, 0);
  lv_obj_add_event_cb(msg_container[0].msg_container_card, slider_released_cb, LV_EVENT_RELEASED, 0);
  lv_obj_add_event_cb(msg_container[0].msg_container_card, event_cb, LV_EVENT_SHORT_CLICKED, 0);

  lv_obj_t *msg_icon = lv_img_create(msg_container[0].msg_container_card);
  lv_img_set_src(msg_icon, icon);
  lv_obj_set_pos(msg_icon, 10, 10);

  lv_obj_t *title_lable = lv_label_create(msg_container[0].msg_container_card);
  // lv_obj_set_align(title_lable,LV_ALIGN_TOP_RIGHT);
  lv_label_set_text(title_lable, title);
  lv_obj_set_pos(title_lable, 160, 10);
  lv_obj_set_style_text_color(title_lable, lv_color_hex(0xffffff), LV_PART_MAIN);

  lv_obj_t *txt_lable = lv_label_create(msg_container[0].msg_container_card);
  // lv_obj_set_align(txt_lable,LV_ALIGN_LEFT_MID);
  lv_label_set_text(txt_lable, txt);
  lv_obj_set_pos(txt_lable, 10, 50);
  lv_obj_set_style_text_color(txt_lable, lv_color_hex(0xffffff), LV_PART_MAIN);

  msg_container[0].has_msg_container = true;

  return true;
}

static void tempcard_ready_cb(lv_anim_t *a) // 消息卡片副本动画
{

  lv_obj_t *temp_card = (lv_obj_t *)a->var;

  lv_anim_t a1;
  lv_anim_init(&a1);
  lv_anim_set_var(&a1, temp_card);
  lv_anim_set_exec_cb(&a1, board_move_y);
  lv_anim_set_path_cb(&a1, lv_anim_path_ease_in);
  lv_anim_set_ready_cb(&a1, tempcard_del_cb);
  lv_anim_set_delay(&a1, 3000);
  lv_anim_set_time(&a1, 300);
  lv_anim_set_values(&a1, 5, -msg_panel_h);
  lv_anim_start(&a1);
}

static void msg_del_cb(lv_anim_t *a) // 消息卡片副本删除
{

  lv_obj_t *temp_card = (lv_obj_t *)a->var;

  lv_obj_del(temp_card);
}

static void slider_cb(lv_event_t *e) // 滑动回调
{
  int i, j;
  lv_obj_t *obj = (lv_obj_t *)e->target;

  static lv_point_t click_point1;
  static lv_point_t click_point2;

  if (touched2 == false)
  {
    lv_indev_get_point(lv_indev_get_act(), &click_point1);
    touched2 = true;
    return;
  }
  else
  {
    lv_indev_get_point(lv_indev_get_act(), &click_point2);
  }

  i = click_point2.x - click_point1.x; // 记录x滑动了多少
  j = click_point2.y - click_point1.y; // 记录y滑动了多少

  click_point1.x = click_point2.x; // 保存，下次进来可以继续比较
  click_point1.y = click_point2.y;

  if (movex == true) // 如果是向右滑动，就移动卡片x坐标
  {
    if (i > 0)
      lv_obj_set_x(obj, lv_obj_get_x(obj) + i);
    return;
  }

  if (test_movexy == false && obj != screen1 && i > 0 && i > (j > 0 ? j * 2 : -j * 2)) // 判断滑动方向更倾向于x还是y
  {
    movex = true;
    test_movexy = true;
    return;
  }

  int count_of_validcard;

  for (count_of_validcard = 0; count_of_validcard < numberof_msg_container; count_of_validcard++) // 检查有多少有效的消息卡片
  {
    if (msg_container[count_of_validcard].has_msg_container == false)
      break;
  }

  if (count_of_validcard == 0)
  {
    return;
  } // 消息为零就返回，不再处理滑动

  for (int i = 0; i < numberof_msg_container; i++) // 将所有的容器上下滑
  {
    msg_container[i].y += j;
    lv_obj_set_y(msg_container[i].msg_container, msg_container[i].y);
  }
}

static void slider_released_cb(lv_event_t *e) // 手指松开
{

  lv_obj_t *obj = (lv_obj_t *)e->target;

  touched2 = false;
  movex = false;
  test_movexy = false;

  if (obj != screen1)
  {
    if (lv_obj_get_x(obj) > lv_obj_get_width(obj) * 0.8)
    {
      msg_container_typedef *yyy = (msg_container_typedef *)(obj->user_data);
      int i = yyy->index;

      lv_obj_del(obj);
      msg_container[i].has_msg_container = false;

      for (; i < numberof_msg_container - 1; i++)
      {

        if (msg_container[i + 1].has_msg_container != false)
        {

          msg_container[i].msg_container_card = msg_container[i + 1].msg_container_card;
          lv_obj_set_parent(msg_container[i].msg_container_card, msg_container[i].msg_container);
          msg_container[i].msg_container_card->user_data = (void *)&msg_container[i];
          msg_container[i + 1].has_msg_container = false;
          msg_container[i].has_msg_container = true;

          lv_anim_t a;
          lv_anim_init(&a);
          lv_anim_set_var(&a, msg_container[i].msg_container_card);
          lv_anim_set_exec_cb(&a, board_move_y);
          lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
          lv_anim_set_time(&a, 200);
          lv_anim_set_values(&a, msg_panel_distance, 0);
          lv_anim_start(&a);
        }
      }
    }
    else
    {

      lv_anim_t a;

      lv_anim_init(&a);
      lv_anim_set_var(&a, obj);
      lv_anim_set_exec_cb(&a, card_back_cb);
      lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
      lv_anim_set_time(&a, 300);
      lv_anim_set_values(&a, lv_obj_get_x(obj), 0);
      lv_anim_start(&a);
    }
  }

  if (msg_container[0].y > (back_h - msg_panel_h) / 2)
  {
    lv_anim_t a1;

    lv_anim_init(&a1);
    lv_anim_set_var(&a1, 0);
    lv_anim_set_exec_cb(&a1, allcard_moveup_cb);
    lv_anim_set_path_cb(&a1, lv_anim_path_ease_out);
    lv_anim_set_time(&a1, 300);
    lv_anim_set_values(&a1, msg_container[0].y, (back_h - msg_panel_h) / 2);
    lv_anim_start(&a1);
    return;
  }

  static int count_of_validcard;

  for (count_of_validcard = 0; count_of_validcard < numberof_msg_container; count_of_validcard++)
  {
    if (msg_container[count_of_validcard].has_msg_container == false)
      break;
  }

  if (count_of_validcard > 0)
  {
    if (msg_container[count_of_validcard - 1].y < (back_h - msg_panel_h) / 2)
    {
      lv_anim_t a2;
      lv_anim_init(&a2);
      lv_anim_set_var(&a2, &count_of_validcard);
      lv_anim_set_exec_cb(&a2, allcard_movedown_cb);
      lv_anim_set_path_cb(&a2, lv_anim_path_ease_out);
      lv_anim_set_time(&a2, 300);
      lv_anim_set_values(&a2, msg_container[count_of_validcard - 1].y, (back_h - msg_panel_h) / 2);
      lv_anim_start(&a2);
      return;
    }
  }
}

static void allcard_moveup_cb(void *var, int32_t v)
{

  for (int i = 0; i < numberof_msg_container; i++)
  {
    msg_container[i].y = i * msg_panel_distance + v;
    lv_obj_set_y(msg_container[i].msg_container, msg_container[i].y);
  }
}

static void allcard_movedown_cb(void *var, int32_t v)
{
  int count_of_validcard = *(int *)var;

  for (int i = 0; i < numberof_msg_container; i++)
  {
    msg_container[i].y = (i - count_of_validcard + 1) * msg_panel_distance + v;
    lv_obj_set_y(msg_container[i].msg_container, msg_container[i].y);
  }
}

static void tempcard_press_cb(lv_event_t *e)
{
  lv_obj_t *tempcard = (lv_obj_t *)e->target;

  lv_anim_del(tempcard, board_move_y);

  static lv_point_t click_point1;
  static lv_point_t click_point2;

  if (tempcard_touched == false)
  {
    lv_indev_get_point(lv_indev_get_act(), &click_point1);
    tempcard_touched = true;
    return;
  }
  else
  {
    lv_indev_get_point(lv_indev_get_act(), &click_point2);
  }

  int j = click_point2.y - click_point1.y;
  click_point1.y = click_point2.y;

  if (j < 0)
  {
    lv_obj_set_y(tempcard, lv_obj_get_y(tempcard) + j);
  }
}

static void tempcard_released_cb(lv_event_t *e)
{
  lv_obj_t *tempcard = (lv_obj_t *)e->target;

  tempcard_touched = false;

  if (lv_obj_get_y(tempcard) < -msg_panel_h / 2)
  {
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, tempcard);
    lv_anim_set_exec_cb(&a, board_move_y);
    lv_anim_set_ready_cb(&a, tempcard_del_cb);
    lv_anim_set_time(&a, 200);
    lv_anim_set_values(&a, lv_obj_get_y(tempcard), -msg_panel_h);
    lv_anim_start(&a);
  }
  else
  {
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, tempcard);
    lv_anim_set_exec_cb(&a, board_move_y);
    lv_anim_set_ready_cb(&a, tempcard_ready_cb);
    lv_anim_set_time(&a, 200);
    lv_anim_set_values(&a, lv_obj_get_y(tempcard), 5);
    lv_anim_start(&a);
  }
}

static void tempcard_del_cb(lv_anim_t *a)
{

  lv_obj_t *tempcard = (lv_obj_t *)a->var;

  lv_obj_del(tempcard);
}

static void board_move_y(void *var, int32_t v)
{

  lv_obj_set_y((lv_obj_t *)var, v);
}

static void card_back_cb(void *var, int32_t v)
{

  lv_obj_set_x((lv_obj_t *)var, v);
}

void test()
{
  msg_roll(0); // 一定要先运行这个，再添加新消息
  lv_add_newmsg(&bmp_tianqi_40x40_0, "txt1", "efefwqfewqdwqdwfwq", 0);
}