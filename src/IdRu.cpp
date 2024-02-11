/*
Проект Идиатуллина Руслана - Игра лабиринт
Педагог - Гайнутдинов Р.Р.
*/

// несколько концовок
// добавть уровень на котором игрок теряет жизнь(1), и находит новую(1)
// ловушки меняющие свое состояние при каждом нажтии
// добавить ловушки+-
// создать класс для монстра

// сделать две двери на одном уровне
// сделать уровень в 2 экрана
// добавить 0 уровень(проверка кнопок)   +-

// ЗВУК И СЮЖЕТ

#include <GyverTimer.h> // подключаем библиотеку
#include <EncButton.h>  // подключаем библиотеку
#include "sprites.h"    //файл с рисунками карт и символов
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "../lib/Player/player.h"   //Класс персонажа
#include "../lib/Monster/monster.h" //Класс монстра
#include "alldefs.h"                //все найстройки define
#include "dialogs.h"                //все диалоги
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
Button up(BUTTON_UP);
Button down(BUTTON_DOWN);
Button left(BUTTON_LEFT);
Button right(BUTTON_RIGHT);

Player player(0, 0);   // инициализация с координатами персонажа
Monster monster(0, 0); // инициализация с координатами монстра
// Написать класс карты?(объектов на карте). Случайное расположение 1/0

byte key[3] = {0, 3, 1};    // 0-x,1-y,2-сколько ключей in map
byte door[3] = {19, 2, 1};  // 1-закрытаб, 0-открыта
byte hearts[3] = {0, 0, 1}; // 0-х,1-у, 2-кол-во на карте
byte trap[3] = {4, 0, 0};   // 0-х,1-y,3-кол-во статичная ловушка
byte heart[3] = {17, 3, 1}; // 0-x,1-y,2-кол-во аптечек на карте

byte lvl = 0;
boolean lvlup = 0; // флаг для перехода в следующий уровень
byte life = 5;

void all_tone(byte val)
{ // общий звук который будет использоваться часто (подбор и использование ключа, открытие двери и т.к)
  switch (val)
  {
  case 0: // общий звук
    tone(BUZZER_PIN, 2115, 100);
    delay(200);
    tone(BUZZER_PIN, 1800, 200);
    delay(200);
    tone(BUZZER_PIN, 3000, 300);
    break;
  case TONE_OPEN: // звук открытия ложной двери
    tone(BUZZER_PIN, 7000, 150);
    break;
  case TONE_LOSE: // звук проигрыша
    tone(BUZZER_PIN, 2500, 500);
    break;
  case 3:
    tone(BUZZER_PIN, 1500, 500); // 500
    delay(200);
    tone(BUZZER_PIN, 1800, 500); // 500
    delay(200);
    tone(BUZZER_PIN, 3000, 500); // 500
    break;
  }
}

void gate(int8_t level)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("level:");
  lcd.print(level);

  lcd.setCursor(0, 1);
  lcd.print("hp:");
  lcd.print(player.hp);

  lcd.setCursor(0, 2);
  lcd.print("keys:");
  lcd.print(player.getNumberOfKeys());

  delay(2000);
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
  if (player.getCurrentX() == trap[0] && player.getCurrentY() == trap[1]) // столкновение со статичной ловушкой
    player.getDamage(1);

  if (player.getCurrentX() == monster.getCurrentX() && player.getCurrentY() == monster.getCurrentY()) // столкновение с движущимся монстром
    player.getDamage(1);
}

void writeFaceOfGod(uint8_t pos)
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
  for (int i = 1; i <= 8; i++) // говорим ртом
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
    delay(500);
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
    break;
  }
  delay(3000);
  lcd.clear();
}
void charsCreate(uint8_t num)
{
  switch (num)
  {
  case ANIMATION_DEFAULT:
    lcd.createChar(0, people);
    lcd.createChar(1, keyChar);
    lcd.createChar(2, doorChar);
    lcd.createChar(3, wallChar);
    lcd.createChar(4, fakedoorChar);
    lcd.createChar(5, heartChar);
    // lcd.createChar(6, people);
    lcd.createChar(7, monsterChar);
    break;
  case ANIMATION_OPENING:
    lcd.createChar(1, eyeChar);
    lcd.createChar(2, oneNose);
    lcd.createChar(3, twoNose);
    lcd.createChar(4, faceOneChar);
    lcd.createChar(5, faceTwoChar);
    lcd.createChar(6, faceThreeChar);
    lcd.createChar(7, rotChar);
    lcd.createChar(0, openRotChar);
    break;
  case 3:
    // загрузить героев
    // нарисовать героев, написать диалог
    //
    break;
  }
}
void play_animation(uint8_t num)
{
  switch (num)
  {
  case ANIMATION_OPENING:
    charsCreate(ANIMATION_DEFAULT);
    lcd.setCursor(0, 3);
    lcd.write(SKIN_CHEL);
    chelSays(1); // ETO IGRA?

    charsCreate(ANIMATION_OPENING);
    writeFaceOfGod(16); // 16-x
    godSays(1);         // Здравстуй, ты попал сюда не случайно

    charsCreate(ANIMATION_DEFAULT);
    lcd.setCursor(0, 3);
    lcd.write(SKIN_CHEL);
    chelSays(2); // почему я здесь?

    charsCreate(ANIMATION_OPENING);
    writeFaceOfGod(16); // 16-x
    godSays(2);         // BaLLI XapakTep u noctynku onpede/|u/|u BaLLIy cyDb6y

    charsCreate(ANIMATION_DEFAULT);
    lcd.setCursor(0, 3);
    lcd.write(SKIN_CHEL);
    chelSays(3); // HO 9I BceGda

    charsCreate(ANIMATION_OPENING);
    writeFaceOfGod(16); // 16-x
    godSays(3);         // у вас свои особенности
    godSays(4);         // вы должны пройти Исптытания
    writeFaceOfGod(16);
    godSays(5); // тогда вы будете дойстойны возвращения домой

    charsCreate(ANIMATION_DEFAULT);
    lcd.setCursor(0, 3);
    lcd.write(SKIN_CHEL);
    chelSays(4); // Хорошо, я приму этот вызов

    delay(1000); // 3 sec
    break;

  case ANIMATION_ENDING:
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
    player.flashlight(1);
    lcd.setCursor(0, 0);
    lcd.print("press down button");
    lcd.setCursor(0, 1);
    lcd.print("to pick up items");
    lcd.setCursor(0, 2);
    lcd.print("you have 5 lives");
    key[0] = 2;
    key[1] = 3;
    key[2] = 1;

    door[0] = 19;
    door[1] = 2;
    door[2] = 1;

    monster.setFieldMoving(0, 17, 0, 3);
    monster.setCurrentXY(5, 3);
    break;

  case 1:
    key[0] = 0;
    key[1] = 3;
    key[2] = 1;

    door[0] = 19;
    door[1] = 2;
    door[2] = 1;

    player.setCurrentXY(1, 1);

    trap[0] = 3;
    trap[1] = 1;

    heart[0] = 12;
    heart[1] = 2;
    heart[2] = 1;

    // monster[2] = 0;

    for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 20; x++)
      {
        wall[y][x] = wall_1[y][x];
      }
    }
    break;
  case 2:
    key[0] = 15;
    key[1] = 1;
    key[2] = 1;

    door[0] = 19;
    door[1] = 2;
    door[2] = 1;

    player.setCurrentXY(1, 1);
    heart[2] = 0;
    trap[2] = 0;

    for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 20; x++)
      {
        wall[y][x] = wall_2[y][x];
      }
    }
    break;
  case 3:
    player.flashlight(1);
    player.setCurrentXY(1, 1);

    trap[0] = 7;
    trap[1] = 0;

    key[0] = 2;
    key[1] = 3;
    key[2] = 1;

    door[0] = 19;
    door[1] = 2;
    door[2] = 1;

    for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 20; x++)
      {
        wall[y][x] = wall_3[y][x];
      }
    }
    break;
  }
}
void draw()
{
  lcd.clear();

  for (int y = 0; y < 4; y++)
  {
    for (int x = player.fieldOfViewStart; x < player.fieldOfViewEnd; x++) // если координаты стены в пределах фонаря(+-2 ед.) или дисплея(0,19)
    {
      if (wall[y][x] > 0) // и стена есть, торисуем её
      {
        lcd.setCursor(x, y);
        lcd.write(3);
      }
    }
  }

  // вывод персонажа
  lcd.setCursor(player.getCurrentX(), player.getCurrentY());
  lcd.write(0);

  if (key[2] > 0) // вывод ключа
  {
    lcd.setCursor(key[0], key[1]);
    lcd.write(1);
  }
  if (door[2] > 0) // вывод двери
  {
    lcd.setCursor(door[0], door[1]);
    lcd.write(2);
  }
  // вывод количества hp в кармане
  lcd.setCursor(19, 0);
  lcd.print(player.hp);

  // вывод монстра
  lcd.setCursor(monster.getCurrentX(), monster.getCurrentY());

  lcd.write(7);

  if (trap[2] > 0)
  {
    lcd.setCursor(trap[0], trap[1]);
    lcd.print("^");
  }
  if (heart[2] > 0)
  {
    lcd.setCursor(heart[0], heart[1]);
    lcd.write(5);
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
  { // подбор ключа
    // chell[3]++;//прибавляем ключ в карман
    key[2]--;           // вычитаем ключ из карты
    all_tone(TONE_KEY); // звук подбора
    player.addKeys(1);  // добавляем ключ в карман
    return 1;
  }

  if (down.hold() && player.getCurrentX() == heart[0] && player.getCurrentY() == heart[1] && heart[2] > 0)
  { // подбор жизни
    // chell[2]++;//прибавляем ХП в карман
    heart[2]--; // вычитаем аптечкуиз карты
    // player.getHeal(1);
    all_tone(TONE_HEART);
    return 1;
  }

  if (down.hold() && player.getCurrentX() == door[0] && player.getCurrentY() == door[1] && door[2] > 0 && player.getNumberOfKeys() > 0)
  {
    // chell[3]--;//вычиттаем ключи из кармана
    player.dropKeys(1); //

    door[2]--;           // вычиттаем дчверь из карты
    all_tone(TONE_OPEN); // звук открытия двери
    lvlup = 1;
    lcd.clear();
    // lvl_design();
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
  Serial.print(player.hp);
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
  // CharsCreate(DEFAULT);
  charsCreate(ANIMATION_DEFAULT);
  lcd.backlight();
  pinMode(BUZZER_PIN, OUTPUT);
  all_tone(3);
  lvl_design();
}

void loop()
{
  // опрос кнопок, расчёты, дебаг
  up.tick();
  left.tick();
  down.tick();
  right.tick();
  if (cbuttons())
  { // если любая кнопка была нажата
    monster.autoStep(HORIZONTAL);
    player.setFieldOfView(); // обновляем поле видимости
    ccheck();
    draw(); // отрисовка карты
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
