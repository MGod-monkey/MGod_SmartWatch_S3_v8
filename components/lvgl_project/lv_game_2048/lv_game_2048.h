﻿/**
 * @file lv_game_2048.h
 *
 */

#ifndef LV_GAME_2048_H
#define LV_GAME_2048_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"


/*********************
 *      DEFINES
 *********************/
#define MATRIX_SIZE     4

/**********************
 *      TYPEDEFS
 **********************/
/*Data of canvas*/
typedef struct {
    lv_obj_t obj;
    lv_obj_t * btnm;
    uint16_t score;
    uint16_t map_count;
    uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE];
    char    * btnm_map[MATRIX_SIZE * MATRIX_SIZE + MATRIX_SIZE];
    bool     game_over;
} lv_game_2048_t;

/***********************
 * GLOBAL VARIABLES
 ***********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
lv_obj_t * lv_game_2048_create(lv_obj_t * parent);
void lv_game_2048_simple_test();

/*=====================
 * Setter functions
 *====================*/
void lv_game_2048_set_new_game(lv_obj_t * obj);

/*=====================
 * Getter functions
 *====================*/
uint16_t lv_game_2048_get_best_tile(lv_obj_t * obj);

uint16_t lv_game_2048_get_score(lv_obj_t * obj);

bool lv_game_2048_get_status(lv_obj_t * obj);

/*=====================
 * Other functions
 *====================*/

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_GAME_2048_H*/