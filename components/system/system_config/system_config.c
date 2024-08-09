#include "system_config.h"
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_spiffs.h"
#include "cJSON.h"

// 全局变量定义
_system_wifi_data *system_wifi_data = NULL;
_system_data system_data;
static const char *SYSTEMCONFIG_TAG = "system_config";

// 创建WiFi列表节点函数
_system_wifi_data* wifi_list_create(const char *name, const char *password, const char *auto_connect) {
    _system_wifi_data* NewNode = (_system_wifi_data*)malloc(sizeof(_system_wifi_data));
    if (!NewNode) {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Failed to allocate memory for WiFi node using malloc");
        return NULL;
    }

    memset(NewNode, 0x00, sizeof(_system_wifi_data));
    strncpy(NewNode->name, name, sizeof(NewNode->name) - 1);
    strncpy(NewNode->password, password, sizeof(NewNode->password) - 1);
    strncpy(NewNode->auto_connect, auto_connect, sizeof(NewNode->auto_connect) - 1);
    NewNode->next = NULL;

    return NewNode;
}

// 链表尾部插入一个节点
void wifi_list_add_node_back(_system_wifi_data **pphead, const char *name, const char *password, const char *auto_connect) {
    log_printf(SYSTEMCONFIG_TAG, LOG_DEBUG, "Adding a node to the back of the list");
    _system_wifi_data *NewNode = wifi_list_create(name, password, auto_connect);
    if (!NewNode) {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Failed to create new node");
        return;
    }

    if (!*pphead) {
        *pphead = NewNode;
        log_printf(SYSTEMCONFIG_TAG, LOG_DEBUG, "Initialized the list");
    } else {
        _system_wifi_data *tail = *pphead;
        while (tail->next) {
            tail = tail->next;
        }
        tail->next = NewNode;
        log_printf(SYSTEMCONFIG_TAG, LOG_DEBUG, "Inserting new node at the end");
    }
}

// 链表头部插入一个节点
void wifi_list_add_node_front(_system_wifi_data **pphead, const char *name, const char *password, const char *auto_connect) {
    _system_wifi_data *NewNode = wifi_list_create(name, password, auto_connect);
    if (!NewNode) {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Failed to create new node at the front");
        return;
    }
    NewNode->next = *pphead;
    *pphead = NewNode;
}

// 删除尾节点
void wifi_list_delete_node_back(_system_wifi_data **pphead) {
    if (!*pphead) return;
    if (!(*pphead)->next) {
        heap_caps_free(*pphead);
        *pphead = NULL;
    } else {
        _system_wifi_data *prev = NULL, *tail = *pphead;
        while (tail->next) {
            prev = tail;
            tail = tail->next;
        }
        heap_caps_free(tail);
        prev->next = NULL;
    }
}

// 删除头节点
void wifi_list_delete_node_front(_system_wifi_data **pphead) {
    if (!*pphead) return;
    _system_wifi_data *next = (*pphead)->next;
    heap_caps_free(*pphead);
    *pphead = next;
}

// 根据名字删除节点
void wifi_list_delete_node_name(_system_wifi_data **pphead, const char *name) {
    if (!*pphead) return;
    _system_wifi_data *prev = NULL, *current = *pphead;

    while (current) {
        if (strcmp(current->name, name) == 0) {
            if (!prev) {
                *pphead = current->next;
            } else {
                prev->next = current->next;
            }
            heap_caps_free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// 查找名字节点并返回指针
_system_wifi_data *wifi_list_find_name(_system_wifi_data *phead, const char *name) {
    _system_wifi_data *current = phead;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

uint32_t system_get_file_size(const char *path) {
    FILE *pread = fopen(path, "rb");
    if (!pread) {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Failed to open file: %s", path);
        return 0;
    }

    fseek(pread, 0, SEEK_END);
    uint32_t fileSize = ftell(pread);
    fclose(pread);
    log_printf(SYSTEMCONFIG_TAG, LOG_DEBUG, "File path: %s size: %d", path, fileSize);
    return fileSize;
}

// 读取配置
void system_read_config(void) {
    const char *config_file_path = "/spiffs/system/config.json";
    uint32_t file_size = system_get_file_size(config_file_path);
    if (!file_size) {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Failed to get file size");
        return;
    }

    FILE* file = fopen(config_file_path, "r");
    if (!file) {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Failed to open config file");
        return;
    }
    log_printf(SYSTEMCONFIG_TAG, LOG_DEBUG, "Opened config file successfully");

    char *buf = (char*)malloc(file_size + 1);
    if (!buf) {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Failed to allocate buffer with malloc");
        fclose(file);
        return;
    }
    memset(buf, 0x00, file_size + 1);

    size_t read_size = fread(buf, 1, file_size, file);
    fclose(file);
    if (read_size != file_size) {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Failed to read the entire file");
        free(buf);
        return;
    }

    log_printf(SYSTEMCONFIG_TAG, LOG_DEBUG, "Read file: %s", buf);

    cJSON *json_A = cJSON_Parse(buf);
    if (!json_A) {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Failed to parse JSON");
        free(buf);
        return;
    }

    cJSON *j_system_set = cJSON_GetObjectItem(json_A, "system_set");
    cJSON *j_wifi = cJSON_GetObjectItem(json_A, "wifi");

    if (j_system_set && cJSON_IsArray(j_system_set)) {
        cJSON *j_system_set_item = cJSON_GetArrayItem(j_system_set, 0);
        if (j_system_set_item) {
            cJSON *backlight_item = cJSON_GetObjectItem(j_system_set_item, "backlight");
            cJSON *wifi_switch_item = cJSON_GetObjectItem(j_system_set_item, "wifi switch");
        
            if (cJSON_IsNumber(backlight_item) && cJSON_IsNumber(wifi_switch_item)) {
                system_data.backlight = backlight_item->valueint;
                system_data.wifi_switch = wifi_switch_item->valueint;
                log_printf(SYSTEMCONFIG_TAG, LOG_INFO, "[system set]backlight:%d; WiFi switch:%d", system_data.backlight, system_data.wifi_switch);
            } else {
                log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Invalid system set data in JSON");
            }
        } else {
            log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "No valid system set item found in JSON");
        }
    } else {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "No system set data found in JSON");
    }

    if (j_wifi && cJSON_IsArray(j_wifi)) {
        cJSON *j_wifi_item = cJSON_GetArrayItem(j_wifi, 0);
        if (j_wifi_item) {
            cJSON *name_item = cJSON_GetObjectItem(j_wifi_item, "name");
            cJSON *password_item = cJSON_GetObjectItem(j_wifi_item, "password");
            cJSON *auto_connect_item = cJSON_GetObjectItem(j_wifi_item, "auto connect");

            if (cJSON_IsString(name_item) && cJSON_IsString(password_item) && cJSON_IsString(auto_connect_item)) {
                const char *name = name_item->valuestring;
                const char *password = password_item->valuestring;
                const char *auto_connect = auto_connect_item->valuestring;

                log_printf(SYSTEMCONFIG_TAG, LOG_INFO, "[WiFi]name:%s; password:%s; auto_connect:%s", name, password, auto_connect);

                snprintf(system_data.wifi_name, sizeof(system_data.wifi_name), "%s", name);
                snprintf(system_data.wifi_password, sizeof(system_data.wifi_password), "%s", password);

                wifi_list_add_node_back(&system_wifi_data, name, password, auto_connect);
            } else {
                log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Invalid WiFi data in JSON");
            }
        } else {
            log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "No valid WiFi data item found in JSON");
        }
    } else {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "No WiFi data found in JSON");
    }

    cJSON_Delete(json_A);
    free(buf);
}

int system_save_config(void) {
    const char *config_file_path = "/spiffs/system/config.json";
    cJSON* root = cJSON_CreateObject();
    if (!root) {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Failed to create JSON object");
        return 1;
    }

    cJSON* arr_system_set = cJSON_CreateArray();
    if (!arr_system_set) {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Failed to create system set array");
        cJSON_Delete(root);
        return 1;
    }

    cJSON* obj_system_set = cJSON_CreateObject();
    if (!obj_system_set) {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Failed to create system set object");
        cJSON_Delete(arr_system_set);
        cJSON_Delete(root);
        return 1;
    }

    cJSON_AddNumberToObject(obj_system_set, "wifi switch", system_get_wifi_switch());
    cJSON_AddNumberToObject(obj_system_set, "backlight", system_data.backlight);
    cJSON_AddNumberToObject(obj_system_set, "language", system_get_Language());
    cJSON_AddItemToArray(arr_system_set, obj_system_set);
    cJSON_AddItemToObject(root, "system_set", arr_system_set);

    cJSON* arr_wifi = cJSON_CreateArray();
    if (!arr_wifi) {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Failed to create WiFi array");
        cJSON_Delete(root);
        return 1;
    }

    cJSON* obj_wifi = cJSON_CreateObject();
    if (!obj_wifi) {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Failed to create WiFi object");
        cJSON_Delete(arr_wifi);
        cJSON_Delete(root);
        return 1;
    }

    cJSON_AddStringToObject(obj_wifi, "name", system_data.wifi_name);
    cJSON_AddStringToObject(obj_wifi, "password", system_data.wifi_password);
    cJSON_AddStringToObject(obj_wifi, "auto connect", "true");
    cJSON_AddItemToArray(arr_wifi, obj_wifi);
    cJSON_AddItemToObject(root, "wifi", arr_wifi);

    char* str = cJSON_PrintUnformatted(root);
    if (!str) {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Failed to print JSON object");
        cJSON_Delete(root);
        return 1;
    }

    int len = strlen(str);
    log_printf(SYSTEMCONFIG_TAG, LOG_DEBUG, "len:%d %s\n", len, cJSON_Print(root));

    FILE* file = fopen(config_file_path, "w+");
    if (!file) {
        log_printf(SYSTEMCONFIG_TAG, LOG_ERROR, "Failed to open config file for writing");
        cJSON_Delete(root);
        free(str);
        return 1;
    }

    fwrite(str, 1, len, file);
    fclose(file);
    log_printf(SYSTEMCONFIG_TAG, LOG_DEBUG, "Save file:\r\n%s\r\n", str);

    cJSON_Delete(root);
    free(str);
    return 0;
}

void system_init(void) {
    log_printf(SYSTEMCONFIG_TAG, LOG_DEBUG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = false
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        log_printf(SYSTEMCONFIG_TAG, LOG_DEBUG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        log_printf(SYSTEMCONFIG_TAG, LOG_DEBUG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        log_printf(SYSTEMCONFIG_TAG, LOG_DEBUG, "Partition size: total: %d, used: %d", total, used);
    }

    system_set_wifi_switch(true);

    system_wifi_data = wifi_list_create("NULL", "NULL", "NULL");
    if (!system_wifi_data) {
        log_printf(SYSTEMCONFIG_TAG, LOG_DEBUG, "WiFi link list create failed");
    }
    system_read_config();
}

void system_set_Language(int dat) {
    system_data.Language = dat;
    system_data.uart_rx_flag = 0;
    log_printf(SYSTEMCONFIG_TAG, LOG_DEBUG, "Set language: %d", system_data.Language);
}

int system_get_Language(void) {
    return system_data.Language;
}

int system_set_wifi_switch(int dat) {
    int dat1 = system_data.wifi_switch;
    system_data.wifi_switch = dat;
    return dat1;
}

int system_get_wifi_switch(void) {
    return system_data.wifi_switch;
}

void system_power_off(void) {
    // bsp_power_off();
}

int system_set_backlight(int dat) {
    system_data.backlight = dat;
	// bsp_ledc_set_duty(system_data.backlight);
	return system_data.backlight;
}

int system_get_backlight(void)
{
	return system_data.backlight;
}

