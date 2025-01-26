## Игра лабиринт с сюжетом  
Герой оказывается внутри игры и ему предстоит выбраться

------------------------------------------
## Железки:
- Arduino nano
- LCD Дисплей с I2c 20x4
- Энкодер

------------------------------------------
## Схема подключения: 


------------------------------------------
#### TODO
- [ ] Добавить текст в интро 3-го уровня. Заблокировать действия на первые 2 секунды 
- [ ] Добавить текст в интро 4-го уровня(где всего по 2). Заблокировать действия на первые 2 секунды  
- [ ] Добавить 5-й уровень
- [ ] Виборомотор добавить
- [ ] Плавный неблокирующий звук
- [ ] Написать звук на каждое событие 
- [ ] Добавить физический выключатель звука
- [ ] Персонаж должен выйти из игры физически

~~- [ ] Переделать вывод текста на русский язык~~
- [ ] несколько концовок
- [ ] Добавить ещё 3 карты
- [ ] сделать уровень в 2 экрана
- [ ] Ловушки меняющие свое состояние при каждом действии игрока
- [х] переписать насраное на абстрактный  класс obj_on_map, наследовать на разные объекты ? 
- [х] Диалог при возвращении в игру
- [x] Второй уровень очень сложный (убрать монстра со 2-го уровня)
- [х] выключить отображение хп на end_lvl
------------------------------------------

- [x] Сюжет
- [x] Отображать hp 
- [x] Секретный уровень
- [x] Пропуск фразы диалога по клику
- [x] Анимации на уровни 2, 3... 
- [x] Расположить ловушки и монстров на каждом уровне
- [x] 2 двери на одном уровне (2 lvl)
- [x] Спрайт стены поправить
- [x] Класс для монстра 
- [x] Настройка ориентации монстра в классе монстра
- [x] на уровне с фонарём гаснет подсветка во время мерцания. Весь уровень фонарь мерцает
- [x] Перейти на любой уровень по количеству кликов(дебаг) начиная с 2 кликов
- [x] Один из монстров на 3 уровне долджен идти вниз-вверх на анимации. Как на уровне
- [х] Анимацая проигрыша (возврат к началу игры/reset)
- [x] Анимация gate() - (открывается дверь) Весь экран заливается спрайтом стены, постепенно стирая их в сторону. Гаснет подсветка
~~- [ ] Анимация проигрыша для каждого уровня~~ Много памяти уйдёт, думаю
------------------------------------------
#### BUGS

- [ ] анимация лица на lvl 1 (ловушки)
- [ ] анимацию лица на таймер
- [ ] Поправить спрайты на ОМ лвл
- [ ] secret lvl не работает
- [ ] хреново ресетит на end_lvl
- [х] сейчас в начале уровня нужно совершить действие для старта(отрисовки экрана) - переделать 
- [х] Включить подсветку на уровне после фонаря (3)
- [x] На некоторых анимациях рот без лица - фича
- [x] Иногда буквы в диалогах внутри лица  

