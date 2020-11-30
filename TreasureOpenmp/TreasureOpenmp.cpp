/*
25. Первая задача об Острове Сокровищ. Шайка пиратов под
предводительством Джона Сильвера высадилась на берег Острова Сокровищ.
Не смотря на добытую карту старого Флинта, местоположение сокровищ попрежнему остается загадкой, поэтому искать клад приходится практически
на ощупь. Так как Сильвер ходит на деревянной ноге, то самому бродить по
джунглям ему не с руки. Джон Сильвер поделил остров на участки, а пиратов
на небольшие группы. Каждой группе поручается искать клад на одном из
участков, а сам Сильвер ждет на берегу. Пираты, обшарив свою часть
острова, возвращаются к Сильверу и докладывают о результатах. Требуется
создать многопоточное приложение с управляющим потоком, моделирующее
действия Сильвера и пиратов.
*/

// Точилинв Полина, БПИ199

// Вариация программы с Openmp

#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <queue>
#include <algorithm>
#include <omp.h>
#include <thread>

// высота острова
int fieldHeight;
// ширина острова
int fieldWidth;
// номер ячейки с сокровищем
int treasureNumber;
// Найден ли клад
bool found = false;
// группа нашедшая клад
int foundGroup;
// группы пиратов
std::vector<int> groups;
// задания для потоков
std::vector<int> tasks;
std::queue<int> freeThreads;
// остров
std::vector<std::string> island;

bool stoi(const std::string& str, int* p_value, std::size_t* pos = 0, int base = 10) {
	// обертка чтобы не было исключений
	try {
		*p_value = std::stoi(str, pos, base);
		return true;
	}
	catch (const std::exception& e)
	{
		return false;
	}
}

int readInt(int min = INT_MIN, int max = INT_MAX) {
	int result;
	bool correctInput = false;
	std::string tmp;
	std::cin >> tmp;
	correctInput = stoi(tmp, &result);

	while (!correctInput || std::to_string(result) != tmp || result < min || result > max) {
		std::cout << "Некорректный ввод. Попробуйте ещё раз\n";
		std::cin >> tmp;
		correctInput = stoi(tmp, &result);
	}

	return result;
}

void outIsland() {
	std::cout << "Остров:\n";
	for (int i = 0; i < fieldHeight; i++) {
		for (int j = 0; j < fieldWidth; j++)
			std::cout << island[i * fieldWidth + j] << " ";
		std::cout << std::endl;
	}
}

void setup() {
	// добавляем еще в векторы главный поток, чтобы сравнять индексацию потоков и ячеек векторов
	groups.push_back(0);
	tasks.push_back(0);

	std::cout << "Джон, мы видим остров!\n"
		<< "Однако он слишком большой, а карта не даёт четких указаний о местоположении сокровищ.\n"
		<< "Я предлагаю разделить остров на секции и пиратов на группы.\n"
		<< "(Один пират исследует один участок за 5 секунд. Группа из n пиратов работает в n раз быстрее)\n\n";

	std::cout << "Введите высоту острова (> 0):\n";
	fieldHeight = readInt(1);

	std::cout << "Введите ширину острова (> 0):\n";
	fieldWidth = readInt(1);

	std::cout << std::endl;

	for (int i = 0; i < fieldHeight * fieldWidth; i++) island.push_back("_");
	outIsland();
	std::cout << std::endl;

	std::cout << "Введите количество пиратов (> 0):\n";
	int people = readInt(1);
	int freePeople = people;
	std::cout << std::endl;

	for (int i = 0; ; i++) {
		if (freePeople == 0)
			break;
		std::cout << "Количество свободных пиратов:  " << freePeople << std::endl;
		std::cout << "Введите количество пиратов, из которых будет сформирована группа " << i + 1 << " (0 < n <= free pirates)\n";
		int currentGroup = readInt(1, freePeople);
		groups.push_back(currentGroup);
		freePeople -= currentGroup;
	}

	std::cout << "\nГруппы пиратов сформированы:\n";
	for(int i = 1; i < groups.size(); i++)
		std::cout << groups[i] << " ";
	std::cout << " \n\nВы прибыли на остров. Поиски начинаются.\n\n"
		<< "------------------------------------------------------------------\n\n";

	treasureNumber = std::rand() % (fieldHeight * fieldWidth);
}

void groupFunction(int members) {
	int tNumber = omp_get_thread_num();

	while (true) {
		// ожидаем поступления задания
		while (tasks[tNumber] == -1) {}

		int task = tasks[tNumber];

		// завершение миссии
		if (task == -2)
			return;

#pragma omp critical (cout)
		{
			std::cout << tNumber << " начала обследовать ячейку " << task << std::endl;
		}

		// выполнение задания
		std::this_thread::sleep_for(std::chrono::seconds(5 / members));

		// нашли ли клад
		if (task == treasureNumber) {
			island[task] = "X";

#pragma omp critical (cout)
			{
				std::cout << std::endl << tNumber << " закончила обследовать ячейку " << task << std::endl;
				std::cout << "Капитан, мы нашли клад! Дождёмся ушедшие группы и можно отплывать.\n";
				outIsland();
				std::cout << std::endl;
			}
#pragma omp critical (tasks)
			{
				foundGroup = tNumber;
				found = true;
				freeThreads.push(tNumber);
			}
			return;
		}

		island[task] = "O";

#pragma omp critical (cout)
		{
			std::cout << std::endl << tNumber << " закончила обследовать ячейку " << task << std::endl;
			outIsland();
			std::cout << std::endl;
		}

#pragma omp critical (tasks)
		{
			if (tasks[tNumber] != -2)
				tasks[tNumber] = -1;
			freeThreads.push(tNumber);
		}
	}
}

int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(0));
	setup();
	// устанавливаем количество потоков
	omp_set_num_threads(std::min((int)groups.size(), fieldHeight * fieldWidth + 1));

	// добавляем все потоки в очередь
	for (int i = 1; i < std::min((int)groups.size(), fieldHeight * fieldWidth + 1); i++) {
		freeThreads.push(i);
		tasks.push_back(-1);
	}

#pragma omp parallel
	{
		if (omp_get_thread_num() != 0)
			groupFunction(groups[omp_get_thread_num()]);

#pragma omp master
		{
			// для каждой ячейки острова
			for (int i = 0; i < fieldHeight * fieldWidth; i++) {
				// ждем свободного потока
				while (freeThreads.empty()) {}

				// проверяем найден ли клад
				if (found) break;

				// блокируем вывод и выводим команду.
#pragma omp critical (cout)
				{
					std::cout << "\nКоманде " << freeThreads.front() << " назначено новое задание в ячейке " << i << "\n\n";
				}

#pragma omp critical (tasks)
				{
					// добавляем задачу треду в очереди
					tasks[freeThreads.front()] = i;
					// удаляем тред из очереди
					freeThreads.pop();
				}
			}

			while (!found) {}

#pragma omp critical (tasks)
			{
				for (int i = 0; i < tasks.size(); i++) tasks[i] = -2;
			}
		}
	}
	// так как мы вышли из блока, все потоки завершились

	std::cout << "Клад найден группой " << foundGroup << "\n";

	std::cout << "Все пираты вернулись к кораблю, можно отплывать!\n";
}