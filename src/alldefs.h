#ifndef ALLDEFS_H
#define ALLDEFS_H

//includes
#include <Arduino.h>
// #include "sound.h"                  //Звуки
// #include "../lib/GyverTimer/GyverTimer.h" //Таймеры и пр> // подключаем библиотеки
#include <GyverTimer.h>
// #include <Random16.h>
// #include "../lib/random16/Random16.h"
#include <Random16.h>
// #include "../lib/EncButton/EncButton.h
#include <EncButton.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "../lib/Map_obj/Map_obj.h"
#include "../lib/Map_obj/Key_obj.h"
#include "../lib/Map_obj/Door_obj.h"
#include "sprites.h"                //файл с рисунками карт и символов
#include "../lib/Player/Player.h"   //Класс персонажа
#include "../lib/Monster/monster.h" //Класс монстра
#include "../lib/animations/animations.h"             //Все анимации
#include "dialogs.h"                //все диалоги
// #include "animations.h"             //все анимации
//system
#define RESET_TIME_MS   6000

#define OPENING_LVL         0
#define FLASHLIGHT_LVL      1
#define EVIL_FRIENDS_LVL    3
#define END_LVL             99
#define END_LVL_CHOICE      100
#define OM_LVL              108
#define OM_LVL_PLAY         109


//---- DEBUG --------------------------------------------
// #define DEBUG                   1
// #define OFF_ANIMATIONS          1
// #define OFF_ANIMATION_OPENING   1
// #define OFF_ANIMATION_TRAINING      1
#define DEVELOPER_MODE          1
// #define IMMORTAL                1
// #define SOUNDS_ON               1
//---- DEBUG --------------------------------------------

#define ON              1
#define OFF             0
//LCD
#define CENTER_X          7
#define CENTER_Y          1

#define BUZZER_PIN          8//9 //off sound
//TONES NUMS
#define TONE_PICK_UP        0
#define TONE_OPEN_DOOR      1
#define TONE_TRAP_ACTIVATE  2
#define TONE_START_GAME     3
#define TONE_LOSE           4
#define TONE_WIN            5
#define TONE_HEAL           6
#define TONE_LVLUP          7
#define TONE_MONSTER_DAMAGE 8
//SPRITES DEFAULT 
/*
#define SKIN_CHEL       0
#define SKIN_KEY        1
#define SKIN_DOOR       2
#define SKIN_WALL       3
#define SKIN_FAKEDOOR   4
#define SKIN_HEART      5
#define SKIN_TRAP       6
#define SKIN_MONSTER    7
*/
enum sprites_default {
                SKIN_CHEL       = 0,
                SKIN_KEY        = 1,
                SKIN_DOOR       = 2,
                SKIN_WALL       = 3,
                SKIN_FAKEDOOR   = 4,
                SKIN_HEART      = 5,
                SKIN_TRAP       = 6,
                SKIN_MONSTER    = 7
};
// //SPRITES GOD
enum sprites_god {
                SKIN_EYE    = 1, 
                SKIN_NOSE   = 3,
                SKIN_FACE_1 = 4,
                SKIN_FACE_2 = 5, 
                SKIN_FACE_3 = 6, 
                SKIN_MOUTH  = 7, 
            SKIN_MOUTH_OPEN = 0
};

/**
#define SKIN_EYE        1
#define SKIN_NOSE_2 2 // nop
#define SKIN_NOSE       3
#define SKIN_FACE_1     4 // ЛОб
#define SKIN_FACE_2     5 //
#define SKIN_FACE_3     6 // Шея
#define SKIN_MOUTH      7  // РОТ
#define SKIN_MOUTH_OPEN 0
*/

#define SKIN_OM_L       6
#define SKIN_OM_R       7
//ANIMATIONS PLAYS (in level design)
/*
#define ANIMATION_OPENING           0
#define ANIMATION_FLASHLIGHT        1
#define ANIMATION_MONSTER           2
#define ANIMATION_EVIL_FRIENDS      3    
#define ANIMATION_PRESS_AND_TURN    80
#define ANIMATION_GATE              81                      
#define ANIMATION_GAME_OVER         98
#define ANIMATION_ENDING            99
#define ANIMATION_OM_LVL            108
*/
enum animations_plays {
            ANIMATION_OPENING           = 0,
            ANIMATION_FLASHLIGHT        = 1,
            ANIMATION_MONSTER           = 2,
            ANIMATION_EVIL_FRIENDS      = 3,
            ANIMATION_PRESS_AND_TURN    = 80,
            ANIMATION_GATE              = 81,
            ANIMATION_GAME_OVER         = 98,
            ANIMATION_ENDING            = 99,
            ANIMATION_OM_LVL            = 108
    
};
// //DIALOGS
#define CHEL_CHILL_DIALOG             100
#define MONSTER_SLABAK_DIALOG         66
#define EVIL_FRIENDS_DIALOG           67

//CREATE CHARS (in "playAnimation")
/*
#define CHARS_DEFAULT       2
#define CHARS_GOD           3
#define CHARS_MONSTER       4
#define CHARS_ENDING        5
#define CHARS_EVIL_FRIENDS  66
#define CHARS_GATE          81
#define CHARS_GAME_OVER     98
#define CHARS_OM_LVL        108
*/
enum create_chars {
            CHARS_DEFAULT       = 2,
            CHARS_GOD           = 3,
            CHARS_MONSTER       = 4,
            CHARS_ENDING        = 5,
            CHARS_EVIL_FRIENDS  = 66,
            CHARS_GATE          = 81,
            CHARS_GAME_OVER     = 98,
            CHARS_OM_LVL        = 108
};

//STRINGS FOR DIALOGUee
#define FPSTR(pstr) (const __FlashStringHelper*)(pstr) // макрос для печати строк из PROGMEM на дисплей
#define CHEL_STR_OPEN(x) FPSTR(strChel_opening[(x)]) //упрощенный вид макроса выше. Для строк чела. OPENING
#define GOD_STR_OPEN(x) FPSTR(strGod_opening[(x)]) //упрощенный вид макроса выше. Для строк Бога. OPENING

// #define CHEL_STR_OPEN(x) FPSTR(strChel_flashlight[(x)]) //Для строк чела. FLASHLIGHT 
#define GOD_STR_FLASHLIGHT(x) FPSTR(strGod_flashlight[(x)]) //Для строк Бога. FLASHLIGHT

#define MONSTER_STR_SLABAK(x) FPSTR(strMonster_slabak[(x)])//У тебя не выйдет ничего
#define CHEL_STR_CHILL(x) FPSTR(strChel_chill[(x)])//выйдет.

#define MONSTERS_STR_EVIL_FRIENDS(x) FPSTR(strEvil_friends[(x)])//Нас двое
// ---- encoder -----------------------------------------------
#define EB_NO_FOR           // отключить поддержку pressFor/holdFor/stepFor и счётчик степов (экономит 2 байта оперативки)
#define EB_NO_COUNTER       // отключить счётчик энкодера (экономит 4 байта оперативки) 
// #define EB_NO_FOR           // отключить поддержку pressFor/holdFor/stepFor и счётчик степов (экономит 2 байта оперативки)
// #define EB_NO_CALLBACK      // отключить обработчик событий attach (экономит 2 байта оперативки)
// #define EB_NO_COUNTER       // отключить счётчик энкодера (экономит 4 байта оперативки)
// #define EB_NO_BUFFER        // отключить буферизацию энкодера (экономит 1 байт оперативки)

#define EB_STEP_TIME 100    // таймаут импульсного удержания (кнопка)
// #define EB_DEB_TIME 50      // таймаут гашения дребезга кнопки (кнопка)
// #define EB_CLICK_TIME 500   // таймаут ожидания кликов (кнопка)
#define EB_HOLD_TIME 200    // таймаут удержания (кнопка)
// #define EB_FAST_TIME 30     // таймаут быстрого поворота (энкодер)

// ---- prototypes -----------------------------------------------
boolean immortality = false;
int8_t enterOm[3]={12,2, 0};//x,y| есть ли
int8_t exitOm[3]={18,2, 0};//x,y| есть ли
void draw();
void play_animation(uint8_t num);
void toggleBacklight(void);
void wait_for_action(void);
void gameOver();

#endif