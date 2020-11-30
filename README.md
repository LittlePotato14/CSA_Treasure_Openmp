# Точилина Полина БПИ199.
## Вариант 25
###  Первая задача об Острове Сокровищ (с Openmp)
Шайка пиратов под предводительством Джона Сильвера высадилась на берег Острова Сокровищ. Не смотря на добытую карту старого Флинта, местоположение сокровищ попрежнему остается загадкой, поэтому искать клад приходится практически на ощупь. Так как Сильвер ходит на деревянной ноге, то самому бродить по джунглям ему не с руки. Джон Сильвер поделил остров на участки, а пиратов на небольшие группы. Каждой группе поручается искать клад на одном из участков, а сам Сильвер ждет на берегу. Пираты, обшарив свою часть острова, возвращаются к Сильверу и докладывают о результатах. Требуется создать многопоточное приложение с управляющим потоком, моделирующее действия Сильвера и пиратов.

### Используемая модель
[Источник информации](https://it.wikireading.ru/hqzeY92SAG "Модель делегирования")

В модели делегирования есть два типа потоков: 1 - управляющий, 2 - рабочий. Управляющий поток создаёт рабочие и раздаёт им задания. 

Для взаимодействия между потоками использованы mutex (чтобы ограничивать одновременный доступ) и conditional variable (чтобы уведомлять потоки о новых задачах и прогбуждать их от ожидания).

# Код программы
## includes

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/includes.png "includes")

## Глобальные переменные

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/globals.png "globals")

## Считывание целочисленных значений
Переписан stoi, чтобы не было исключений

Написан метод, который считывает числа в интервале и сравнивает stoi с начальной строкой, чтобы не пропускать строки, которые просто начинаются с числа.

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/readInt.png "readInt")

## Вывод информации об острове
Данный метод используется, чтобы вывести все ячейки острова с текущей информацией о том, исследованы ли они

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/outIsland.png "outIsland")

## Приветствие пользователя и установка начальных значений
Данный метод объясняет пользователю суть программы и просит ввести его начальные данные для работы программы

Генерируется рандомное место сокровища на острове

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/setup.png "setup")

# Функция потоков-рабочих
## Поток ожидает новой задаче в своей ячейке вектора задач

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/group1.png "group1")

## Выполнение задачи
Поток завершается, если задача = -2 (код завершения)

В ином случае поток уведомляет о начале выполнения задания

Поток засыпает на время равное 5 / members (один пират исследует ячейку за 5 секунд), симулируя выполнение задания

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/group2.png "group2")

## Обработка информации
После пробуждения поток определяет, был ли найден клад и уведомляет главный поток о том, что работа завершена

Поток выводит информацию о текущем статусе острова после заверщения миссии

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/group3.png "group3")

# Функция main
## Активирует setup и настраивает локаль, рандом и количество потоков

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/main1.png "main1")

## Далее функция регулирует основной поток
Добавляем в очередь номера будущих потоков, симулирующих команды пиратов

Если введено команд больше, чем ячеек в острове, то лишние команды проигнорируются (останутся у корабля)

Распараллеливаем на количество потоков вызов функции для отработки метода группы (мастер туда не заходит)

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/main2.png "main2")

В цикле для каждой ячейки острова мастер ждет свободную команду и отдает ей на выполнение данную ячейку, если клад еще не был найден

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/main3.png "main3")

После того как был найден клад или остров закончился мастер выходит из цикла

Поток ожидает, пока не будет найден клад

(Например если была 1 команда и клад находился в последней ячейке, нам необходимо дождаться его нахождения после завершения цикла)

Далее мастер уведомляет все рабочие потоки о необходимости завершения и ожидает их

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/main4.png "main4")

# Тестирование программы
## Протестируем проверку ввода острова

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/checkIsland.png "checkIsland")

## Протестируем проверку ввода пиартов

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/checkPirates.png "checkPirates")

## Проверим на корретном вводе
Вводим все необходимые значения

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/input1.png "1")

Так как команда №1 в 18 раз больше двух других, она завершает свои задачи быстрее.

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/input2.png "2")

После нахождения клада командой, основной поток подождал возвращения двух оставшихся и программа завершилась

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/input3.png "3")

## Проверим на работе только с одной командой
Вводим все необходимые значения

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/input21.png "1")

Поток последовательно отдает ячейки команде, пока не будет найден клад.

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/input22.png "2")

## Проверим на ввод большего количества команд, чем есть ячеек на острове
Вводим все необходимые значения

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/input31.png "1")

Все команды кроме одного ушли выполнять задания (поток для последней команды не был создан)

![alt-текст](https://github.com/LittlePotato14/CSA_Treasure_Openmp/blob/master/screens/input32.png "2")
