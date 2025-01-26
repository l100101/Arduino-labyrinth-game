// #ifndef ANIMATIONS_H
// #define ANIMATIONS_H
// #include <stdint.h>
// #include <Arduino.h>
// // #include "../include/alldefs.h"
// // #include <alldefs.h>

// //SPRITES GOD
// enum sprites_god {
//                 SKIN_EYE    = 1, 
//                 SKIN_NOSE   = 3,
//                 SKIN_FACE_1 = 4,
//                 SKIN_FACE_2 = 5, 
//                 SKIN_FACE_3 = 6, 
//                 SKIN_MOUTH  = 7, 
//             SKIN_MOUTH_OPEN = 0
// };


// #define FPSTR(pstr) (const __FlashStringHelper*)(pstr) // макрос для печати строк из PROGMEM на дисплей
// #define CHEL_STR_OPEN(x) FPSTR(strChel_opening[(x)]) //упрощенный вид макроса выше. Для строк чела. OPENING
// #define GOD_STR_OPEN(x) FPSTR(strGod_opening[(x)]) //упрощенный вид макроса выше. Для строк Бога. OPENING

// // #define CHEL_STR_OPEN(x) FPSTR(strChel_flashlight[(x)]) //Для строк чела. FLASHLIGHT 
// #define GOD_STR_FLASHLIGHT(x) FPSTR(strGod_flashlight[(x)]) //Для строк Бога. FLASHLIGHT

// #define MONSTER_STR_SLABAK(x) FPSTR(strMonster_slabak[(x)])//У тебя не выйдет ничего
// #define CHEL_STR_CHILL(x) FPSTR(strChel_chill[(x)])//выйдет.

// #define MONSTERS_STR_EVIL_FRIENDS(x) FPSTR(strEvil_friends[(x)])//Нас двое

// //DIALOGS
// #define CHEL_CHILL_DIALOG             100
// #define MONSTER_SLABAK_DIALOG         66
// #define EVIL_FRIENDS_DIALOG           67


// #include <LiquidCrystal_I2C.h>
// extern LiquidCrystal_I2C lcd;
// void writeFaceOfGod(uint8_t pos);
// void mouth_animation(int8_t num);//TODO: delete delay, add timer
// #endif // ANIMATIONS_H
