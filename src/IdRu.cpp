/*
Проект Идиатуллина Руслана - Игра лабиринт
Педагог - Гайнутдинов Р.Р.
*/

#include <GyverTimer.h> // подключаем библиотеки
#include <EncButton.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "sprites.h"                //файл с рисунками карт и символов
#include "../lib/Player/player.h"   //Класс персонажа
#include "../lib/Monster/monster.h" //Класс монстра
#include "alldefs.h"                //все найстройки define
#include "dialogs.h"                //все диалоги

// создаём массив значений сигналов с кнопок
int16_t sigs[4] = {1023, 927, 856, 783};

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
// Button up(BUTTON_UP);
// Button down(BUTTON_DOWN);
// Button left(BUTTON_LEFT);
// Button right(BUTTON_RIGHT);
VirtButton up;
VirtButton down;
VirtButton left;
VirtButton right;

Player player(0, 0, 2);     // инициализация с координатами и ХП персонажа
Monster monster(0, 0, 1);   // инициализация с координатами осн монстра
Monster monster_3(8, 0, 0); // инициализация с координатами доп монстра lvl 3

// Написать класс карты?(объектов на карте). Случайное расположение 1/0

byte key[3] = {0, 3, 1};    // 0-x,1-y,2-сколько ключей in map
byte door[3] = {19, 2, 1};  // x,y, 1-закрыта/0-открыта
byte hearts[3] = {0, 0, 1}; // 0-х,1-у, 2-кол-во на карте
byte trap[3] = {4, 0, 0};   // 0-х,1-y,3-кол-во статичная ловушка
byte heart[3] = {17, 3, 1}; // 0-x,1-y,2-кол-во аптечек на карте

byte lvl = 0;      // 0
boolean lvlup = 0; // флаг для перехода в следующий уровень

void all_tone(byte val)
{ // звук
  switch (val)
  {
  case TONE_PICK_UP: // подбор предмета
    tone(BUZZER_PIN, 2115, 100);
    delay(200);
    tone(BUZZER_PIN, 1800, 200);
    delay(200);
    tone(BUZZER_PIN, 3000, 300);
    break;
  case TONE_OPEN_DOOR: // звук открытия двери
    tone(BUZZER_PIN, 7000, 150);
    break;
  case TONE_LOSE: // звук проигрыша
    tone(BUZZER_PIN, 2500, 500);
    break;
  case TONE_TRAP_ACTIVATE: // Срабатывания ловушки
    tone(BUZZER_PIN, 1500, 500);
    delay(200);
    tone(BUZZER_PIN, 1800, 500);
    delay(200);
    tone(BUZZER_PIN, 3000, 500);
    break;
  case TONE_START_GAME: // Начало игры
    tone(BUZZER_PIN, 1500, 500);
    delay(200);
    tone(BUZZER_PIN, 1800, 500);
    delay(200);
    tone(BUZZER_PIN, 3000, 500);
    break;
  case TONE_MONSTER_DAMAGE: // Урон от монстра
    tone(BUZZER_PIN, 500, 250);
    delay(300);
    tone(BUZZER_PIN, 100, 100);
    delay(150);
    break;
  }
}

void gate(int8_t level)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("level ");
  lcd.print(level);

  lcd.setCursor(0, 1);
  lcd.print("hp:");
  lcd.print(player.getHp());

  lcd.setCursor(0, 2);
  lcd.print("keys:");
  lcd.print(player.getNumberOfKeys());

  delay(1500);
  lcd.clear();
  while (1)
  {
    lcd.setCursor(0, 0);
    lcd.print("press left or");
    lcd.setCursor(0, 1);
    lcd.print("right button");
    if (!digitalRead(BUTTON_LEFT) || !digitalRead(BUTTON_RIGHT))
      break;
  }
}

void ccheck() // проверка координат
{
  if (player.getCurrentX() < 0)
    player.setCurrentX(0);
  if (player.getCurrentX() > 19)
    player.setCurrentX(19);
  if (player.getCurrentY() < 0)
    player.setCurrentY(0);
  if (player.getCurrentY() > 3)
    player.setCurrentY(3);

  if (wall[player.getCurrentY()][player.getCurrentX()] == 1) // возвращени координат человек на прежнюю координату при столкновении со стеной
  {
    player.setCurrentX(player.getPreviousX());
    player.setCurrentY(player.getPreviousY());
  }
  // -------------------------------------------------------------------------------------------------------------------
  if (player.getCurrentX() == trap[0] && player.getCurrentY() == trap[1] && trap[2] > 0) // столкновение со статичной ловушкой
  {
    player.takeDamage(1);
    // all_tone(TONE_TRAP_ACTIVATE);
  }
  // столкновение с движущимся монстром
  if (player.getCurrentX() == monster.getCurrentX() && player.getCurrentY() == monster.getCurrentY() && monster.getHp() > 0)
  {
    player.takeDamage(1);
    // all_tone(TONE_MONSTER_DAMAGE);
  }
  // столкновение с дополнительным движущимся монстром
  if (player.getCurrentX() == monster_3.getCurrentX() && player.getCurrentY() == monster_3.getCurrentY() && monster_3.getHp() > 0)
    player.takeDamage(1);
}
void drawMap()
{
  for (int y = 0; y < 4; y++)
  {
    for (int x = player.fieldOfViewStart; x < player.fieldOfViewEnd; x++) // если координаты стены в пределах фонаря(+-2 ед.) или дисплея(0,19)
    {
      if (wall[y][x] > 0) // и стена есть, торисуем её
      {
        lcd.setCursor(x, y);
        lcd.write(SKIN_WALL);
      }
    }
  }
}
void writeFaceOfGod(uint8_t pos) // x position
{
  lcd.setCursor(pos, 1);
  lcd.write(SKIN_NOSE);
  lcd.setCursor(pos + 1, 0);
  lcd.write(SKIN_FACE_1);
  lcd.setCursor(pos + 1, 1);
  lcd.write(SKIN_EYE);
  lcd.setCursor(pos + 1, 2);
  lcd.write(SKIN_MOUTH);

  for (int i = 0; i <= 2; i++)
  {
    lcd.setCursor(pos + 2, i);
    lcd.print("#");
  }
  lcd.setCursor(pos + 2, 3);
  lcd.write(SKIN_FACE_3);
  for (int i = 0; i <= 3; i++)
  {
    lcd.setCursor(pos + 3, i);
    lcd.print("#");
  }
}
void mouth_animation()
{
  for (int i = 1; i <= 12; i++) // говорим ртом
  {
    lcd.setCursor(17, 2);
    if (i % 2 == 0)
    {
      lcd.write(SKIN_MOUTH);
    }
    else
    {
      lcd.write(SKIN_MOUTH_OPEN);
    }
    delay(300);
  }
}
void chelSays(uint8_t num)
{

  switch (num)
  {
  case 1:
    lcd.setCursor(0, 0);
    lcd.print(CHEL_STR_OPEN(0)); // что за...
    lcd.setCursor(0, 1);
    lcd.print(CHEL_STR_OPEN(1)); // Это игра?
    break;
  case 2:
    lcd.setCursor(0, 0);
    lcd.print(CHEL_STR_OPEN(2)); // Почему
    lcd.setCursor(0, 1);
    lcd.print(CHEL_STR_OPEN(3)); // я здесь?
    break;
  case 3:
    lcd.setCursor(0, 0);
    lcd.print(CHEL_STR_OPEN(4)); // Но я всегда
    lcd.setCursor(0, 1);
    lcd.print(CHEL_STR_OPEN(5)); // Старался
    lcd.setCursor(0, 2);
    lcd.print(CHEL_STR_OPEN(6)); // Быть добрым
    break;
  case 4:
    lcd.setCursor(0, 0);
    lcd.print(CHEL_STR_OPEN(7)); // Хорошо
    lcd.setCursor(0, 1);
    lcd.print(CHEL_STR_OPEN(8)); // Я приму этот вызов!
    lcd.setCursor(0, 2);
    lcd.print(CHEL_STR_OPEN(9)); // Укажи мне путь!
    break;
  case CHEL_CHILL_DIALOG:
    lcd.setCursor(0, 0);
    lcd.print(CHEL_STR_CHILL(0)); // Меня не беспокоят
    lcd.setCursor(0, 2);
    lcd.print(CHEL_STR_CHILL(1)); //Твои убеждения
    lcd.setCursor(3, 3);
    lcd.print(CHEL_STR_CHILL(2)); //демон.
    break;
  }
  delay(3000);
  lcd.clear();
}
void godSays(uint8_t num)
{
  switch (num)
  {
  case 1:
    lcd.setCursor(0, 0);
    lcd.print(GOD_STR_OPEN(0)); // Здравствуй
    lcd.setCursor(0, 1);
    lcd.print(GOD_STR_OPEN(1)); // ты попал сюда
    lcd.setCursor(0, 2);
    lcd.print(GOD_STR_OPEN(2)); // не случайно
    mouth_animation();
    break;
  case 2:
    lcd.setCursor(0, 0);
    lcd.print(GOD_STR_OPEN(3)); // Ваш характер
    lcd.setCursor(0, 1);
    lcd.print(GOD_STR_OPEN(4)); //
    lcd.setCursor(0, 2);
    lcd.print(GOD_STR_OPEN(5));
    lcd.setCursor(0, 3);
    lcd.print(GOD_STR_OPEN(6));
    mouth_animation();
    break;
  case 3:
    lcd.setCursor(0, 0);
    lcd.print(GOD_STR_OPEN(7)); // str17 - У вас есть свои
    lcd.setCursor(0, 1);
    lcd.print(GOD_STR_OPEN(8)); // 18 - особенности
    mouth_animation();
    break;
  case 4:
    lcd.setCursor(0, 0);
    lcd.print(GOD_STR_OPEN(9)); // Вы должны
    lcd.setCursor(0, 1);
    lcd.print(GOD_STR_OPEN(10)); // Пройти испытания
    lcd.setCursor(0, 2);
    lcd.print(GOD_STR_OPEN(11)); // Чтобы понять
    lcd.setCursor(0, 3);
    lcd.print(GOD_STR_OPEN(12)); // о чем я говорю.
    mouth_animation();
    break;
  case 5:
    lcd.setCursor(0, 0);
    lcd.print(GOD_STR_OPEN(13)); // будет непросто
    lcd.setCursor(0, 1);
    lcd.print(GOD_STR_OPEN(14)); // Но только так
    lcd.setCursor(0, 2);
    lcd.print(GOD_STR_OPEN(15)); // вы будете достойны
    lcd.setCursor(0, 3);
    lcd.print(GOD_STR_OPEN(16)); // возвращения домой
    mouth_animation();
    break;
  case 6:
    lcd.setCursor(0, 0);
    lcd.print(GOD_STR_FLASHLIGHT(0)); //
    lcd.setCursor(0, 1);
    lcd.print(GOD_STR_FLASHLIGHT(1)); //
    lcd.setCursor(0, 2);
    lcd.print(GOD_STR_FLASHLIGHT(2)); //
    lcd.setCursor(0, 3);
    lcd.print(GOD_STR_FLASHLIGHT(3)); //
    mouth_animation();
    break;
  case 7:
    lcd.setCursor(0, 0);
    lcd.print(GOD_STR_FLASHLIGHT(4)); // Здесь
    lcd.setCursor(0, 1);
    lcd.print(GOD_STR_FLASHLIGHT(5)); // могут быть
    lcd.setCursor(0, 2);
    lcd.print(GOD_STR_FLASHLIGHT(6)); // ловушки
    lcd.setCursor(0, 3);
    lcd.print(GOD_STR_FLASHLIGHT(7)); // Будь осторожен
    mouth_animation();
    break;
  }

  delay(3000);
  lcd.clear();
}
void monsterSays(uint16_t num)
{
  switch (num)
  {
  case MONSTER_SLABAK_DIALOG:
    lcd.setCursor(0, 0);
    lcd.print(MONSTER_STR_SLABAK(0));
    lcd.setCursor(0, 2);
    lcd.print(MONSTER_STR_SLABAK(1));
    lcd.setCursor(8, 3);
    lcd.print(MONSTER_STR_SLABAK(2));
    lcd.setCursor(0, 3);
    lcd.print(MONSTER_STR_SLABAK(3));  
    break;
  }
}
void charsCreate(uint8_t num)
{
  switch (num)
  {
  case CHARS_DEFAULT:
  case CHARS_MONSTER:
    lcd.createChar(SKIN_CHEL, people);
    lcd.createChar(SKIN_KEY, keyChar);
    lcd.createChar(SKIN_DOOR, doorChar);
    lcd.createChar(SKIN_WALL, wallChar);
    lcd.createChar(SKIN_FAKEDOOR, fakedoorChar);
    lcd.createChar(SKIN_HEART, heartChar);
    // lcd.createChar(6, );
    lcd.createChar(SKIN_MONSTER, monsterChar);
    break;
  case CHARS_GOD:
    lcd.createChar(SKIN_EYE, eyeChar);
    lcd.createChar(SKIN_NOSE, oneNose);
    lcd.createChar(SKIN_NOSE, twoNose);
    lcd.createChar(SKIN_FACE_1, faceOneChar);
    lcd.createChar(SKIN_FACE_2, faceTwoChar);
    lcd.createChar(SKIN_FACE_3, faceThreeChar);
    lcd.createChar(SKIN_MOUTH, rotChar);
    lcd.createChar(SKIN_MOUTH_OPEN, openRotChar);
    break;
  case CHARS_ENDING:
    break;  
  }
}
void play_animation(uint8_t num)
{
  switch (num)
  {
  case ANIMATION_OPENING:
    charsCreate(CHARS_DEFAULT);
    lcd.setCursor(0, 3);
    lcd.write(SKIN_CHEL);
    chelSays(1); // ETO IGRA?

    charsCreate(CHARS_GOD);
    writeFaceOfGod(16); // 16-x
    godSays(1);         // Здравстуй, ты попал сюда не случайно

    charsCreate(CHARS_DEFAULT);
    lcd.setCursor(0, 3);
    lcd.write(SKIN_CHEL);
    chelSays(2); // почему я здесь?

    charsCreate(CHARS_GOD);
    writeFaceOfGod(16); // 16-x
    godSays(2);         // Ваш характер и поступки определили вашу судьбу

    charsCreate(CHARS_DEFAULT);
    lcd.setCursor(0, 3);
    lcd.write(SKIN_CHEL);
    chelSays(3); // Но я всегда старался быть добрым

    charsCreate(CHARS_GOD);
    writeFaceOfGod(16); // 16-x
    godSays(3);         // у вас свои особенности
    godSays(4);         // вы должны пройти Исптытания
    writeFaceOfGod(16);
    godSays(5); // тогда вы будете дойстойны возвращения домой

    charsCreate(CHARS_DEFAULT);
    lcd.setCursor(0, 3);
    lcd.write(SKIN_CHEL);
    chelSays(4); // Хорошо, я приму этот вызов
    delay(1000); //
    break;
  case ANIMATION_FLASHLIGHT:
    charsCreate(CHARS_DEFAULT);
    for (int i = 0; i < 12; i++) // мигаем фонариком
    {
      lcd.clear();
      lcd.setCursor(2, 1);
      lcd.write(SKIN_CHEL);
      drawMap();
      player.toggleFlashlight();
      delay(280);
    }
    lcd.clear();

    charsCreate(CHARS_GOD);
    writeFaceOfGod(16); // x position
    godSays(6);         // в подземелье тебе понадобится фонарик, он освещает лишь небольшую область
    godSays(7);         // Ещё здесь могут быть ловушки
    charsCreate(CHARS_DEFAULT);
    break;
  case ANIMATION_MONSTER:
    lcd.clear();
    charsCreate(CHARS_MONSTER);
    for (int i = 19; i > 6; i--)//идёт справа налево
    {
      lcd.setCursor(i, 1);
      lcd.write(SKIN_MONSTER);
      lcd.setCursor(2, 1);
      lcd.write(SKIN_CHEL);
      delay(300);
      lcd.clear();
    }
    lcd.setCursor(6, 1);
    lcd.write(SKIN_MONSTER);
    monsterSays(MONSTER_SLABAK_DIALOG);//ты слабак
    delay(4000);
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.write(SKIN_CHEL);
    chelSays(CHEL_CHILL_DIALOG);//не
    delay(4000);
    lcd.clear();
    charsCreate(CHARS_DEFAULT);
    break;
  case ANIMATION_EVIL_FRIENDS:
    // charsCreate(CHARS_ENDING);


  case ANIMATION_ENDING:
    charsCreate(CHARS_ENDING);
    break;
  }
}

void lvl_design() // вызываем в начале/конце каждого уровня
{
  gate(lvl); // переход(должен принимать номер уровня/ или нет, оставить lvl глобальной?
  lcd.clear();
  switch (lvl)
  {
  case 0:
    play_animation(ANIMATION_OPENING);
    lcd.setCursor(0, 0);
    lcd.print("press down button");
    lcd.setCursor(0, 1);
    lcd.print("to pick up items");
    lcd.setCursor(0, 2);
    lcd.print("you have 5 lives");

    player.flashlight(0);
    // Есть ли на карте
    key[2] = 1;
    door[2] = 1;
    // x , y
    key[0] = 2;
    key[1] = 3;

    door[0] = 19;
    door[1] = 2;

    monster.setHp(1);
    monster.setFieldMoving(0, 17, 0, 3);
    monster.setCurrentXY(5, 3);
    break;

  case 1:
    // change map 1
    for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 20; x++)
      {
        wall[y][x] = wall_1[y][x];
      }
    }
    play_animation(ANIMATION_FLASHLIGHT);
    player.flashlight(1);
    player.setCurrentXY(2, 1);

    // add set orientation
    monster.setHp(1);
    monster.setFieldMoving(0, 18, 0, 3);
    monster.setCurrentXY(5, 3);
    // Есть ли на карте
    key[2] = 1;
    door[2] = 1;
    trap[2] = 1;
    heart[2] = 1;
    // x , y
    key[0] = 0;
    key[1] = 3;

    door[0] = 19;
    door[1] = 2;

    trap[0] = 9;
    trap[1] = 2;

    heart[0] = 12;
    heart[1] = 2;
    break;
  case 2:
    // change map 2
    for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 20; x++)
      {
        wall[y][x] = wall_2[y][x];
      }
    }
    play_animation(ANIMATION_MONSTER);//У тебя ничего не получится
    player.flashlight(0);
    player.setCurrentXY(1, 1);
    monster.setHp(1);
    monster.setFieldMoving(0, 17, 0, 3);
    monster.setCurrentXY(6, 1);
    // Есть ли на карте
    key[2] = 1;
    door[2] = 1;
    heart[2] = 0;
    trap[2] = 0;
    // x , y
    key[0] = 15;
    key[1] = 1;

    door[0] = 19;
    door[1] = 2;

    break;
  case 3:
    // change map 3
    play_animation(ANIMATION_EVIL_FRIENDS);//зовёт друга
    for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 20; x++)
      {
        wall[y][x] = wall_3[y][x];
      }
    }
    player.flashlight(1);
    player.setCurrentXY(19, 2);
    monster.setHp(1);
    monster.setFieldMoving(0, 17, 0, 3);
    monster.setCurrentXY(5, 3);
    monster_3.setHp(1);
    monster_3.setFieldMoving(3, 19, 0, 3);
    monster_3.setCurrentXY(8, 3);
    // Есть ли на карте
    trap[2] = 1;
    key[2] = 1;
    door[2] = 1;
    // x , y
    trap[0] = 4;
    trap[1] = 1;

    key[0] = 2;
    key[1] = 3;

    door[0] = 1;
    door[1] = 3;
    break;
  }
}

void draw()
{
  lcd.clear();
  // вывод карты
  drawMap();
  // вывод персонажа
  lcd.setCursor(player.getCurrentX(), player.getCurrentY());
  lcd.write(SKIN_CHEL);

  if (key[2] > 0) // вывод ключа
  {
    lcd.setCursor(key[0], key[1]);
    lcd.write(SKIN_KEY);
  }
  if (door[2] > 0) // вывод двери
  {
    lcd.setCursor(door[0], door[1]);
    lcd.write(SKIN_DOOR);
  }
  // вывод количества hp в кармане
  lcd.setCursor(19, 0);
  lcd.print(player.getHp());

  // вывод основного монстра
  if (monster.getHp() > 0)
  {
    lcd.setCursor(monster.getCurrentX(), monster.getCurrentY());
    lcd.write(SKIN_MONSTER);
  }

  if (monster_3.getHp() > 0) // вывод доп. монстра
  {
    lcd.setCursor(monster_3.getCurrentX(), monster_3.getCurrentY());
    lcd.write(SKIN_MONSTER);
  }

  if (trap[2] > 0)
  {
    lcd.setCursor(trap[0], trap[1]);
    lcd.print("^");
  }
  if (heart[2] > 0)
  {
    lcd.setCursor(heart[0], heart[1]);
    lcd.write(SKIN_HEART);
  }
}
byte cbuttons()
{
  if (up.click())
  { //++y
    player.move(UP_DIR);
    return 1;
  }
  if (down.click())
  { //--y
    player.move(DOWN_DIR);
    return 1;
  }
  if (left.click())
  { //--x
    player.move(LEFT_DIR);
    return 1;
  }
  if (right.click())
  { //++x
    player.move(RIGHT_DIR);
    return 1;
  }

  if (down.hold() && player.getCurrentX() == key[0] && player.getCurrentY() == key[1] && key[2] > 0)
  {                         // подбор ключа
    player.takeKeys(1);     // добавляем ключ в карман
    key[2]--;               // вычитаем ключ из карты
    all_tone(TONE_PICK_UP); // звук подбора
    return 1;
  }

  if (down.hold() && player.getCurrentX() == heart[0] && player.getCurrentY() == heart[1] && heart[2] > 0)
  {                     // подбор жизни
    heart[2]--;         // вычитаем аптечкуиз карты
    player.takeHeal(1); // лечим перса
    all_tone(TONE_HEAL);
    return 1;
  }

  if (down.hold() && player.getCurrentX() == door[0] && player.getCurrentY() == door[1] && door[2] > 0 && player.getNumberOfKeys() > 0)
  {
    player.dropKeys(1);       //       вычитаем ключ из кармана
    door[2]--;                // вычиттаем дчверь из карты
    all_tone(TONE_OPEN_DOOR); // звук открытия двери
    lvlup = 1;
    lcd.clear();
    return 1;
  }
  return 0;
}
void debug()
{
  //  Serial.print(player.fieldOfViewStart );
  //  Serial.print("\t");
  //  Serial.println(player.fieldOfViewEnd);
  // Serial.print(player.getCurrentX());
  // Serial.print("\t");
  // Serial.println(player.getCurrentY());
  // Serial.print("\t");
  // Serial.print("\t");
  // Serial.print(lvl);
  // Serial.print("\t");
  Serial.print(player.getHp());
  Serial.print("\t");
  Serial.print(heart[0]);
  Serial.print("\t");
  Serial.print(heart[1]);
  Serial.print("\t");
  Serial.println(heart[2]);
}
void setup()
{
  Serial.begin(9600);
  lcd.init(); // initialize the lcd
  lcd.init();
  charsCreate(CHARS_DEFAULT);
  lcd.backlight();
  pinMode(BUZZER_PIN, OUTPUT);
  all_tone(TONE_START_GAME);
  lvl_design();
}
void loop()
{
  // опрос кнопок, расчёты, дебаг
  up.tick(      BUTTON_UP_VALUE+WINDOW_WIDTH>   analogRead(KEYPAD_PIN) > BUTTON_UP_VALUE-WINDOW_WIDTH);
  left.tick(    BUTTON_LEFT_VALUE+WINDOW_WIDTH> analogRead(KEYPAD_PIN) > BUTTON_LEFT_VALUE-WINDOW_WIDTH);
  down.tick(    BUTTON_DOWN_VALUE+WINDOW_WIDTH> analogRead(KEYPAD_PIN) > BUTTON_DOWN_VALUE-WINDOW_WIDTH);
  right.tick(   BUTTON_RIGHT_VALUE+WINDOW_WIDTH>analogRead(KEYPAD_PIN) > BUTTON_RIGHT_VALUE-WINDOW_WIDTH);
  printf("%d", analogRead(KEYPAD_PIN));
  if (cbuttons())
  {                               // если любая кнопка была нажата
    monster.autoStep(HORIZONTAL); // двигаем монстра по горизонтали
    monster_3.autoStep(VERTICAL); // двигаем монстра на 3 уровне
    player.setFieldOfView();      // обновляем поле видимости
    ccheck();
    draw(); // отрисовка карты и всего остального
  }
  if (lvlup)
  {
    all_tone(TONE_LVLUP); // звук перехода в следующий уровень
    lvl++;
    lvl_design(); // поменять декорации
    lvlup = 0;    // сбрасываем флаг перехода в следующий уровень
  }
  debug();
}
