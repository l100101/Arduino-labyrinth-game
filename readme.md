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
- [ ] переписать насраное на абстрактный  класс obj_on_map, наследовать на разные объекты ? 
- [ ] Виборомотор добавить
- [ ] Плавный неблокирующий звук
- [ ] Написать звук на каждое событие 

~~- [ ] Переделать вывод текста на русский язык~~
- [ ] несколько концовок
- [ ] Добавить ещё 3 карты
- [ ] сделать уровень в 2 экрана
- [ ] Ловушки меняющие свое состояние при каждом нажтии
- [ ] Диалог при возвращении в игру

- [ ] выключить отображение хп на end_lvl
- [ ] хреново ресетит на end_lvl
- [ ] Анимация проигрыша для каждого уровня
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
------------------------------------------
#### BUGS
- [ ] сейчас в начале уровня нужно совершить действие для старта(отрисовки экрана) - переделать 
- [ ] анимация лица на lvl 1 (ловушки)
- [ ] secret lvl не работает
- [x] На некоторых анимациях рот без лица - фича
- [x] Иногда буквы в диалогах внутри лица  

