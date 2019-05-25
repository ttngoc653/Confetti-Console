#include<thread>
#include<iostream>
#include<stdio.h>
#include<string>
#include <windows.h>

using namespace std;

int dem_giay = 0;

// move control to point
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void UpTo1(const char* msg)
{
	cout << "MSG = " << msg << endl;
	while (dem_giay<100)
	{
		dem_giay++;
		_sleep(500);
		gotoxy(10, 10);
		cout << dem_giay;
	}
}

void UpTo3(const char* msg)
{
	//cout << "MSG = " << msg << endl;
	_sleep(100);
	while (dem_giay<100)
	{
		dem_giay+=3;
		_sleep(500);
		gotoxy(15, 10);
		cout << dem_giay;
	}
}

int main()
{
	thread task(UpTo1, "Hello STDIO");
	thread task2(UpTo3, "Hello STDIO");
	task.join(); //ham se cho khi ham dc goi hoan thanh xog no moi cho di tiep
	task2.join();
	//getchar();
	return 0;
}