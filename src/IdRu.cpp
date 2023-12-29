//сделать систему жизней

//сдеать переход с уровня на уровень +
//сделать надпись с кол-во ресурсов между уровнями   +
//сделать систему с фонариком ccheck, draw +
//сделать монстра      +

//сделать две двери на одном уровне
//сделать уровень в 2 экрана
//добавить 0 уровень(проверка кнопок)   +-

//добавть уровень на котором игрок теряет жизнь(1), и находит новую(1)
//ловушки меняющие свое состояние при каждом нажтии

//добавить ловушки+-

//При переходе на след уровень координаты чела менять   +

#include <GyverTimer.h>   // подключаем библиотеку
#include <EncButton.h>   // подключаем библиотеку
#include "sprites.h"  //файл с рисунками карт и символов
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "../lib/Player/player.h"

#define  BUTTON_UP 4
#define  BUTTON_DOWN  5  
#define  BUTTON_LEFT  7
#define  BUTTON_RIGHT  6
#define BUZZER_PIN 8

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
Button up (BUTTON_UP);
Button down(BUTTON_DOWN);
Button left( BUTTON_LEFT);
Button right(BUTTON_RIGHT);

Player player(0, 0, true, 0);

//создать класс для персонажа [V]
//создать класс для монстра
//int chell[4] = {1, 1, 5, 0};// 0-x,1-y,2-hp,3-haveKeys
//byte last_free[2] = {0, 0}; //0-x, 1-y, последняя свободняя координата
byte key[3] = {0, 3, 1};//0-x,1-y,2-сколько ключей in map
byte door[3] = {19, 2, 1}; //1-закрытаб, 0-открыта
byte flashlight[2] = {0, 4}; //область видимости фонаря по x
byte hearts[3] = {0, 0, 1}; //0-х,1-у, 2-кол-во на карте
byte trap[3] = {4, 0, 0}; //0-х,1-y,3-кол-во статичная ловушка
byte heart[3] = {17, 3, 1}; //0-x,1-y,2-кол-во аптечек на карте

byte lvl = 0;
boolean flashEnabled = 0;
byte pole_zrenia_1 = 0;
byte pole_zrenia_2 = 19;
byte life = 5;
byte  monster[4] = {5, 5, 9, 3}; //5x текущая, 5x-9x,3y
boolean dir = 0;//направление движ монстра

//Объединить все tone//заdefinить номер звука
void all_tone(byte val) { //общий звук который будет использоваться часто (подбор и использование ключа, открытие двери и т.к)
  switch (val)
  {
    case 0://общий звук
      tone(BUZZER_PIN, 2115, 100);
      delay(200);
      tone(BUZZER_PIN, 1800, 200);
      delay(200);
      tone(BUZZER_PIN, 3000, 300);
      break;
    case 1://звук открытия ложной двери
      tone(BUZZER_PIN, 7000, 150);
      break;
    case 2://звук проигрыша
      tone(BUZZER_PIN, 2500, 500);
      break;
    case 3:
      tone(BUZZER_PIN, 1500, 500);//500
      delay(200);
      tone(BUZZER_PIN, 1800, 500);//500
      delay(200);
      tone(BUZZER_PIN, 3000, 500);//500
      break;
  }
}

//void lvl_design();
//void debug();
//byte cbuttons();
//void ccheck();


void monstep()
{
  if (dir)
    monster[0]++;
  else
    monster[0]--;
  if (monster[0] < monster[1] || monster[0] > monster[2])
    dir = !dir;
}
void gate()
{
  //  ------------
  //ошибочный вывод 2 раза

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("level:");
  lcd.print(lvl);
  lcd.setCursor(0, 1);
  lcd.print("hp:");
  lcd.print(player.getHp());
  lcd.setCursor(0, 2);
  lcd.print("keys:");
  lcd.print(player.getNumberOfKeys());
  delay(3000);
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

void ccheck()//проверка координат
{
//  chell[0] = constrain(chell[0], 0 , 19);//ограниение куда может ходить человек
//  chell[1] = constrain(chell[1], 0, 3);
if (player.getCurrentX() < 0 ) player.setCurrentX(0);
if (player.getCurrentX() > 19 ) player.setCurrentX(19);
if (player.getCurrentY() < 0 ) player.setCurrentY(0);
if (player.getCurrentY() > 3 ) player.setCurrentY(3);


  if (flashEnabled)
  {
    flashlight[0] = player.getCurrentX() - 2;//поле видимости фонарика
    flashlight[1] = player.getCurrentX() + 2;//поле видимости фонарика

    flashlight[0] = constrain(flashlight[0], 0, 17);//от куда до куда может показывать фонарик
    flashlight[1] = constrain(flashlight[1], 2, 19);
  }
 
  if (wall[player.getCurrentY()][player.getCurrentX()] == 1)//возвращени координат человек на прежнюю координату
  {
    player.setCurrentX(player.getPreviousX());
    player.setCurrentY(player.getPreviousY());
  }
  //*

  // -------------------------------------------------------------------------------------------------------------------
  if (player.getCurrentX() == trap[0] && player.getCurrentY() == trap[1]) //столкновение со статичной ловушкой
  {
    player.getDamage(1);
  }

  if (player.getCurrentX() == monster[0] && player.getCurrentY() == monster[1]) //столкновение с движущимся монстром
  {
     player.getDamage(1);
  }

}
void lvl_design()//вызываем в начале/конце каждого уровня
{
  gate();//переход(должен принимать номер уровня
  switch (lvl) {
    case 0:
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


      monster[1] = 0;
      monster[2] = 17;
      monster[0] = monster[0]+1 ;
      
      monster[3] = 3;

      break;

    case 1:
      key[0] = 0;
      key[1] = 3;
      key[2] = 1;

      door[0] = 19;
      door[1] = 2;
      door[2] = 1;

      player.setCurrentX(1);
      player.setCurrentY(1);

      trap[0] = 3;
      trap[1] = 1;

      heart[0] = 12;
      heart[1] = 2;
      heart[2] = 1;

      monster[2] = 0;

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

      player.setCurrentX(1);
      player.setCurrentY(1);

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
      flashEnabled = 1;

      player.setCurrentX(1);
      player.setCurrentY(1);

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
  ccheck();
  lcd.clear();

  //if flashlight enabled
  if (flashEnabled)
  {
    pole_zrenia_1 =  flashlight[0];
    pole_zrenia_2 = flashlight[1];
  } else {
    pole_zrenia_1 =  0;
    pole_zrenia_2 = 19;
  }

  for (int y = 0; y < 4; y++)
  {
    for (int x = pole_zrenia_1; x < pole_zrenia_2 ; x++)//если координаты стены в пределах фонаря(+-2 ед.) или дисплея(0,19)
    {
      if (wall[y][x] > 0 )//и стена есть, торисуем её
      {
        lcd.setCursor(x, y);
        lcd.write(3);
      }
    }
  }

  //вывод персонажа
  //lcd.setCursor(chell[0], chell[1]);
  lcd.setCursor(player.getCurrentX(), player.getCurrentY());
  
  lcd.write(0);
  if (key[2] > 0)//вывод ключа
  {
    lcd.setCursor(key[0], key[1]);
    lcd.write(1);
  }
  if (door[2] > 0)//вывод двери
  {
    lcd.setCursor(door[0], door[1]);
    lcd.write(2);
  }
  //вывод количества hp в кармане
  lcd.setCursor(19, 0);
  lcd.print(player.getHp());

  //вывод монстра
  lcd.setCursor(monster[0], monster[3] );
  lcd.write(7);

  if (trap[2] > 0)
  {
    lcd.setCursor(trap[0], trap[1] );
    lcd.print("^");
  }
  if (heart[2] > 0)
  {
    lcd.setCursor(heart[0], heart[1] );
    lcd.write(5);
  }

}

byte cbuttons()
{
  if (up.click()) { //++y
    //chell[0]++;
    player.move(UP_DIR);
    Serial.print("UPCLICK\t");
    return 1;
  }
  if (down.click()) { //--y {
    //chell[1]++;
    player.move(DOWN_DIR);
    Serial.print("DOWNCLICK\t");
    return 1;
  }
  if (left.click()) { //--x {
    //chell[0]--;
    player.move(LEFT_DIR);
    Serial.print("LEFTCLICK\t");
    return 1;
  }
  if (right.click()) { //++x {
    //chell[1]--;
    player.move(RIGHT_DIR);
    Serial.print("RIGHTCLICK\t");
    return 1;
  }

  if (down.hold() && player.getCurrentX() == key[0] && player.getCurrentY() == key[1] && key[2] > 0) { //подбор ключа
    //chell[3]++;//прибавляем ключ в карман
    key[2]--;//вычитаем ключ из карты
    all_tone(0);//0-4(?)
    player.addKeys(1);//добавляем ключ в карман
    return 1;
  }

  if (down.hold() && player.getCurrentX() == heart[0] && player.getCurrentY() == heart[1] && heart[2] > 0) { //подбор жизни
   // chell[2]++;//прибавляем ХП в карман
    heart[2]--;//вычитаем аптечкуиз карты
    player.getHeal(1);
    all_tone(0);
    return 1;
  }

  if (down.hold() && player.getCurrentX() == door[0] && player.getCurrentY() == door[1] && door[2] > 0 && player.getNumberOfKeys() > 0 )
  {
    //chell[3]--;//вычиттаем ключи из кармана
    player.dropKeys(1);//
    door[2]--;//вычиттаем дчверь из карты
    all_tone(0);
    lvl++;
    lcd.clear();
    lvl_design();
    return 1;
  }
  return 0;
}



void debug()
{
 // Serial.print(up.read() + "\t");

  // Serial.print(player.getCurrentX());
  // Serial.print("\t");
  // Serial.println(player.getCurrentY());
  //Serial.print("\t");
  //Serial.println(player.getHp());
  //  Serial.print(pole_zrenia_2);
  // Serial.print("\t");
  // Serial.print(lvl);
  // Serial.print("\t");
  // Serial.print(heart[0]);
  // Serial.print(heart[1]);
  // Serial.println(heart[2]);
}
void setup()
{
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd
  lcd.init();
  lcd.createChar(0, people);
  lcd.createChar(1, keyChar);
  lcd.createChar(2, doorChar);
  lcd.createChar(3, wallChar);
  lcd.createChar(4, fakedoorChar);
  lcd.createChar(5, heartChar);
  //lcd.createChar(6, winChar);
  lcd.createChar(6, people);
  lcd.createChar(7, monsterChar);
  //  lcd.createChar(8, heartChar);
  lcd.backlight();

  pinMode(BUZZER_PIN, OUTPUT);
  all_tone(3);
  lvl_design();
}

void loop()
{
  up.tick();
  left.tick();
  down.tick();
  right.tick();
  //опрос кнопок, расчёты, дебаг
  if (cbuttons()) { //если любая кнопка была нажата
    monstep();
    draw();
  }
  //отрисовка тут(если cbuttons вернёт 1)
  debug();

}
