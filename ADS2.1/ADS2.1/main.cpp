#include <iostream>
//#include<ctime>
#include <time.h>
#include <math.h>
#include "Tree.h"

using namespace std;
typedef int DataType;
typedef int KeyType;

typedef unsigned long long INT_64;
//��������� ��������� ����� ������� �����������
//���������� � ��������� ���������� LineRand()
static INT_64 RRand = 15750;
const INT_64 mRand = (1 << 63) - 1;
const INT_64 aRand = 6364136223846793005;
const INT_64 cRand = 1442695040888963407;
//������� ��������� ������� ���������� ����� �� ����� ����������
void sRand() { srand(time(0)); RRand = (INT_64)rand(); }
//������� ��������� ���������� �����
//�������� ������������ ��������� Xi+1=(a*Xi+c)%m
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
	//�������� ������ ��� 64 � ��������� ������ ���� INT_64
	MyTree< INT_64, int > tree;
	//������ ��� ������, ������� ������������ � ������
	INT_64* m = new INT_64[n];
	//��������� ������� ���������� �����
	sRand();
	//���������� ������ � ������� ���������� �� ���������� �������
	int nn=0;
	bool b = true;
	for (int i = 0; i < n; i++)
	{
		m[i] = LineRand();
		tree.Insert(m[i], 1, b,nn);
	}
	//����� ������� ������ �� �����
	cout << "items count:" << tree.AskForSize() << endl;
	//��������� ��������� ����������� �������, �������� � ������
	double I = 0;
	double D = 0;
	double S = 0;
	//��������� ������ ��������, 10% - ������� ��������
	for (int i = 0; i < n / 2; i++)
		if (i % 10 == 0) //10% ��������
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
			//��������� ���������� ��� ������ �������� ������
			catch (int) { S += nn; }
		}
		else //90% �������� ��������
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
			//��������� ���������� ��� ������ �������� ������
			catch (int) { S += nn; }
		} //����� �����
		//����� �����������:
		//����� ������� ������ ����� �����
	cout << "items count:" << tree.AskForSize() << endl;
	//������������� ������ ����������� �������� BST
	cout << "1.39*log2(n)=" << 1.39 * (log((double)n) / log(2.0)) << endl;
	//����������������� ������ ����������� �������
	cout << "Count insert: " << I / (n / 2) << endl;
	//����������������� ������ ����������� ��������
	cout << "Count delete: " << D / (n / 2) << endl;
	//����������������� ������ ����������� ������
	cout << "Count search: " << S / (n / 2) << endl;
	//������������ ������ ������� m[]
	delete[] m;
}

void test_ord(int n)
{
	//�������� ������ ��� 64 � ��������� ������ ���� INT_64
	MyTree< INT_64, int > tree;
	//������ ��� ������, ������� ������������ � ������
	INT_64* m = new INT_64[n];
	//���������� ������ � ������� ���������� � ������������� ������� //������� �� ��������� [0, 10000, 20000, ... ,10000*n]
	int nn = 0;
	bool b = true;
	for (INT_64 i = 0; i < n; i++) {
		m[i] = i * 10000;
		tree.Insert(m[i], 1, b,nn);
	}
	//����� ������� ������ �� �����
	cout << "items count:" << tree.AskForSize() << endl;
	//��������� ��������� ����������� �������, �������� � ������
	double I = 0;
	double D = 0;
	double S = 0;
	//��������� ������� ���������� �����
	sRand();
	//��������� ������ ��������, 10% - ������� ��������
	for (int i = 0; i < n / 2; i++)
	{
		if (i % 10 == 0) //10% ��������
		{
			INT_64 k = LineRand() % (10000 * n);
			k = k + k % 2; //��������� �������� ����
			nn = 0;
			tree.Delete(k, b, nn);
			D += nn;
			nn = 0;
			tree.Insert(m[rand() % n], 1, b, nn);
			I += nn;
			k = LineRand() % (10000 * n);
			k = k + !(k % 2); // ��������� �������� ����
			try {
				nn = 0;
				tree.Read(k, nn);
				S += nn;
			}
			//��������� ���������� ��� ������ �������� ������
			catch (int) { S += nn; }
		}
		else //90% �������� ��������
		{
			int ind = rand() % n;
			nn = 0;
			tree.Delete(m[ind], b, nn);
			D += nn;
			INT_64 k = LineRand() % (10000 * n);
			k = k + k % 2; // ��������� ������ ����
			nn = 0;
			tree.Insert(k, 1, b, nn);
			I += nn;
			m[ind] = k;
			try {
				nn = 0;
				tree.Read(m[rand() % n], nn);
				S += nn;
			}
			//��������� ���������� ��� ������ �������� ������
			catch (int) { S += nn; }
		}
	}
	//����� �����������:
	// ����� ������� ������ ����� �����
	cout << "items count:" << tree.AskForSize() << endl;
	//������������� ������ ����������� �������� BST
	cout << "n/2 =" << n / 2 << endl;
	//����������������� ������ ����������� �������
	cout << "Count insert: " << I / (n / 2) << endl;
	//����������������� ������ ����������� ��������
	cout << "Count delete: " << D / (n / 2) << endl;
	//����������������� ������ ����������� ������
	cout << "Count search: " << S / (n / 2) << endl;
	//������������ ������ ������� m[]
		delete[] m;
} //����� �����

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

	cout << "0 - ��������� ������ ���������" << endl;
	cout << "1 - ����� ������� ������" << endl;
	cout << "2 - ������� ������" << endl;
	cout << "3 - �������� ������ �� �������" << endl;
	cout << "4 - ������ �������� � �������� ������" << endl;
	cout << "5 - �������� ����� �������� � �������� ������" << endl;
	cout << "6 - �������� ������ � �������� ������" << endl;
	cout << "7 - ����������� ����������� ������ ��� �������� � �������� ������" << endl;
	cout << "8 - ������������ ������ ������ � ������ � ������� ������ ����� �� t->Lt->Rt" << endl;
	cout << "9 - ������ ��������� ������" << endl;
	cout << "��������" << endl;
	cout << "10 - ��������� �� ������ ���� � ������ � ����������� ������" << endl;
	cout << "11 - ��������� �� ��������� ���� � ������ � ������������ ������" << endl;
	cout << "12 - ��������� �� ���������" << endl;
	cout << "13 - ��������� �� ����������"<<endl;
	cout << "14 - �������� ��������� ���������"<<endl;
	cout << "15 - ������ �� ������ � ������ �������� ���� � ������"<<endl;
	cout << "16 - ������ �� ������ � ������ �������� ���� � ������"<<endl;
	cout << "�������� ��������" << endl;
	cout << "17 - ��������� �� ������ ���� � ������ � ����������� ������" << endl;
	cout << "18 - ��������� �� ��������� ���� � ������ � ������������ ������" << endl;
	cout << "19 - ��������� �� ���������" << endl;
	cout << "20 - ��������� �� ����������" << endl;
	cout << "21 - �������� ��������� ���������" << endl;
	cout << "22 - ������ �� ������ � ������ �������� ���� � ������"<<endl;
	cout << "23 - ������ �� ������ � ������ �������� ���� � ������"<<endl;
	cout << "24 - ���� ���������� ������" << endl;
	cout << "25 - ���� ������������ BST-������"<<endl;
	cout << "26 - �������� ������� ������ ������" << endl << endl;

	/*NewTree.Insert(100, 11, b, nn);
	NewTree.Insert(110, 12, b, nn);
	NewTree.Insert(10, 13, b, nn);
	NewTree.Insert(1, 14, b, nn);
	NewTree.Insert(101, 15, b, nn);
	NewTree.Insert(111, 16, b, nn);
	NewTree.Insert(11, 17, b, nn);
	NewTree.Insert(1000, 18, b, nn);*/

	do {
		cout << "������� ����� �������: ";
		cin >> command;
		switch (command) {
		
		case 0:	//��������� ������ ���������
			cout << "������ ��������� ���������" << endl;
			break;
		
		case 1:	//����� ������� ������
			cout << "������ ������ ���������� " << NewTree.AskForSize() << " ���������" << endl << endl;
			break;
		
		case 2:	//������� ������
			NewTree.DeleteAll();
			cout << "������ ������" << endl << endl;
			break;
		
		case 3:	//�������� ������ �� �������
			if (NewTree.EmptyCheck() == 1) {
				cout << "C����� ����" << endl << endl;
			}
			else {
				cout << "������ �� ����" << endl << endl;
			}
			break;
		
		case 4:	//������ �������� � �������� ������
			cout << "������� �������� �����: ";
			cin >> key;
			try {
				cout << "�������� � �������� ������ = " << NewTree.Read(key, nn) << endl << endl;		
			}
			catch (int) {
				cout << "������ ���� � ������ �����������" << endl << endl;
			}
			break;
		
		case 5:	//�������� ����� �������� � �������� ������
			cout << "������� ����: ";
			cin >> key;
			cout << "������� ����� ��������: ";
			cin >> value;
			if (NewTree.Insert(key, value, b, nn) == true) {
				cout << "�������� ��������" << endl<<endl;
			}
			else {
				cout << "�������� �� ��������" << endl << endl;
			}
			break;
		
		case 6:	//�������� ������ � �������� ������
			cout << "������� ����: ";
			cin >> key;
			if (NewTree.Delete(key, b, nn) == true) {
				cout << "������� � ������ ������ ������"<<endl<<endl;
			}
			else {
				cout << "������ ���� � ������ �����������" << endl << endl;
			}
			break;
		
		case 7:	//����������� ����������� ������ ��� �������� � �������� ������
			cout << "������� ����: ";
			cin >> key;
			if (NewTree.KeyNumber(key, nn) != -1) {
				cout << "�������� � ������ ������ ����� ���������� ����� = " << NewTree.KeyNumber(key, nn) << endl << endl;
			}
			else {
				cout << "������ ���� � ������ �����������" << endl << endl;
			}
			break;
		
		case 8:	//������������ ������ ������ � ������ � ������� ������ ����� �� tLR
			NewTree.StartPrint();
			cout << endl;
			break;
		
		case 9://������ ��������� ������
			NewTree.PrintTree();
			break;
		
		case 10://��������� �� ������ ���� � ������ 
			try {
				it.begin();
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ����������" << endl << endl;
			}
			break;

		case 11://��������� �� ��������� ���� � ������ 
			try {
				it.end();
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ����������" << endl << endl;
			}
			break;

		case 12://��������� �� ���������
			try {
				it.next();
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ���������� " << endl << endl;
			}
			break;

		case 13://��������� �� ����������
			try {
				it.prev();
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ����������" << endl << endl;
			}
			break;

		case 14://�������� ��������� ���������
			if (it.is_off() == true){
				cout<<"�������� �� ����������"<<endl<<endl;
			}
			else {
				cout << "�������� ����������" << endl<<endl;
			}
			break;

		case 15://������ �� ������
			try {
				cout << *it<<endl<<endl;
			}
			catch (int) {
				cout << "�������� �� ��������� ������"<<endl<<endl;
			}
			break;

		case 16://������ �� ������
			try {
				cout << "������� ����� ��������: ";
				cin >> *it;
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ��������� ������"<<endl << endl;
			}
			break;

		case 17://��������� �� ������ ���� � ������ 
			try {
				rit.rbegin();
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ����������" << endl << endl;
			}
			break;

		case 18://��������� �� ��������� ���� � ������ 
			try {
				rit.rend();
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ����������" << endl << endl;
			}
			break;

		case 19://��������� �� ���������
			try {
				rit.next();
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ����������" << endl << endl;
			}
			break;

		case 20://��������� �� ����������
			try {
				rit.prev();
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ����������" << endl << endl;
			}
			break;

		case 21://�������� ��������� ���������
			if (rit.is_off() == true) {
				cout << "�������� �� ����������" << endl << endl;
			}
			else {
				cout << "�������� ����������" << endl << endl;
			}
			break;

		case 22://������ �� ������
			try {
				cout << *rit;
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ��������� ������"<<endl<<endl;
			}
			break;

		case 23://������ �� ������
			try {
				cout << "������� ����� ��������: ";
				cin >> *rit;
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ��������� ������"<<endl<<endl;
			}
			break;

		case 24://���� ���������� ������
			cout << "������� ��������� ���������" << endl;
			cin >> key;
			test_rand(key);
			break;
		
		case 25://���� ������������ ������
			cout << "������� ��������� ���������" << endl;
			cin >> key;
			test_ord(key);
			break;

		case 26://������� ������ ������
			cout << "0 - ��������� ������ ���������" << endl;
			cout << "1 - ����� ������� ������" << endl;
			cout << "2 - ������� ������" << endl;
			cout << "3 - �������� ������ �� �������" << endl;
			cout << "4 - ������ �������� � �������� ������" << endl;
			cout << "5 - �������� ����� �������� � �������� ������" << endl;
			cout << "6 - �������� ������ � �������� ������" << endl;
			cout << "7 - ����������� ����������� ������ ��� �������� � �������� ������" << endl;
			cout << "8 - ������������ ������ ������ � ������ � ������� ������ ����� �� t->Lt->Rt" << endl;
			cout << "9 - ������ ��������� ������" << endl;
			cout << "��������" << endl;
			cout << "10 - ��������� �� ������ ���� � ������ � ����������� ������" << endl;
			cout << "11 - ��������� �� ��������� ���� � ������ � ������������ ������" << endl;
			cout << "12 - ��������� �� ���������" << endl;
			cout << "13 - ��������� �� ����������" << endl;
			cout << "14 - �������� ��������� ���������" << endl;
			cout << "15 - ������ �� ������ � ������ �������� ���� � ������" << endl;
			cout << "16 - ������ �� ������ � ������ �������� ���� � ������" << endl;
			cout << "�������� ��������" << endl;
			cout << "17 - ��������� �� ������ ���� � ������ � ����������� ������" << endl;
			cout << "18 - ��������� �� ��������� ���� � ������ � ������������ ������" << endl;
			cout << "19 - ��������� �� ���������" << endl;
			cout << "20 - ��������� �� ����������" << endl;
			cout << "21 - �������� ��������� ���������" << endl;
			cout << "22 - ������ �� ������ � ������ �������� ���� � ������" << endl;
			cout << "23 - ������ �� ������ � ������ �������� ���� � ������" << endl;
			cout << "24 - ���� ���������� ������"<<endl;
			cout << "25 - ���� ������������ BST-������"<<endl;
			cout << "26 - �������� ������� ������ ������" << endl << endl;
			break;
		
		default:
			cout << "������� � ����� ������� �� ����������" << endl << endl;
		}
	} while (command != 0);
}
