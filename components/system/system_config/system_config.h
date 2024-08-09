#ifndef __system_config_H__
#define __system_config_H__


#include "main.h"

#define WIFI_SCAN_LIST_SIZE 40
// #include "lvgl_project.h"

typedef struct _system_wifi_data1
{

    char name[50];
    char password[20];
    char auto_connect[10];
    struct _system_wifi_data1 *next;
}_system_wifi_data;


typedef struct
{

    // __clock_data clock_data;

    int sd_sta;
    int sht20_sta;
    int ft5206_sta;
    int spiffs_sta;
    int nvs_sta;
    int wm8978_sta;
    int mpu6050_sta;

	int Language;


    char IP_ADDR[30];
    char City[30];

    uint8_t MAC[6];
    char ip[30];
    char ziwangyanma[30];
    char wangguan[30];
    char wifi_name[32];
    char wifi_password[64];
    int Temp;
    int Humi;
    int HuoQu_TianQi_Flag;
    int HuoQu_ShiJian_Flag;
    int HuoQu_LaoHuangLi_Flag;
    int HuoQu_bilibili_Flag;
    int WIFI_Sta;//WIFI连接状态
    bool set_connect_wifi_flag;

    int web_huoqu_tianqi_flag;
    int wifi_scan_flag;

    int wifi_scan_list_cnt;
    wifi_ap_record_t wifi_ap_info[WIFI_SCAN_LIST_SIZE];	// AP信息结构体大小

    int wifi_switch;
    int backlight;

    lv_indev_data_t mouse;
    lv_indev_data_t huadong_mouse_0;
    lv_indev_data_t huadong_mouse_1;


    int uart_rx_home;
    int uart_rx_flag;

    //wifi链接成功事件
    EventGroupHandle_t wifi_event_group;
    EventGroupHandle_t Wav_event_group;
    EventGroupHandle_t TianQi_event_group;

    xSemaphoreHandle  https_request_Semaphore;

    // _Weather_Data   Weather_Data[3];
    // _kongqi_Data    kongqi_Data;
    // _RiLuo_Data     RiLuo_Data[3];
    // _RiQi_Data      RiQi_Data;
    // _laohuangli_data laohuangli_data;

    // _bilibili_Data bilibili_Data;

}_system_data;

extern _system_wifi_data *system_wifi_data;

void system_init(void);
void system_set_Language(int dat);
int system_get_Language(void);
int system_set_wifi_switch(int dat);
int system_get_wifi_switch(void);
void system_power_off(void);
int system_set_backlight(int dat);
int system_get_backlight(void);
void system_get_wifi_data(void);
_system_wifi_data* wifi_list_create(const char *name, const char *password, const char *auto_connect);
void wifi_list_add_node_back(_system_wifi_data **pphead, const char *name, const char *password, const char *auto_connect);
void wifi_list_add_node_front(_system_wifi_data **pphead, const char *name, const char *password, const char *auto_connect);
void wifi_list_delete_node_back(_system_wifi_data **pphead);
void wifi_list_delete_node_front(_system_wifi_data **pphead);
void wifi_list_delete_node_name(_system_wifi_data **pphead, const char *name);
_system_wifi_data *wifi_list_find_name(_system_wifi_data *phead, const char *name);
void system_read_config(void);
int system_save_config(void);
// uint32_t system_get_file_size(const char * path);


extern _system_data system_data;

// typedef struct
// {

//     int type;
//     int style;

// }__clock_data;


// #define SYSTEM_LANGUAGE_NUM 4

// enum Language_Num {
// 	JianTiZhongWen = 0,
// 	FanTiZhongWen,
// 	YingWen,
// 	QiTa,
// };


//extern wavctrl WaveCtrlData;
//extern FILE* Wave_File;

#endif
