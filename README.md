# LampWithTimer
Таймер для стандартного настільного світильника з таймером відключення. Таймер вбудований для фоторезистивного проявлення друкованих плат з UV лампою.
![mainimg](https://github.com/JohnMWarren/LampWithTimer/blob/main/doc/LampTimer-1.jpg)
- 4 символьний 8 розрядний Led індикатор на чіпі TM1637
- просте керування таймером, з таймінгом від 10 секунд до 1 години
- побудований на стандартних модулях, з контроллером ATMEGA328 (arduino Nano)
- режими: Звичайний світильник, та робота з таймером
    - повертання енкодера вправо/вліво задає або віднімає хвилини від 0 до 60 хв.
    - повертання з натисканням енкодера вправо/вліво задає або віднімає секунди с кроком 10 секунд від 0 до 50 сек.
    - подвійне натискання - вмикає зворотній відлік таймеру. Лампа вмикається і працює до обнулення таймера.
    - будьякий поворот енкодера зупиняє таймер та вимикає лампу.


### Сумісність
Сумісна зі всіма Arduino платформами (використовуються Arduino-функції)

## Зміст
- [Інсталювання](#install)
- [Вступ](#base)
- [Инициализация](#init)
- [Використання](#usage)
- [Версії](#versions)
- [Баги і фідбек](#feedback)

<a id="install"></a>
## Інсталювання
- Библіотеки можно знайти та встановити та встановити через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2

- [Завантажити бібліотеку енкодера](https://github.com/JohnMWarren/LampWithTimer/blob/main/src/EncButton-main.zip)
- [Завантажити бібліотеку Індикатора TM1637](https://github.com/JohnMWarren/LampWithTimer/blob/main/src/TM1637-clock.zip) 
 
 .zip архивом для ручного інсталювання: 

  - Розпакувати та перемістити в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
  - Розпакувати та перемістити в *C:\Program Files\Arduino\libraries* (Windows x32)
  - Розпакувати та перемістити в *Документы/Arduino/libraries/*
  - (Arduino IDE) автоматичне інсталювання из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* і вказати завантажений архів
  



<a id="base"></a>
## Вступ

Лама з UV діапазоні використовується для фоторезиста друкованих плат, для їх подальшої обробки в процесі травлення для видалення мідної вольги в чітко зазначених місцях. Також в процесі на плату наноситься фоторезистивна маска і шелкографія які теж експонуються UV лампою. Час експонування дуже важливий, і чекати біля лампи з секундоміром це пуста втрата часу, для цього винайшли автоматику. Можна також придбати в магазині реле часу для розетки, і встановити його між розеткою та світильником, але так не цікаво.

Схема складена за допомогою програми Fritzing, де просто і наглядно описано як що і до чого під'єнувати. Для проекту обрані стандартні модулі де не треба припаювати резистори та конденсатори, що максимально спрощує сбірку всього пристрою.

Програма використовує  6364 байт (20%) пам'яті контроллера. Весь код контролює обробку нажать та поворотів енкодера та інтерпретацію в код виводу на лед індикатор TM1637. Будьте уважні для живлення використовується блок Hi-Link 5V 1A на два контакта якого подається напруга 220В. 

**ТОЖ УВАГА ТУТ Є ВИСОКА НАПРУГА, БУДЬТЕ ОБЕРЕЖНІ**


### Схема

![shemaimg](https://github.com/JohnMWarren/LampWithTimer/blob/main/doc/Connection2.jpg)

- З'єднання:
  + Encoder EC11      Dt-(Nano pin 4), Clk-(Nano pin 5), Sw-(Nano pin 3)
  + TM1637 indicator  CLK-(Nano pin 6) DIO-(Nano pin 7)
  + Gnd to Gnd, 5V to Nano pin Vin
  + Rele (Nano pin 8).



<a id="init"></a>
## Використання


<details>
<summary>Початкові налаштування</summary>

```cpp
void setup() {

  pinMode(relePin, OUTPUT);                   // rele 250 v
  pinMode(relePin13, OUTPUT);                 // This pin need for led indicator relay work

  digitalWrite(relePin13, 1);
  digitalWrite(relePin, 1);                   // when power On - lamp is normal mode ON, but is Off when touch encoder

  Serial.begin(9600);

  tm1637.set(BRIGHT_TYPICAL);
  tm1637.init();

  Intro();                                   // here is a blank start screen

}
```
- Ініціалізуємо серіал порт для того щоб бачити чи все вірно працює в коді
- Підключаеємо вихід реле як  OUTPUT та ставимо йому стан 1 - тобто ON. Це потрібно для того щоб при ввімкнені лампа працювала як звичайний світильник.
- BRIGHT_TYPICAL - ставимо максимальну яскравість індикатору.
- Intro() - це перша ынформація що відобразиться на індикаторі, зараз по замовчюванню - пусто, і точки між 2 і 3 символом теж вимкнені.

</details>
<details>
<summary>Постійний цикл програми</summary>

```cpp
void loop() {

  enc.tick();
  EncoderRead();

  displayData();

  StartTimer();

  digitalWrite(relePin13, (digitalRead(relePin)));    // indication led rele state

}
```
-  enc.tick() - постійне опитування енкодера - чи щось трапилось.
-  EncoderRead() - обробка дій з енкодером та обмеження змінних хвилин та секунд
-  displayData() - дані що надходять з  EncoderRead() виводяться на led індикатор
-  StartTimer() - запуск зворотнього відліку таймеру
-  і остання дія - це сигнальний вбудований світлодіод який дублює лампочку.
  
</details>

Відео з результатом можна переглянути тут:
[![mainimg](https://github.com/JohnMWarren/LampWithTimer/blob/main/doc/video.JPG)](https://youtu.be/NNdjo4cXRUg)

<a id="versions"></a>
## Версії
- v1.1 - таймер очікує вводу данних одразу після включення, індикатор світиться
- v1.2 - при включенні лампа одразу загорається і таймер очікує будь якої дії енкодера для запуску

    
<a id="feedback"></a>
## Зворотній зв'язок
При проблемах створюйте **Issue**, або пишіть на почту [h.warren4242@gmail.com](h.warren4242@gmail.com)  
