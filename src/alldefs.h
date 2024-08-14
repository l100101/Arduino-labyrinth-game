//system
#define RESET_TIME_MS   8000

#define ON              1
#define OFF             0
//LCD
#define CENTER_X          7
#define CENTER_Y          1

#define BUZZER_PIN          8
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
#define SKIN_CHEL       0
#define SKIN_KEY        1
#define SKIN_DOOR       2
#define SKIN_WALL       3
#define SKIN_FAKEDOOR   4
#define SKIN_HEART      5
#define SKIN_TRAP       6
#define SKIN_MONSTER    7
//SPRITES GOD
#define SKIN_EYE        1
// #define SKIN_NOSE_2 2 // nop
#define SKIN_NOSE       3
#define SKIN_FACE_1     4 // ЛОб
#define SKIN_FACE_2     5 //
#define SKIN_FACE_3     6 // Шея
#define SKIN_MOUTH      7  // РОТ
#define SKIN_MOUTH_OPEN 0

//ANIMATIONS PLAYS (in level design)
#define ANIMATION_OPENING           0
#define ANIMATION_FLASHLIGHT        1
#define ANIMATION_MONSTER           2
#define ANIMATION_EVIL_FRIENDS      3    
#define ANIMATION_PRESS_AND_TURN    80
#define ANIMATION_ENDING            99

//DIALOGS
#define CHEL_CHILL_DIALOG             100
#define MONSTER_SLABAK_DIALOG         600


//CREATE CHARS (in "playAnimation")
#define CHARS_DEFAULT   2
#define CHARS_GOD       3
#define CHARS_MONSTER   4
#define CHARS_ENDING    5


//STRINGS FOR DIALOGUee
#define FPSTR(pstr) (const __FlashStringHelper*)(pstr) // макрос для печати строк из PROGMEM на дисплей
#define CHEL_STR_OPEN(x) FPSTR(strChel_opening[(x)]) //упрощенный вид макроса выше. Для строк чела. OPENING
#define GOD_STR_OPEN(x) FPSTR(strGod_opening[(x)]) //упрощенный вид макроса выше. Для строк Бога. OPENING

// #define CHEL_STR_OPEN(x) FPSTR(strChel_flashlight[(x)]) //Для строк чела. FLASHLIGHT 
#define GOD_STR_FLASHLIGHT(x) FPSTR(strGod_flashlight[(x)]) //Для строк Бога. FLASHLIGHT

#define MONSTER_STR_SLABAK(x) FPSTR(strMonster_slabak[(x)])//У тебя не выйдет ничего
#define CHEL_STR_CHILL(x) FPSTR(strChel_chill[(x)])//выйдет.

// ---------------------------------------------------
//Настройки энкодера
#define EB_NO_FOR           // отключить поддержку pressFor/holdFor/stepFor и счётчик степов (экономит 2 байта оперативки)
#define EB_NO_COUNTER       // отключить счётчик энкодера (экономит 4 байта оперативки) 
// #define EB_NO_FOR           // отключить поддержку pressFor/holdFor/stepFor и счётчик степов (экономит 2 байта оперативки)
// #define EB_NO_CALLBACK      // отключить обработчик событий attach (экономит 2 байта оперативки)
// #define EB_NO_COUNTER       // отключить счётчик энкодера (экономит 4 байта оперативки)
// #define EB_NO_BUFFER        // отключить буферизацию энкодера (экономит 1 байт оперативки)

#define EB_STEP_TIME 100    // таймаут импульсного удержания (кнопка)
// #define EB_DEB_TIME 50      // таймаут гашения дребезга кнопки (кнопка)
// #define EB_CLICK_TIME 500   // таймаут ожидания кликов (кнопка)
#define EB_HOLD_TIME 300    // таймаут удержания (кнопка)
// #define EB_FAST_TIME 30     // таймаут быстрого поворота (энкодер)

// ---------------------------------------------------