/*
Проект Идиатуллина Руслана - Игра лабиринт
Педагог - Гайнутдинов Р.Р.
*/

#include <alldefs.h>                //все найстройки define & includes

Random16 rnd;
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
EncButton encbut(3, 2, 0);

Player player(0, 0, 1);     // инициализация с координатами и ХП персонажа
Monster monster(0, 0, 1);   // инициализация с координатами осн монстра
Monster monster_3(8, 0, 0); // инициализация с координатами доп монстра lvl 3
Monster monster_4(8, 0, 0); // инициализация с координатами доп монстра lvl 4

KeyMapObject key_obj(0,3,1);  // инициализация с координатами ключа
DoorMapObject door_obj_lvlup(19, 2, 0, TYPE_DOOR_OBJECT);  // инициализация с координатами двери
DoorMapObject door_obj_restart(0,0,0, TYPE_RESTART_DOOR_OBJECT); // инициализация с координатами двери
DoorMapObject door_obj_gg(0,0,0, TYPE_GG_DOOR_OBJECT); // инициализация с координатами двери
DoorMapObject door_obj_om(0,0,0, TYPE_OM_DOOR_OBJECT); // инициализация с координатами двери
// TrapMapObject trap_obj(0,0,0); // инициализация с координатами ловушки
// HeartMapObject heart_obj(0,0,1); // инициализация с координатами сердца

// Написать класс карты?(объектов на карте). Случайное расположение 1/0

byte key[3] = {0, 3, 1};    // 0-x,1-y,2-сколько ключей in map
byte door[3] = {19, 2, 1};  // x,y, 1-закрыта/0-открыта
byte fake_door[3] = {6, 3, 0}; //x,y 1-есть/0-нет. Есть на 3 лвл
byte restart_door[3] = {0, 0, 0};
byte gg_door[3] = {0, 0, 0};
byte hearts[3] = {0, 0, 1}; // 0-х,1-у, 2-кол-во на карте
byte hp_pos_x = 19;
byte trap[3] = {4, 0, 0};   // 0-х,1-y,3-кол-во статичная ловушка
byte heart[3] = {17, 3, 1}; // 0-x,1-y,2-кол-во аптечек на карте

byte lvl = OPENING_LVL;      
// 0 - Opening, PRESS AND TURN, 1 monster
// 1 - фонарь, монстр и ловушка
// 2 = сейчас 1 монстр. без приколов/ Доделать ловушку 
// 3 =  фонарь и 2 монстра. посветка не моргает. 2 двери. близнецовый уровень
// 4 = сейчас заглушка. без приколов
// 108 
boolean lvlup = 0; // флаг для перехода в следующий уровень
byte cbuttons();

bool check_collision(Player* obj1, MapObject* obj2) {
  if (obj2->get_exist()) {
    if (obj1->getCurrentX() == obj2->getX() && obj1->getCurrentY() == obj2->getY()) {
        return true;
    }
  }
  return false;
}


void softwareReset(){
  asm volatile("jmp 0");
}
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
void gate(int8_t level)
{
  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print("level ");
  // lcd.print(level);
  // lcd.setCursor(0, 1);
  // lcd.print("hp:");
  // lcd.print(player.getHp());

  // lcd.setCursor(0, 2);
  // lcd.print("keys:");
  // lcd.print(player.getNumberOfKeys());
//add animation gate
  lcd.clear();
  play_animation(ANIMATION_GATE);
  uint32_t click_timer = millis();
  uint16_t period = 500;
  uint8_t click_loc_x = 6;
  uint8_t click_loc_y = 2;
  boolean click_is_visible = true;
  while (true)
  {
    if (millis() - click_timer > period)
    {
      lcd.clear();
      click_timer = millis();
      click_loc_x = rnd.get(0, 16);
      click_loc_y = rnd.get(0, 4);
      click_is_visible = !click_is_visible;
    }
    lcd.setCursor(click_loc_x, click_loc_y);
    click_is_visible ? lcd.print("click") : lcd.print("");

    encbut.tick();
    if (encbut.click())
      {
        delay(100);
        break;
      }
  }
}

///@brief check coords
void ccheck() // проверка координат
{
  // if (player.getCurrentX() < 0)
  //   player.setCurrentX(0);
  // if (player.getCurrentX() > 19)
  //   player.setCurrentX(19);
  // if (player.getCurrentY() < 0)
  //   player.setCurrentY(0);
  // if (player.getCurrentY() > 3)
  //   player.setCurrentY(3);
  player.checkMoveField();

    //NEW 
    // if (check_collision(&player, &key_obj))
    //   {
    //     player.takeKeys(1);     // добавляем ключ в карман
    //     key_obj.set_exist(0);   // вычитаем ключ из карты
    //     all_tone(TONE_PICK_UP); // звук подбора
    //   }

    // if (check_collision(&player, &heart_obj))
    //   {
    //     player.Heal(1);     // добавляем ключ в карман
    //     heart_obj.set_exist(0);   // вычитаем ключ из карты
    //     all_tone(TONE_HEAL); // звук подбора
    //   }

    // if (check_collision(&player, &door_obj))
    //   {
    //  switch (door_obj.get_type())
    //  {
    //  case NORM_DOOR:
      //  player.dropKeys(1);       //       вычитаем ключ из кармана
      //  door.set_exist(0);        //       вычитаем дверь из карты
      //  all_tone(TONE_OPEN_DOOR); // звук открытия двери
      //  vlup = true;
      //  lcd.clear();
    //    break;
    //  case RESTART_DOOR:
    //   break;
    //  case GG_DOOR:
    //   break;
    //  default:
    //   break;
    //  }   

  if (wall[player.getCurrentY()][player.getCurrentX()] == 1) // возвращени координат человек на прежнюю координату при столкновении со стеной
  {
    player.setCurrentX(player.getPreviousX());
    player.setCurrentY(player.getPreviousY());
  }

  // -------------------------------------------------------------------------------------------------------------------
  if (player.getCurrentX() == trap[0] && player.getCurrentY() == trap[1] && trap[2] > 0) // столкновение со статичной ловушкой
  {
    player.takeDamage(1);
    all_tone(TONE_TRAP_ACTIVATE);
  }
  // столкновение с движущимся монстром
  if (player.getCurrentX() == monster.getCurrentX() && player.getCurrentY() == monster.getCurrentY() && monster.getHp() > 0)
  {
    player.takeDamage(1);
    all_tone(TONE_MONSTER_DAMAGE);
  }
  // столкновение с дополнительным движущимся монстром
  if (player.getCurrentX() == monster_3.getCurrentX() && player.getCurrentY() == monster_3.getCurrentY() && monster_3.getHp() > 0)
    player.takeDamage(1);
 
   if (player.getCurrentX() == exitOm[0] && player.getCurrentY() == exitOm[1] && lvl == OM_LVL)
  // if (player.getCurrentX() == 18 && player.getCurrentY() == 2 && lvl == OM_LVL)
    {
      lvl = 0;
      lvlup = true;
    }
 

  if (player.getCurrentX() == enterOm[0] && player.getCurrentY() == enterOm[1] && lvl == OPENING_LVL)
    {
      lvlup = true;
      lvl = OM_LVL;
    }

  // if (immortality == true)
  //   return;

  if (player.getHp() <= 0)
  {
    gameOver();
  }  
}
void drawMap()
{
  for (int y = 0; y < 4; y++)
  {
    for (int x = player.fieldOfViewStart; x <= player.fieldOfViewEnd; x++) // если координаты стены в пределах фонаря(+-2 ед.) или дисплея(0,19)
    {
      if (wall[y][x] > 0) // и стена есть, торисуем её
      {
        lcd.setCursor(x, y);
        if (lvl == OM_LVL )
          lcd.write(SKIN_HEART);
        else
          lcd.write(SKIN_WALL);
           
      }
    }
  }
}

void mouth_animation(int8_t num)//TODO: delete delay, add timer
{
  for (int i = 1; i <= num; i++) // говорим ртом
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
    lcd.print(CHEL_STR_CHILL(1)); // Твои убеждения
    lcd.setCursor(3, 3);
    lcd.print(CHEL_STR_CHILL(2)); // демон.
    break;
  case EVIL_FRIENDS_DIALOG:
    lcd.setCursor(0, 2);
    lcd.print(MONSTERS_STR_EVIL_FRIENDS(3));
    break;
  }
  // delay(3000);
  wait_for_action();
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
    mouth_animation(6);
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
    mouth_animation(8);
    break;
  case 3:
    lcd.setCursor(0, 0);
    lcd.print(GOD_STR_OPEN(7)); // str17 - У вас есть свои
    lcd.setCursor(0, 1);
    lcd.print(GOD_STR_OPEN(8)); // 18 - особенности
    mouth_animation(8);
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
    mouth_animation(10);
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
    mouth_animation(12);
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
    mouth_animation(10);
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
    mouth_animation(10);
    break;
  }

  // delay(3000);
  wait_for_action();
  lcd.clear();
}
void monsterSays(uint16_t num)
{
  switch (num)
  {
  case MONSTER_SLABAK_DIALOG:
  {
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
  case EVIL_FRIENDS_DIALOG:
  {
    lcd.setCursor(0, 0);
    lcd.print(MONSTERS_STR_EVIL_FRIENDS(0));
    lcd.setCursor(0, 2);
    lcd.print(MONSTERS_STR_EVIL_FRIENDS(1));
    lcd.setCursor(0, 3);
    lcd.print(MONSTERS_STR_EVIL_FRIENDS(2));
    break;
  }
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
  case CHARS_EVIL_FRIENDS:

    break;
  case CHARS_GAME_OVER:

    break;
  case CHARS_OM_LVL:
    lcd.createChar(SKIN_CHEL, people);
    lcd.createChar(SKIN_KEY, keyChar);
    lcd.createChar(SKIN_DOOR, doorChar);
    lcd.createChar(SKIN_WALL, wallChar);
    
    lcd.createChar(SKIN_HEART, heartChar);
    lcd.createChar(SKIN_OM_L, OmCharL);
    lcd.createChar(SKIN_OM_R, OmCharR);

    break;
  case CHARS_ENDING:
    break;
  }
}
void play_animation(uint8_t num)
{
  switch (num)
  {
  case ANIMATION_PRESS_AND_TURN:
  {
    #ifdef OFF_ANIMATIONS
      break;
    #endif
    #ifdef OFF_ANIMATION_TRAINING
      break;
    #endif

    int8_t counter = 0;
    while (true)
    {
      const char press_word[6] = "PRESS";
      encbut.tick();

      for (int i = 0; i < 5 - counter; i++)
      {
        lcd.setCursor(CENTER_X + i, CENTER_Y);
        lcd.print(press_word[i]);
      }
      if (encbut.step())
      {
        counter++;
        lcd.clear();
      }
      if (counter == 6)
        break;
    }
    lcd.clear();
    counter = 0;
    while (true)
    {
      encbut.tick();
      lcd.setCursor(0 + counter, CENTER_Y);
      lcd.print("TURN");
      if (encbut.turn())
        counter++;
      if (counter == 16)
        break;
    }
    lcd.clear();
    counter = 0;
    while (true)
    {
      encbut.tick();
      lcd.setCursor(0, 0 + (counter / 2));
      lcd.print("PRESS AND TURN");
      if (encbut.turnH())
        counter++;
      if (counter == 8)
        break;
    }
    lcd.clear();
    
    while (true)
    {
      encbut.tick();
      lcd.setCursor(CENTER_X - 7, CENTER_Y);
      lcd.print("1 }|{u3Hb");
      delay(50);  //чтобы случайно не пропустить    
      wait_for_action();
      break;
    }
    break;
  }
  case ANIMATION_OPENING:
  {
    #ifdef OFF_ANIMATIONS 
      break;
    #endif
    #ifdef OFF_ANIMATION_OPENING 
      break;
    #endif
 
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
    // delay(1000); //
    wait_for_action();
    break;
  }
  case ANIMATION_FLASHLIGHT:
  {
    #ifdef OFF_ANIMATIONS
      break;
    #endif
    charsCreate(CHARS_DEFAULT);
    for (int i = 0; i < 6; i++) // мигаем фонариком
    {
      lcd.noBacklight();
      lcd.clear();
      lcd.setCursor(2, 1);
      lcd.write(SKIN_CHEL);
      drawMap();
      player.toggleFlashlight();
      toggleBacklight();
      delay(240);
    }
    lcd.backlight();
    lcd.clear();

    charsCreate(CHARS_GOD);
    writeFaceOfGod(16); // x position
    godSays(6);         // в подземелье тебе понадобится фонарик, он освещает лишь небольшую область
    godSays(7);         // Ещё здесь могут быть ловушки
    charsCreate(CHARS_DEFAULT);
    break;
  }
  case ANIMATION_MONSTER:
  {
    #ifdef OFF_ANIMATIONS
      break;
    #endif
    lcd.clear();
    charsCreate(CHARS_MONSTER);
    for (int i = 19; i > 6; i--) // идёт справа налево
    {
      lcd.setCursor(i, 1 + (i % 2));
      lcd.write(SKIN_MONSTER);
      lcd.setCursor(2, 1);
      lcd.write(SKIN_CHEL);
      delay(300);
      lcd.clear();
    }
    lcd.setCursor(6, 1);
    lcd.write(SKIN_MONSTER);
    monsterSays(MONSTER_SLABAK_DIALOG); // ты слабак
    // delay(4000);
    wait_for_action();
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.write(SKIN_CHEL);
    chelSays(CHEL_CHILL_DIALOG); // не
    // delay(4000);
    wait_for_action();
    lcd.clear();
    charsCreate(CHARS_DEFAULT);
    break;
  }
  case ANIMATION_EVIL_FRIENDS: // TODO
  {
    #ifdef OFF_ANIMATIONS
      break;
    #endif
    // charsCreate(CHARS_EVIL_FRIENDS);
    charsCreate(CHARS_MONSTER);
    lcd.clear();
    for (int i = 19; i > 6; i--) // идут справа налево
    {
      lcd.setCursor(i, 1 + (i % 2));
      lcd.write(SKIN_MONSTER);
      lcd.setCursor(7, (i % 4));
      lcd.write(SKIN_MONSTER);
      lcd.setCursor(2, 1);
      lcd.write(SKIN_CHEL);
      delay(300);
      lcd.clear();
    }
    lcd.setCursor(6, 1);
    lcd.write(SKIN_MONSTER);
    lcd.setCursor(7, 1);
    lcd.write(SKIN_MONSTER);

    monsterSays(EVIL_FRIENDS_DIALOG); // Нас 2
    // delay(4000);
    wait_for_action();
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.write(SKIN_CHEL);
    chelSays(EVIL_FRIENDS_DIALOG); // оба пизды получите
    // delay(4000);
    // lcd.clear();
    // charsCreate(CHARS_DEFAULT);
    break;
  }
  case ANIMATION_GATE:
  {
    #ifdef OFF_ANIMATIONS
      break;
    #endif
    // charsCreate(CHARS_GATE);
    lcd.clear();
    for (int i= 0; i < 19; i++)
    {
      lcd.setCursor(i, 0);
      lcd.write(SKIN_DOOR);
      lcd.setCursor(i, 1);
      lcd.write(SKIN_DOOR);
      lcd.setCursor(i, 2);
      lcd.write(SKIN_DOOR);
      lcd.setCursor(i, 3);
      lcd.write(SKIN_DOOR);
      delay(40+i*5);
    }
    lcd.noBacklight();
    delay(300);
    lcd.backlight();
    for (int i= 19; i >= 0; i--)
    {
      lcd.setCursor(i, 0);
      lcd.print(" ");
      lcd.setCursor(i, 1);
      lcd.print(" ");
      lcd.setCursor(i, 2);
      lcd.print(" ");
      lcd.setCursor(i, 3);
      lcd.print(" ");
      delay(30+i*5);
    }
    break;
  }
  case ANIMATION_ENDING:
  {
    #ifdef OFF_ANIMATIONS
      break;
    #endif
    charsCreate(CHARS_ENDING);

    break;
  }
  case ANIMATION_GAME_OVER:
  {
    #ifdef OFF_ANIMATIONS
      break;
    #endif
    switch (lvl)
    {
    case 50:
      break;
    default:
      {
        // charsCreate(CHARS_GAME_OVER);
        for(int i = 1; i < 16; i++)
        {
          toggleBacklight();
          delay(i*40);
        }
        lcd.noBacklight();
        break;
      }
    }
    break;
  }
  case ANIMATION_OM_LVL:
  {
    #ifdef OFF_ANIMATIONS
      break;
    #endif
    charsCreate(CHARS_OM_LVL);
    lcd.clear();
    for (int i= 0; i < 2; i++)
    {
      lcd.setCursor(CENTER_X, i);
      lcd.write(SKIN_OM_L);
      lcd.setCursor(CENTER_X+1, i);
      lcd.write(SKIN_OM_R);
      lcd.setCursor(CENTER_X, 3);
      lcd.write(SKIN_CHEL);
      delay(1500);
      lcd.clear();
    }
    lcd.setCursor(CENTER_X, 2);
    lcd.write(SKIN_OM_L);
    lcd.setCursor(CENTER_X+1, 2);
    lcd.write(SKIN_OM_R);
    lcd.setCursor(CENTER_X, 3);
    lcd.write(SKIN_CHEL);
    
    break;
  }
  default:
    break;
  }
}

void lvl_design() // вызываем в начале/конце каждого уровня
{
  gate(lvl); // переход(должен принимать номер уровня/ или нет, оставить lvl глобальной?
  lcd.clear();
  switch (lvl)
  {
  case OPENING_LVL:
  {
    play_animation(ANIMATION_OPENING);
    delay(200);
    play_animation(ANIMATION_PRESS_AND_TURN);

    player.setCurrentXY(2, 1);
    player.flashlight(OFF);
    // KeyMapObject key_o(2, 3, 1);
    // DoorMapObject door_o(19, 2, 1);
    // FakeDoorMapObject fake_door_o(19, 2, 0);
    
    // Есть ли на карте
    // door_obj.set_exist(1);
    // fake_door_obj.set_exist(1);
    key[2] = 1;
    door[2] = 1;
    fake_door[2] = 0;
    // x , y
    key[0] = 2;
    key[1] = 3;

    door[0] = 19;
    door[1] = 2;

    monster.setHp(1);
    monster.setFieldMoving(0, 17, 0, 3);
    monster.setCurrentXY(5, 3);
    break;
  }
  case FLASHLIGHT_LVL:
  {
    // change map 1
    for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 20; x++)
      {
        wall[y][x] = wall_1[y][x];
      }
    }
    play_animation(ANIMATION_FLASHLIGHT);
    player.flashlight(ON);
    player.setCurrentXY(2, 1);

    // add set orientation
    monster.setHp(1);
    monster.setFieldMoving(0, 18, 0, 3);
    monster.setCurrentXY(5, 3);

    // KeyMapObject key_o(0, 3, 1);
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
  }
  case 2:
  {
    // change map 2
    for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 20; x++)
      {
        wall[y][x] = wall_2[y][x];
      }
    }
    play_animation(ANIMATION_MONSTER); // У тебя ничего не получится
    player.flashlight(OFF);
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
  }
  case EVIL_FRIENDS_LVL:
  {
    // change map 3
    play_animation(ANIMATION_EVIL_FRIENDS); // зовёт друга
    for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 20; x++)
      {
        wall[y][x] = wall_3[y][x];
      }
    }
    player.flashlight(ON);
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
    fake_door[2] = 1;
    // x , y
    trap[0] = 4;
    trap[1] = 1;

    key[0] = 2;
    key[1] = 3;

    door[0] = 6;
    door[1] = 3;
    fake_door[0] = 3;
    fake_door[1] = 1;

    break;
  }
  case 4:
  {
    // change map 4
    // play_animation();
    for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 20; x++)
      {
        wall[y][x] = wall_4[y][x];
      }
    }
    player.flashlight(ON);
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
    fake_door[2] = 0;
    // x , y
    trap[0] = 4;
    trap[1] = 1;

    key[0] = 2;
    key[1] = 3;

    door[0] = 1;
    door[1] = 3;
    break;
  }
  case OM_LVL:
  {
    play_animation(ANIMATION_OM_LVL);
    // immortality = true;
    player.setHp(108);
    for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 20; x++)
      {
        wall[y][x] = wall_108[y][x];
      }
    }
    player.flashlight(OFF);
    player.setCurrentXY(CENTER_X, 3);
     // Есть ли на карте
    monster.setHp(0);
    trap[2] = 0;
    key[2] = 0;
    door[2] = 0;
    fake_door[2] = 0;  
   break; 
  }
  case END_LVL:
  {
    // play_animation(ANIMATION_END_LVL);
    for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 20; x++)
      {
        wall[y][x] = wall_108[y][x];
      }
    }
    player.flashlight(OFF);
    player.setCurrentXY(CENTER_X, 3);
    monster.setHp(0);
    trap[2] = 0;
    key[2] = 0;
    door[2] = 0;
    heart[2] = 0;
    fake_door[2] = 0;  
    restart_door[2] = 1;
    gg_door[2] = 1;

    // x , y
    restart_door[0] = 2;
    restart_door[1] = 2;
    gg_door[0] = 17;
    gg_door[1] = 2;

    
    break;
  }
  default:
  {break;}
  }
  draw();
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
    if (gg_door[2] > 0) // вывод двери выхода
    {
      lcd.setCursor(gg_door[0], gg_door[1]);
      lcd.write(SKIN_DOOR);
    }
    if (restart_door[2] > 0) // вывод двери рестарта
    {
      lcd.setCursor(restart_door[0], restart_door[1]);
      lcd.write(SKIN_DOOR);
    }
    if (fake_door[2] > 0) // вывод фальшивой двери
    {
      lcd.setCursor(fake_door[0], fake_door[1]);
      lcd.write(SKIN_DOOR);
    }
    if (lvl == OM_LVL)
    {
      lcd.setCursor(exitOm[0], exitOm[1]);
      lcd.write(SKIN_DOOR);
    }
    if (lvl == OM_LVL)
    {
      lcd.setCursor(CENTER_X, 1);
      lcd.write(SKIN_OM_L);
      lcd.setCursor(CENTER_X+1, 1);
      lcd.write(SKIN_OM_R);
    }
    
    // вывод количества hp персонажа
    if (lvl != END_LVL && lvl != OM_LVL)
    {
      lcd.setCursor(hp_pos_x, 0);
      lcd.print(player.getHp());
    }
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
    if (encbut.rightH())
    { //++y
      player.move(UP_DIR);
      return 1;
    }
    if (encbut.leftH())
    { //--y
      player.move(DOWN_DIR);
      return 1;
    }
    if (encbut.left())
    { //--x
      player.move(LEFT_DIR);
      return 1;
    }
    if (encbut.right())
    { //++x
      player.move(RIGHT_DIR);
      return 1;
    }

    if (encbut.click() && player.getCurrentX() == key[0] && player.getCurrentY() == key[1] && key[2] > 0)
    {                         // подбор ключа
      player.takeKeys(1);     // добавляем ключ в карман
      key[2]--;               // вычитаем ключ из карты
      all_tone(TONE_PICK_UP); // звук подбора
      return 1;
    }
    if (encbut.click() && player.getCurrentX() == heart[0] && player.getCurrentY() == heart[1] && heart[2] > 0)
    {                     // подбор жизни
      heart[2]--;         // вычитаем аптечкуиз карты
      player.takeHeal(1); // лечим перса
      all_tone(TONE_HEAL);
      return 1;
    }
    //NEW 
    // if (check_collision(&player, &key_obj))
    //   {
    //     player.takeKeys(1);     // добавляем ключ в карман
    //     key_obj.set_exist(0);   // вычитаем ключ из карты
    //     all_tone(TONE_PICK_UP); // звук подбора
    //     return 1;
    //   }

    // if (check_collision(&player, &heart_obj))
    //   {
    //     player.Heal(1);     // добавляем ключ в карман
    //     heart_obj.set_exist(0);   // вычитаем ключ из карты
    //     all_tone(TONE_HEAL); // звук подбора
    //     return 1;
    //   }

    // if (check_collision(&player, &door_obj))
    //   {
    //  switch (door_obj.get_type())
    //  {
    //  case NORM_DOOR:
      //  player.dropKeys(1);       //       вычитаем ключ из кармана
      //  door.set_exist(0);        //       вычитаем дверь из карты
      //  all_tone(TONE_OPEN_DOOR); // звук открытия двери
      //  vlup = true;
      //  lcd.clear();
      //  return 1;
    //    break;
    //  case RESTART_DOOR:
    //   break;
    //  case GG_DOOR:
    //   break;
    //  default:
    //   break;
    //  }   
   
    
    //     player.Heal(1);     // добавляем ключ в карман
    //     heart_obj.set_exist(0);   // вычитаем ключ из карты
    //     all_tone(TONE_HEAL); // звук подбора
    //     return 1;
    //   }

    if (encbut.click() && player.getCurrentX() == door[0] && player.getCurrentY() == door[1] && door[2] > 0 && player.getNumberOfKeys() > 0)
    {
      player.dropKeys(1);       //       вычитаем ключ из кармана
      door[2]--;                // вычиттаем дчверь из карты
      all_tone(TONE_OPEN_DOOR); // звук открытия двери
      lvlup = true;
      lcd.clear();
      return 1;
    }
    if (encbut.click() && player.getCurrentX() == restart_door[0] && player.getCurrentY() == restart_door[1] && restart_door[2] > 0)
    {
      restart_door[2]--;                // вычиттаем дчверь из карты
      all_tone(TONE_OPEN_DOOR); // звук открытия двери
      lcd.clear();
      softwareReset();
      return 1;
    }
    if (encbut.click() && player.getCurrentX() == gg_door[0] && player.getCurrentY() == gg_door[1] && gg_door[2] > 0)
    {
      gg_door[2]--;                // вычиттаем дчверь из карты
      all_tone(TONE_OPEN_DOOR); // звук открытия двери
      lcd.clear();
      play_animation(ANIMATION_ENDING);
      
      lcd.noBacklight();
      while (true){}
      return 1;
    }

    #ifdef DEVELOPER_MODE
      if (encbut.hold(2))//получить уровень 
       lvlup = true;
    if (encbut.holdFor(RESET_TIME_MS))//Сброс
      softwareReset();
    #endif

    return 0;
  }
  void toggleBacklight()
  {
    static bool backlight_state = false;
    if (backlight_state)
      {
        lcd.noBacklight();
        backlight_state = false;
      }
    else
    {
      lcd.backlight();
      backlight_state = true;
    }
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
    Serial.print(hp_pos_x);
    Serial.print("\t");
    Serial.println(lvl);
    
  }
  void setup()
  {
    rnd.setSeed(analogRead(0));
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
    encbut.tick();
    if (cbuttons())
    {                               // если любая кнопка была нажата
      monster.autoStep(HORIZONTAL); // двигаем монстра по горизонтали
      monster_3.autoStep(VERTICAL); // двигаем монстра на 3 уровне
      player.setFieldOfView();      // обновляем поле видимости
      ccheck();
      draw(); // отрисовка карты и всего остального
      if (lvl == FLASHLIGHT_LVL)
        toggleBacklight();     
    }
    if (lvlup)
    {
      if (lvl == OM_LVL)
      {
        // all_tone(OM_TONE);
        hp_pos_x = 17;
        // lvl_design();
        // lvlup = false;
      }
      else
      {
        all_tone(TONE_LVLUP); // звук перехода в следующий уровень
        lvl++;
      }
      lvl_design();  // поменять декорации
      lvlup = false; // сбрасываем флаг перехода в следующий уровень
    }
    debug();
  }

void wait_for_action() {
  // delay(800);
 while(true)
  { 
    encbut.tick();
    if (cbuttons())
       return;
    if (encbut.click())
       return;
    if (encbut.hold())
       return;
  }
  return;
}

void gameOver() {
  play_animation(ANIMATION_GAME_OVER);//togglebacklight
  softwareReset();
}