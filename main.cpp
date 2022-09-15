#include <iostream>
//#include<ctime>
#include <time.h>
#include <math.h>
#include "Tree.h"

using namespace std;
typedef int DataType;
typedef int KeyType;

typedef unsigned long long INT_64;
//Генератор случайных чисел большой разрядности
//переменная и константы генератора LineRand()
static INT_64 RRand = 15750;
const INT_64 mRand = (1 << 63) - 1;
const INT_64 aRand = 6364136223846793005;
const INT_64 cRand = 1442695040888963407;
//функция установки первого случайного числа от часов компьютера
void sRand() { srand(time(0)); RRand = (INT_64)rand(); }
//функция генерации случайного числа
//линейный конгруэнтный генератор Xi+1=(a*Xi+c)%m
//habr.com/ru/post/208178
INT_64 LineRand()
{
	INT_64 y1, y2;
	y1 = (aRand * RRand + cRand) % mRand;
	y2 = (aRand * y1 + cRand) % mRand;
	RRand = y1 & 0xFFFFFFFF00000000LL ^ (y2 & 0xFFFFFFFF00000000LL) >> 32;
	return RRand;
}

void test_rand(int n)
{
	//создание дерева для 64 – разрядных ключей типа INT_64
	MyTree< INT_64, int > tree;
	//массив для ключей, которые присутствуют в дереве
	INT_64* m = new INT_64[n];
	//установка первого случайного числа
	sRand();
	//заполнение дерева и массива элементами со случайными ключами
	int nn=0;
	bool b = true;
	for (int i = 0; i < n; i++)
	{
		m[i] = LineRand();
		tree.Insert(m[i], 1, b,nn);
	}
	//вывод размера дерева до теста
	cout << "items count:" << tree.AskForSize() << endl;
	//обнуление счётчиков трудоёмкости вставки, удаления и поиска
	double I = 0;
	double D = 0;
	double S = 0;
	//генерация потока операций, 10% - промахи операций
	for (int i = 0; i < n / 2; i++)
		if (i % 10 == 0) //10% промахов
		{
			nn = 0;
			tree.Delete(LineRand(), b, nn);
			D += nn;
			nn = 0;
			tree.Insert(m[rand() % n], 1, b, nn);
			I += nn;
			try {
				nn = 0;
				tree.Read(LineRand(), nn);
				S += nn;
			}
			//обработка исключения при ошибке операции поиска
			catch (int) { S += nn; }
		}
		else //90% успешных операций
		{
			int ind = rand() % n;
			nn = 0;
			tree.Delete(m[ind], b, nn);
			D += nn;
			INT_64 key = LineRand();
			nn = 0;
			tree.Insert(key, 1, b, nn);
			I += nn;
			m[ind] = key;
			try {
				nn = 0;
				tree.Read(m[rand() % n], nn);
				S += nn;
			}
			//обработка исключения при ошибке операции поиска
			catch (int) { S += nn; }
		} //конец теста
		//вывод результатов:
		//вывод размера дерева после теста
	cout << "items count:" << tree.AskForSize() << endl;
	//теоретической оценки трудоёмкости операций BST
	cout << "1.39*log2(n)=" << 1.39 * (log((double)n) / log(2.0)) << endl;
	//экспериментальной оценки трудоёмкости вставки
	cout << "Count insert: " << I / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости удаления
	cout << "Count delete: " << D / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости поиска
	cout << "Count search: " << S / (n / 2) << endl;
	//освобождение памяти массива m[]
	delete[] m;
}

void test_ord(int n)
{
	//создание дерева для 64 – разрядных ключей типа INT_64
	MyTree< INT_64, int > tree;
	//массив для ключей, которые присутствуют в дереве
	INT_64* m = new INT_64[n];
	//заполнение дерева и массива элементами с возрастающими чётными //ключами на интервале [0, 10000, 20000, ... ,10000*n]
	int nn = 0;
	bool b = true;
	for (INT_64 i = 0; i < n; i++) {
		m[i] = i * 10000;
		tree.Insert(m[i], 1, b,nn);
	}
	//вывод размера дерева до теста
	cout << "items count:" << tree.AskForSize() << endl;
	//обнуление счётчиков трудоёмкости вставки, удаления и поиска
	double I = 0;
	double D = 0;
	double S = 0;
	//установка первого случайного числа
	sRand();
	//генерация потока операций, 10% - промахи операций
	for (int i = 0; i < n / 2; i++)
	{
		if (i % 10 == 0) //10% промахов
		{
			INT_64 k = LineRand() % (10000 * n);
			k = k + k % 2; //случайный нечётный ключ
			nn = 0;
			tree.Delete(k, b, nn);
			D += nn;
			nn = 0;
			tree.Insert(m[rand() % n], 1, b, nn);
			I += nn;
			k = LineRand() % (10000 * n);
			k = k + !(k % 2); // случайный нечётный ключ
			try {
				nn = 0;
				tree.Read(k, nn);
				S += nn;
			}
			//обработка исключения при ошибке операции поиска
			catch (int) { S += nn; }
		}
		else //90% успешных операций
		{
			int ind = rand() % n;
			nn = 0;
			tree.Delete(m[ind], b, nn);
			D += nn;
			INT_64 k = LineRand() % (10000 * n);
			k = k + k % 2; // случайный чётный ключ
			nn = 0;
			tree.Insert(k, 1, b, nn);
			I += nn;
			m[ind] = k;
			try {
				nn = 0;
				tree.Read(m[rand() % n], nn);
				S += nn;
			}
			//обработка исключения при ошибке операции поиска
			catch (int) { S += nn; }
		}
	}
	//вывод результатов:
	// вывод размера дерева после теста
	cout << "items count:" << tree.AskForSize() << endl;
	//теоретической оценки трудоёмкости операций BST
	cout << "n/2 =" << n / 2 << endl;
	//экспериментальной оценки трудоёмкости вставки
	cout << "Count insert: " << I / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости удаления
	cout << "Count delete: " << D / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости поиска
	cout << "Count search: " << S / (n / 2) << endl;
	//освобождение памяти массива m[]
		delete[] m;
} //конец теста

int main()
{
	setlocale(LC_ALL, "Russian");

	MyTree<int, int> NewTree;
	MyTree<int, int>::Iterator it(NewTree);
	MyTree<int, int>::ReverseIterator rit(NewTree);
	int command;
	int value;
	int key;
	bool b;
	int nn;

	cout << "0 - Завершить работу программы" << endl;
	cout << "1 - Опрос размера дерева" << endl;
	cout << "2 - Очистка дерева" << endl;
	cout << "3 - Проверка дерева на пустоту" << endl;
	cout << "4 - Чтение значения с заданным ключом" << endl;
	cout << "5 - Включить новое значение с заданным ключом" << endl;
	cout << "6 - Удаление данных с заданным ключом" << endl;
	cout << "7 - Определение порядкового номера для элемента с заданным ключом" << endl;
	cout << "8 - Формирование списка ключей в дереве в порядке обхода узлов по t->Lt->Rt" << endl;
	cout << "9 - Печать структуры дерева" << endl;
	cout << "Итератор" << endl;
	cout << "10 - Установка на первый узел в дереве с минимальным ключом" << endl;
	cout << "11 - Установка на последний узел в дереве с максимальным ключом" << endl;
	cout << "12 - Установка на следующий" << endl;
	cout << "13 - Установка на предыдущий"<<endl;
	cout << "14 - Проверка состояния итератора"<<endl;
	cout << "15 - Доступ по чтению к данным текущего узла в дерере"<<endl;
	cout << "16 - Доступ по записи к данным текущего узла в дереве"<<endl;
	cout << "Обратный итератор" << endl;
	cout << "17 - Установка на первый узел в дереве с минимальным ключом" << endl;
	cout << "18 - Установка на последний узел в дереве с максимальным ключом" << endl;
	cout << "19 - Установка на следующий" << endl;
	cout << "20 - Установка на предыдущий" << endl;
	cout << "21 - Проверка состояния итератора" << endl;
	cout << "22 - Доступ по чтению к данным текущего узла в дерере"<<endl;
	cout << "23 - Доступ по записи к данным текущего узла в дереве"<<endl;
	cout << "24 - Тест случайного дерева" << endl;
	cout << "25 - Тест вырожденного BST-дерева"<<endl;
	cout << "26 - Повторно вывести список команд" << endl << endl;

	/*NewTree.Insert(100, 11, b, nn);
	NewTree.Insert(110, 12, b, nn);
	NewTree.Insert(10, 13, b, nn);
	NewTree.Insert(1, 14, b, nn);
	NewTree.Insert(101, 15, b, nn);
	NewTree.Insert(111, 16, b, nn);
	NewTree.Insert(11, 17, b, nn);
	NewTree.Insert(1000, 18, b, nn);*/

	do {
		cout << "Введите номер команды: ";
		cin >> command;
		switch (command) {
		
		case 0:	//Завершить работу программы
			cout << "Работа программы завершена" << endl;
			break;
		
		case 1:	//Опрос размера дерева
			cout << "Размер дерева составляет " << NewTree.AskForSize() << " элементов" << endl << endl;
			break;
		
		case 2:	//Очистка дерева
			NewTree.DeleteAll();
			cout << "Список очищен" << endl << endl;
			break;
		
		case 3:	//Проверка дерева на пустоту
			if (NewTree.EmptyCheck() == 1) {
				cout << "Cписок пуст" << endl << endl;
			}
			else {
				cout << "Список не пуст" << endl << endl;
			}
			break;
		
		case 4:	//Чтение значения с заданным ключом
			cout << "Введите значение ключа: ";
			cin >> key;
			try {
				cout << "Значение с заданным ключом = " << NewTree.Read(key, nn) << endl << endl;		
			}
			catch (int) {
				cout << "Данный ключ в дереве отсутствует" << endl << endl;
			}
			break;
		
		case 5:	//Включить новое значение с заданным ключом
			cout << "Введите ключ: ";
			cin >> key;
			cout << "Введите новое значение: ";
			cin >> value;
			if (NewTree.Insert(key, value, b, nn) == true) {
				cout << "Значение включено" << endl<<endl;
			}
			else {
				cout << "Значение не включено" << endl << endl;
			}
			break;
		
		case 6:	//Удаление данных с заданным ключом
			cout << "Введите ключ: ";
			cin >> key;
			if (NewTree.Delete(key, b, nn) == true) {
				cout << "Элемент с данным ключом удален"<<endl<<endl;
			}
			else {
				cout << "Данный ключ в дереве отсутствует" << endl << endl;
			}
			break;
		
		case 7:	//Определение порядкового номера для элемента с заданным ключом
			cout << "Введите ключ: ";
			cin >> key;
			if (NewTree.KeyNumber(key, nn) != -1) {
				cout << "Значение с данным ключом имеет порядковый номер = " << NewTree.KeyNumber(key, nn) << endl << endl;
			}
			else {
				cout << "Данный ключ в дереве отсутствует" << endl << endl;
			}
			break;
		
		case 8:	//Формирование списка ключей в дереве в порядке обхода узлов по tLR
			NewTree.StartPrint();
			cout << endl;
			break;
		
		case 9://Печать структуры дерева
			NewTree.PrintTree();
			break;
		
		case 10://Установка на первый узел в дереве 
			try {
				it.begin();
				cout << endl;
			}
			catch (int) {
				cout << "Итератор не установлен" << endl << endl;
			}
			break;

		case 11://Установка на последний узел в дереве 
			try {
				it.end();
				cout << endl;
			}
			catch (int) {
				cout << "Итератор не установлен" << endl << endl;
			}
			break;

		case 12://Установка на следующий
			try {
				it.next();
				cout << endl;
			}
			catch (int) {
				cout << "Итератор не установлен " << endl << endl;
			}
			break;

		case 13://Установка на предыдущий
			try {
				it.prev();
				cout << endl;
			}
			catch (int) {
				cout << "Итератор не установлен" << endl << endl;
			}
			break;

		case 14://Проверка состояния итератора
			if (it.is_off() == true){
				cout<<"Итератор не установлен"<<endl<<endl;
			}
			else {
				cout << "Итератор установлен" << endl<<endl;
			}
			break;

		case 15://Доступ по чтению
			try {
				cout << *it<<endl<<endl;
			}
			catch (int) {
				cout << "Итератор за пределами списка"<<endl<<endl;
			}
			break;

		case 16://Доступ по записи
			try {
				cout << "Введите новое значение: ";
				cin >> *it;
				cout << endl;
			}
			catch (int) {
				cout << "Итератор за пределами списка"<<endl << endl;
			}
			break;

		case 17://Установка на первый узел в дереве 
			try {
				rit.rbegin();
				cout << endl;
			}
			catch (int) {
				cout << "Итератор не установлен" << endl << endl;
			}
			break;

		case 18://Установка на последний узел в дереве 
			try {
				rit.rend();
				cout << endl;
			}
			catch (int) {
				cout << "Итератор не установлен" << endl << endl;
			}
			break;

		case 19://Установка на следующий
			try {
				rit.next();
				cout << endl;
			}
			catch (int) {
				cout << "Итератор не установлен" << endl << endl;
			}
			break;

		case 20://Установка на предыдущий
			try {
				rit.prev();
				cout << endl;
			}
			catch (int) {
				cout << "Итератор не установлен" << endl << endl;
			}
			break;

		case 21://Проверка состояния итератора
			if (rit.is_off() == true) {
				cout << "Итератор не установлен" << endl << endl;
			}
			else {
				cout << "Итератор установлен" << endl << endl;
			}
			break;

		case 22://Доступ по чтению
			try {
				cout << *rit;
				cout << endl;
			}
			catch (int) {
				cout << "Итератор за пределами списка"<<endl<<endl;
			}
			break;

		case 23://Доступ по записи
			try {
				cout << "Введите новое значение: ";
				cin >> *rit;
				cout << endl;
			}
			catch (int) {
				cout << "Итератор за пределами списка"<<endl<<endl;
			}
			break;

		case 24://Тест случайного дерева
			cout << "Введите количесто элементов" << endl;
			cin >> key;
			test_rand(key);
			break;
		
		case 25://Тест вырожденного дерева
			cout << "Введите количесто элементов" << endl;
			cin >> key;
			test_ord(key);
			break;

		case 26://Вывести список команд
			cout << "0 - Завершить работу программы" << endl;
			cout << "1 - Опрос размера дерева" << endl;
			cout << "2 - Очистка дерева" << endl;
			cout << "3 - Проверка дерева на пустоту" << endl;
			cout << "4 - Чтение значения с заданным ключом" << endl;
			cout << "5 - Включить новое значение с заданным ключом" << endl;
			cout << "6 - Удаление данных с заданным ключом" << endl;
			cout << "7 - Определение порядкового номера для элемента с заданным ключом" << endl;
			cout << "8 - Формирование списка ключей в дереве в порядке обхода узлов по t->Lt->Rt" << endl;
			cout << "9 - Печать структуры дерева" << endl;
			cout << "Итератор" << endl;
			cout << "10 - Установка на первый узел в дереве с минимальным ключом" << endl;
			cout << "11 - Установка на последний узел в дереве с максимальным ключом" << endl;
			cout << "12 - Установка на следующий" << endl;
			cout << "13 - Установка на предыдущий" << endl;
			cout << "14 - Проверка состояния итератора" << endl;
			cout << "15 - Доступ по чтению к данным текущего узла в дерере" << endl;
			cout << "16 - Доступ по записи к данным текущего узла в дереве" << endl;
			cout << "Обратный итератор" << endl;
			cout << "17 - Установка на первый узел в дереве с минимальным ключом" << endl;
			cout << "18 - Установка на последний узел в дереве с максимальным ключом" << endl;
			cout << "19 - Установка на следующий" << endl;
			cout << "20 - Установка на предыдущий" << endl;
			cout << "21 - Проверка состояния итератора" << endl;
			cout << "22 - Доступ по чтению к данным текущего узла в дерере" << endl;
			cout << "23 - Доступ по записи к данным текущего узла в дереве" << endl;
			cout << "24 - Тест случайного дерева"<<endl;
			cout << "25 - Тест вырожденного BST-дерева"<<endl;
			cout << "26 - Повторно вывести список команд" << endl << endl;
			break;
		
		default:
			cout << "Команды с таким номером не существует" << endl << endl;
		}
	} while (command != 0);
}
